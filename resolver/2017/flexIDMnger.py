import json
import os
import paho.mqtt.client as mqtt
#import unicodedata
import hashlib
import codecs

broker = "iot.eclipse.org"

# global variable for manage number in service ID
deviceID_cache = {}
dbQuery_cache = {}
collision_inc = 4
managerID = ''

db_insert = "/dbqeury/insert/" 
db_select = "/dbquery/select/"
db_delete = "/dbquery/delete/"
db_update = "/dbquery/update/"


def init_manager():
    global managerID
    # Example public key from Comodo's webpage..
    pubKey = '3048 0241 00C9 18FA CF8D EB2D EFD5 FD37 89B9 E069 EA97 FC20 5E35 F577 EE31 C4FB C6E4 4811 7D86 BC8F BAFA 362F 922B F01B 2F40 C744 2654 C0DD 2881 D673 CA2B 4003 C266 E2CD CB02 0301 0001'
    m = hashlib.sha1()
    m.update(pubKey.encode('utf-8'))
    managerID = m.hexdigest() + '0' # add 1byte flag 
    print("  --> Allocated DeviceID: " + managerID + "\n")
    
    

def send_DBquery(msg, topic, wait):

    queryID = codecs.encode(os.urandom(4), 'hex_codec').decode()
    queryID = '0x' + queryID
    while queryID in dbQuery_cache:
        queryID = codecs.encode(os.urandom(4), 'hex_codec').decode()
        queryID = '0x' + queryID

    queryID = '0x' + '001'
    dbQuery_cache[queryID] = "None"
    query = {"id": queryID , "sql": msg}
    query = json.dumps(query)

    db_client.publish(topic + managerID, query)

    #wait response from DB
    if wait:
        while dbQuery_cache[queryID] == "None":
            continue
    
    return queryID


def gen_flag(cache_bit, segment_bit, collision_mngt):
   
    flag = 0
    if collision_mngt > 15:
        raise Exception ('Collision range error')
    
    if cache_bit:
       flag = flag | 128
    
    if segment_bit:
        flag = flag | 64

    collision_mngt = collision_mngt << 2
    flag = flag | collision_mngt

    return flag


def join_genID(deviceID, flag):

    # deviceID's cache bit and segment flag are 0, thus only use 4 bit management number
    newID = deviceID + str(flag)
   
    while newID in deviceID_cache:
        flag = flag + collision_inc
        newID = deviceID + str(flag)
    deviceID_cache[newID] = "None"

    print ("\nCheck DeviceID collision...")
    queryID = send_DBquery("select ~", db_select, True)

    payload = dbQuery_cache[queryID]
    exist = payload.get('exist')

    del dbQuery_cache[queryID]
   
    deviceID_cache[newID] = True
    if not exist:
        return newID
    else:
        deviceID_cache[newID] = True
        return join_genID(deviceID, flag + collision_inc)


def join(tempID, payload):

    print ("\n\n ##Process - Join\n")
    relay = payload.get('relay')
   
    try:
        error = 0
        
        if relay == "none":
            deviceID = tempID
        else:
            deviceID = relay[-1]
            
        print ("Temporary DeviceID: " + deviceID)
 
        # Device ID collision check
        deviceID = join_genID(deviceID, 0)
        print ("Generated DeviceID: " + deviceID)


        neighbors = payload.get('neighbors')
        if neighbors is None:
            neighbors = "NULL"
       
        print ("\n----- Neighbor info.-----")
        for neighbor in neighbors:
            neighborIface = neighbor.get('neighborIface')
            neighborIpv4 = neighbor.get('neighborIpv4')
            neighborHwAddress = neighbor.get('neighborHwAddress')
            neighborFlexID = neighbor.get('neighborFlexID')
            if (neighborIface is None) or (neighborIface == "none"):
                neighborIface = "NULL"
            if (neighborIpv4 is None) or (neighborIpv4 == "none"):
                neighborIpv4 = "NULL"
            if (neighborHwAddress is None) or (neighborHwAddress == "none"):
                neighborHwAddress = "NULL"
            if (neighborFlexID is None) or (neighborFlexID == "none"):
                raise Exception ('Neighbor FlexID error')

            print ("neighborIface: " + neighborIface)
            print ("neighborIpv4: " + neighborIpv4)
            print ("neighborHwAddress: " + neighborHwAddress)
            print ("neighborFlexID: " + neighborFlexID)
            
            #cursor.execute("INSERT into Neighbor (deviceID, neighborIface, neighborIpv4, neighborHwAddress) values ('" + deviceID + "', '" + neighborIface + "', '" + neighborIpv4 + "', '" + neighborHwAddress + "')")
            send_DBquery("INSERT ~", db_insert, False)

        print ("\n-----Device Info.-----")
        uniqueCodes = payload.get('uniqueCodes')
        for data in uniqueCodes:
            ifaceType = data.get('ifaceType')
            hwAddress = data.get('hwAddress')
            ipv4 = data.get('ipv4')
            wifiSSID = data.get('wifiSSID')
            if ifaceType is None:
                ifaceType = "NULL"
            if hwAddress is None:
                ifaceType = "NULL"
            if ipv4 is None:
                ipv4 = "NULL" 
            if wifiSSID is None:
                wifiSSID = "NULL"
            
            print ("Interface: " + ifaceType)
            print ("Mac address: " + hwAddress)
            print ("IPv4 address: " + ipv4)
            print ("wifiSSID: " + wifiSSID)

            #cursor.execute("INSERT into Device (deviceID, interface, mac, ip, wifiSSID, relay) values ('" + deviceID + "', '"  + ifaceType + "', '" + hwAddress + "', '" + ipv4 + "', '" + wifiSSID + "', '" + str(relay) + "')")
            send_DBquery("INSERT ~", db_insert, False)

        print ("\nDB Update Completed..")
        query = {"error:": error, "id": deviceID, "relay": relay}
        query = json.dumps(query)
        client.publish("/configuration/join_ack/" + tempID, query)
        
        print ("\n ##Process Completed - Join\n")

    except Exception as e:
        error = 1
        print ("Join error: ", e)
        query = {"error:": error, "deviceID":tempID, "relay":relay}
        query = json.dumps(query)
        client.publish("/configuration/join_ack/" + tempID, query)


# Unjoin the target device
def leave(deviceID, payload):
    print ("\n\n ##Process - Leave\n")

    relay = payload.get('relay')
    try:
        error = 0
        print ("Device ID: " + deviceID)

        queryID = send_DBquery("DELETE ~", db_delete, False)
        db_payload = dbQuery_cache[queryID]
        db_error1 = db_payload.get('error')
        del dbQuery_cache[queryID]
    
        queryID = send_DBquery("DELETE ~", db_delete,  False)
        db_payload = dbQuery_cache[queryID]
        db_error2 = db_payload.get('error')
        del dbQuery_cache[queryID]
    
        queryID = send_DBquery("DELETE ~", db_delete, False)
        db_payload = dbQuery_cache[queryID]
        db_error3 = db_payload.get('error')
        del dbQuery_cache[queryID]

        del deviceID_cache[deviceID]
    
        print ("\nDB Update Completed..")
        query = {"error:": error, "relay":relay}
        query = json.dumps(query)
        client.publish("/configuration/leave_ack/" + deviceID, query)
        
        print ("\n ##Process Completed - Leave\n")


    except Exception as e:
        error = 1
        print ("Leave error: ", e)
        query = {"error:": error, "relay":relay}
        query = json.dumps(query)
        client.publish("/configuration/leave_ack/" + deviceID, query)
 
 

def register_genID(hash_val, flag):

    newID = hash_val + str(flag)
   
    print ("\nCheck ID collision...\n")
    queryID = send_DBquery("select ~", db_select, True)

    payload = dbQuery_cache[queryID]
    exist = payload.get('exist')

    del dbQuery_cache[queryID]

    if not exist:
        return newID
    else:
        return register_genID(hash_val, flag + collision_inc)



def register(deviceID, payload):
    print ("\n\n ##Process - Register\n")     
    print ("DeviceID: " + deviceID)

    relay = payload.get('relay')
    registerID = payload.get('registerID')
    try:
        error = 0
        
        # Check whether the device exists 
        if deviceID in deviceID_cache:
            exist = True
        else:
            queryID = send_DBquery("SELECT ~", db_select, True)
            db_payload = dbQuery_cache[queryID]
            exist = db_payload.get('exist')
            del dbQuery_cache[queryID]
            if not exist:
                raise Exception ('No Device Error')
            else:
                deviceID_cache[deviceID] = exist

        registerList = payload.get('registerList')

        idList = []
        for item in registerList:
            index = item.get('index')
            registerType = item.get('registerType')
            category = item.get('category')
            cache = item.get('cache')
            segment = item.get('segment')
            collisionAvoid = item.get('collisionAvoid')
            #TODO: split attributes
            attributes = item.get('attributes')

            # generate service/content ID
            hash_val = item.get('hash')
            flag = gen_flag(cache, segment, 0)
            
            if collisionAvoid:
                newID = register_genID(hash_val, flag)
            else:
                newID = hash_val + str(flag)
            
            temp = {index:newID}

            print ("\nGenerated ID of index " + index + ": " + newID)
            idList.append(temp)

            # DB Update
            send_DBquery("INSERT ~", db_insert, False)
            #payload = dbQuery_cache[queryID]
            #db_error = payload.get('error')
            #del dbQuery_cache[queryID]
        

        print ("\nDB Update Completed..")
        query = {"error:": error, "registerID": registerID, "idList": idList, "relay": relay}
        query = json.dumps(query)
        print(query)
        client.publish("/configuration/register_ack/" + deviceID, query)
        
        print ("\n ##Process Completed - Register\n")


    except Exception as e:
        error = 1
        print ("Register error: ", e)
        query = {"error": error, "registerID": registerID, "relay":relay}
        query = json.dumps(query)
        client.publish("/configuration/register_ack/" + deviceID, query)



def update(deviceID, payload):
    print ("\n\n ##Process - Update\n")     
    print ("DeviceID: " + deviceID)
 
    updateID = payload.get('updateID')
    relay = payload.get('relay')

    try:
        error = 0
        
        # Check whether the device exists 
        if deviceID in deviceID_cache:
            exist = True
        else:
            queryID = send_DBquery("SELECT ~", db_select, True)
            db_payload = dbQuery_cache[queryID]
            exist = db_payload.get('exist')
            del dbQuery_cache[queryID]
            if not exist:
                raise Exception ('No Device Error')
            else:
                deviceID_cache[deviceID] = exist

        #TODO: process deregister 
        deregister = payload.get('deregister')
        attributes = payload.get('attributes')

        # check if this content/service exists
        queryID = send_DBquery("SELECT ~", db_select, True)
        db_payload = dbQuery_cache[queryID]
        exist = db_payload.get('exist')
        del dbQuery_cache[queryID]
        if not exist:
            raise Exception ('No Content/Service Error')

        if deregister:
            send_DBquery("DELETE ~", db_delete, False)

        # DB Update
        send_DBquery("UPDATE ~", db_update, False)
        #payload = dbQuery_cache[queryID]
        #db_error = payload.get('error')
        #del dbQuery_cache[queryID]

        print ("\nDB Update Completed..")
        
        query = {"error:": error, "updateID": updateID, "relay": relay}
        query = json.dumps(query)
        client.publish("/configuration/update_ack/" + deviceID, query)
        
        print ("\n ##Process Completed - Update\n")


    except Exception as e:
        error = 1
        print ("Update error: ", e)
        query = {"error": error, "updateID": updateID, "relay":relay}
        query = json.dumps(query)
        client.publish("/configuration/update_ack/" + deviceID, query)



def query(deviceID, payload):
    print ("\n ##Process - Query\n")     
    print ("DeviceID: " + deviceID)
 
    queryID = payload.get('queryID')
    relay = payload.get('relay')

    try:
        error = 0
        
        # Check whether the device exists 
        if deviceID in deviceID_cache:
            exist = True
        else:
            queryID = send_DBquery("SELECT ~", db_select, True)
            db_payload = dbQuery_cache[queryID]
            exist = db_payload.get('exist')
            del dbQuery_cache[queryID]
            if not exist:
                raise Exception ('No Device Error')
            else:
                deviceID_cache[deviceID] = exist

        queryType = payload.get('queryType')
        category = payload.get('type')
        order = payload.get('order')
        desc = payload.get('desc')
        limit = payload.get('limit')
        qosRequirements = payload.get('qosRequirements')
        additionalFields = payload.get('additionalFields')

        for req in qosRequirements:
            metricType = req.get('metricType')
            metricUnit = req.get('metricUnit')
            metricValue = req.get('metricValue')
            metricOperator = req.get('metricOperator')

        print ("\nSearching " + queryType + "..")
        #TODO: Search content/service from DB
        ids = []

       
        reply = {"error:": error, "queryID": queryID, "desc": desc, "ids": ids, "relay": relay}
        reply = json.dumps(reply)
        client.publish("/utilization/reply/" + deviceID, reply)
        
        print ("\n ##Process Completed - Query\n")


    except Exception as e:
        error = 1
        print ("Query error: ", e)
        reply = {"error": error, "queryID": queryID, "relay":relay}
        reply = json.dumps(reply)
        client.publish("/utilization/reply/" + deviceID, reply)





def on_connect(client, userdata, flags, rc):
    print ("Connected with the Message Bus ")
    # communication with end-user
    client.subscribe("/configuration/join/#")
    client.subscribe("/configuration/leave/#")
    client.subscribe("/configuration/register/#")
    client.subscribe("/configuration/update/#")
    client.subscribe("/utilization/query/#")
    client.subscribe("/utilization/request/#")

def on_db_connect(client, userdata, flags, rc):
   # print ("Connected with the Message Bus ")
    # communication with DB
    client.subscribe("/DBQuery/flexMnger/")


def on_message(client, userdata, msg):
    print ("Subscribe - Topic: " + msg.topic)
    topic = msg.topic.split('/')
    payload = json.loads(msg.payload.decode('utf-8'))
    print(payload)
    if "configuration" == topic[1]:
        if "join" == topic[2]:
            deviceID = topic[3]
            join(deviceID, payload)
        elif "leave" == topic[2]:
            deviceID = topic[3]
            leave(deviceID, payload)
        elif "register" == topic[2]:
            deviceID = topic[3]
            register(deviceID, payload)
        elif "update" == topic[2]:
            deviceID = topic[3]
            update(deviceID, payload)
        else:
            print ("Message type error: " + msg.topic)
    
    elif "utilization" == topic[1]:
        if "query" == topic[2]:
            deviceID = topic[3]
            query(deviceID, payload)
        else:
            print ("Message type error: " + msg.topic)
    else:
        print ("Message type error: " + msg.topic)




def on_db_message(client, userdata, msg):
    topic = msg.topic.split('/')
    payload = json.loads(msg.payload.decode('utf-8'))
    #print (msg.payload)
    print ("Subscribe - Topic: " + msg.topic)
    queryID = payload.get('id')
    #print(queryID)
    dbQuery_cache[queryID] = payload
            



def on_publish(client, userdata, mid):
    print ("\n>> Publish a message\n")

def on_subscribe(client, userdata, mid, granted_qos):
    print ("\n<< Subscribe a message\n")

def on_db_publish(client, userdata, mid):
    print ("\n>> Publish a message to DB\n")
    print (userdata)
 
def on_db_subscribe(client, userdata, mid, granted_qos):
    print ("\n<< Subscribe a message from DB\n")


client = mqtt.Client()
db_client = mqtt.Client()


client.on_connect = on_connect
client.on_message = on_message
#client.on_subscribe = on_subscribe
#client.on_publish = on_publish
client.connect(broker ,1883, 60)

db_client.on_connect = on_db_connect
db_client.on_message = on_db_message
#db_client.on_subscribe = on_db_subscribe
db_client.on_publish = on_db_publish
db_client.connect(broker, 1883, 60)

if __name__ == "__main__":
    print("\n FlexID Manager is Running...")
    init_manager()
    while True:
        client.loop_start()
        db_client.loop_start()
