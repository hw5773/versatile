import json
import os
import paho.mqtt.client as mqtt
import unicodedata
import re
import codecs

broker = "iot.eclipse.org"
select_db = "/dbquery/select/flexMnger/"
delete_db = "/dbquery/delete/flexMnger/"
update_db = "/dbquery/update/flexMnger/"
insert_db = "/dbquery/insert/flexMnger/"

# global variable for manage number in service ID
manageNum = 0
requenstNum = 0
deviceID_cache = {}

def check_deviceID(deviceID):
    
    while deviceID in deviceID_cache:
        rand_num = codecs.encode(os.urandom(1), 'hex_codec').decode()
        deviceID = deviceID + rand_num
    deviceID_cache[deviceID] = "None"

    query = '{"id":' + deviceID + '}'
    db_client.publish(select_db, query)

    # wait response from DB
    while True:
        if deviceID_cache[deviceID] != "None":
            break

    if deviceID_cache[deviceID] == "False":
        return deviceID
    else:
        return check_deviceID(deviceID)


# Register the device
def join(deviceID, payload):

    print ("\n ##Process - Join\n")
    #print (msg.topic + " " + str(msg.payload))
    
    origin_devID = deviceID
        
    try:
        error = 0
            
        # Collision check
        deviceID = check_deviceID(deviceID)
        print ("Device ID: " + deviceID)

        relay = payload.get('relay')
        if (relay is None) or (relay == "none"):
            relay = "none"
            print ("\nNeed relay? No")
        else:
            relay = int(relay)
            print ("\nNeed relay? Yes - Device: " + str(relay))
  
        neighbors = payload.get('neighbors')
        if neighbors is None:
            neighbors = "NULL"
       
        print ("\n----- Neighbor info.-----")
        for neighbor in neighbors:
            neighborIface = neighbor.get('neighborIface')
            neighborIpv4 = neighbor.get('neighborIpv4')
            neighborHwAddress = neighbor.get('neighborHwAddress')
            if (neighborIface is None) or (neighborIface == "none"):
                neighborIface = "NULL"
            if (neighborIpv4 is None) or (neighborIface == "none"):
                neighborIpv4 = "NULL"
            if (neighborHwAddress is None) or (neighborHwAddress == "none"):
                neighborHwAddress = "NULL"
            
            print ("neighborIface: " + neighborIface)
            print ("neighborIpv4: " + neighborIpv4)
            print ("neighborHwAddress: " + neighborHwAddress)
            
            #cursor.execute("INSERT into Neighbor (deviceID, neighborIface, neighborIpv4, neighborHwAddress) values ('" + deviceID + "', '" + neighborIface + "', '" + neighborIpv4 + "', '" + neighborHwAddress + "')")
            query = '{"id":' + deviceID + '}'
            db_client.publish(insert_db, query)

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
            query = '{"id":' + deviceID + '}'
            db_client.publish(insert_db, query)
            print ("\n")

        print ("\nDevice DB updated..")
        query = '{"error:":"0", "deviceID":' + deviceID + '}'
        client.publish("/configuration/jack/" + origin_devID, query)
        print ("\n ##Process Completed.\n")

    except Exception as e:
        error = 1
        print ("Join error: ", e)
        query = '{"error:":"1", "deviceID":' + deviceID + '}'
        client.publish("/configuration/jack/" + origin_devID, query)
 

#TODO: need check for cashable bit, segment bit
def genServiceID(service):
    global manageNum
    manageNum = manageNum + 1
    manageStr = "%04d" % manageNum
    if service == "Streaming":
        return "0" + manageStr + "0"
    elif service == "Web":
        return "1" + manageStr + "0"
    elif service == "Voice":
        return "1" + manageStr + "1"
    elif service == "FileTransfer":
        return "0" + manageStr + "1"
    elif service == "Game":
        return "0" + manageStr + "0"
    elif service == "BestEffort":
        return "0" + manageStr + "0"
    elif service == "CPU":
        return "0" + manageStr + "0"
    elif service == "Bandwidth":
        return "0" + manageStr + "0"
    elif service == "Disk":
        return "1" + manageStr + "1"
    elif service == "Temperature":
        return "0" + manageStr + "0"
    elif service == "ImageTransfer":
        return "1" + manageSTr + "1"
    else:
        return 1

def genContentID(content):
    global manageNum
    manageNum = manageNum + 1
    manageStr = "%04d" % manageNum
    if content == "Video":
        return "1" + manageStr + "1"
    elif content == "Photo":
        return "1" + manageStr + "0"
    elif content == "Document":
        return "1" + manageStr + "0"
    else:
        return 1
   

def register_service(payload):
    print ("\n ##Process - Register Service\n")     
    deviceID = payload.get('deviceID')
    print ("Device ID: " + deviceID)
    serviceID = "none"
 
    try:
        error = 0
        #exist = cursor.execute ("SELECT dummyID from Device where deviceID='" + deviceID +"'")
       
        deviceID_cache[deviceID] = "True"
        # Check whether the device exists 
        if deviceID in deviceID_cache:
            exist = 1
        else:
            exist = 0

        if exist == 0:
            error = 1
            print ("This device is not registered")
            query = '{"error:":"1", "deviceID":' + deviceID + '}'
            client.publish("/configuration/rack/" + deviceID, query)
            return

        serviceType = payload.get('serviceType')
      
        attributes = payload.get('attributes')
        serviceID = genServiceID(serviceType)
        relay = payload.get('relay')
        
        query = '{"id":' + deviceID + '}'
        client.publish(insert_db, query)

        '''
        #cursor.execute("SELECT count(*) from information_schema.columns where table_name = 'Service'")
        # The number of attributes column in the current Service table
        #attrCount = cursor.fetchall()[0][0] - 4;
        for service in serviceList:
            serviceType = service.get('serviceType')
            # Generate serviceID according to serviceType - Streaming, Web, Voice, FileTransfer, GAme, BestEffor
            print ("\nGenerate Service ID...")
            serviceID = genServiceID(serviceType)
            attributes = service.get('attributes')
            if len(attributes) > attrCount:
                for i in range(len(attributes) - attrCount):
                    attrNum = "attr"+str(attrCount+1+i)
                    #cursor.execute("ALTER table Service add "+ attrNum + " varchar(50) default '.'")
            attrIdx = ''
            attrStr = ''
            for i in range(len(attributes)):
                attrIdx = attrIdx + ", attr" + str(i+1)
                attrStr = attrStr + ", '" + attributes[i] + "'"
            
            print ("Service ID: " + serviceID)
            print ("Service Type: " + serviceType)

            #cursor.execute("INSERT into Service (serviceID, deviceID, serviceType"+ attrIdx + ") values ('" + str(serviceID) + "', '" + deviceID + "', '" + serviceType + "'" + attrStr + ")")
        '''     

        print ("\nService DB updated..")

        query = '{"error:":"0", "idList":' + '[' + serviceID + ']}'
        client.publish("/configuration/jack/" + deviceID, query)
 
        print ("\n ##Process Completed.\n")

    except Exception as e:
        error = 1
        print ("Register error: ", e)
        query = '{"error:":"1", "deviceID":' + deviceID + '}'
        client.publish("/configuration/rack/" + deviceID, query)
 

def register_content(payload):
    print ("\n ##Process - Register Content\n")     
    deviceID = payload.get('deviceID')
    print ("Device ID: " + deviceID)
    serviceID = "none"
 
    try:
        error = 0
        #exist = cursor.execute ("SELECT dummyID from Device where deviceID='" + deviceID +"'")
       
        deviceID_cache[deviceID] = "True"
        # Check whether the device exists 
        if deviceID in deviceID_cache:
            exist = 1
        else:
            exist = 0

        if exist == 0:
            error = 1
            print ("This device is not registered")
            query = '{"error:":"1", "deviceID":' + deviceID + '}'
            client.publish("/configuration/rack/" + deviceID, query)
            return

        contentType = payload.get('contentType')
        contentName = payload.get('contentName')
        serviceID = genContentID(contentType)
        relay = payload.get('relay')
        
        query = '{"deviceID":' + deviceID + '}'
        client.publish(insert_db, query)

        '''
        #cursor.execute("SELECT count(*) from information_schema.columns where table_name = 'Service'")
        # The number of attributes column in the current Service table
        #attrCount = cursor.fetchall()[0][0] - 4;
        for service in serviceList:
            serviceType = service.get('serviceType')
            # Generate serviceID according to serviceType - Streaming, Web, Voice, FileTransfer, GAme, BestEffor
            print ("\nGenerate Service ID...")
            serviceID = genServiceID(serviceType)
            attributes = service.get('attributes')
            if len(attributes) > attrCount:
                for i in range(len(attributes) - attrCount):
                    attrNum = "attr"+str(attrCount+1+i)
                    #cursor.execute("ALTER table Service add "+ attrNum + " varchar(50) default '.'")
            attrIdx = ''
            attrStr = ''
            for i in range(len(attributes)):
                attrIdx = attrIdx + ", attr" + str(i+1)
                attrStr = attrStr + ", '" + attributes[i] + "'"
            
            print ("Service ID: " + serviceID)
            print ("Service Type: " + serviceType)

            #cursor.execute("INSERT into Service (serviceID, deviceID, serviceType"+ attrIdx + ") values ('" + str(serviceID) + "', '" + deviceID + "', '" + serviceType + "'" + attrStr + ")")
        '''     

        print ("\nService DB updated..")

        query = '{"error:":"0", "idList":' + '[' + serviceID + ']}'
        client.publish("/configuration/jack/" + deviceID, query)
 
        print ("\n ##Process Completed.\n")

    except Exception as e:
        error = 1
        print ("Register error: ", e)
        query = '{"error:":"1", "deviceID":' + serviceID + '}'
        client.publish("/configuration/rack/" + deviceID, query)
 

# Unregister the target device
def leave(deviceID):
    print "\n ##Process - Leave\n"
    try:
        
        print ("Device ID: " + deviceID)

        #cursor.execute("DELETE from Device where deviceID='" + deviceID + "'")
        #cursor.execute("DELETE from Service where deviceID='" + deviceID + "'")
        #cursor.execute("DELETE from Neighbor where deviceID='" + deviceID + "'")

        query = '{"deviceID":' + deviceID + '}'
        client.publish(delete_db, query)

        print ("\n ##Process Completed.\n")

        query = '{"error:":"0"}'
        client.publish("/configuration/lack/" + deviceID, query)
 

    except Exception as e:
        print ("Leave error: ", e)
        query = '{"error:":"1"}'
        client.publish("/configuration/lack/" + deviceID, query)
 

# Service request
def request_service(payload):
    global requestNum
    print ("\n ##Process - Handling Request from the Client\n")
    try:
        #cursor.execute("SELECT MAX(CAST(requestID as  SIGNED)) From Request")

        #maxID = cursor.fetchall()[0][0]
        #if maxID is not None:
        #    requestID = str(int(maxID) + 1) # Generate new request ID
        #else:
        #    requestID = "1"

        requestNum = requestNum + 1
        requestID = str(requst)
        print ("Generate Request ID...")
        print ("Request ID: " + requestID)
    
        # Temporary service ID, TODO: should modify this
        serviceID = "temp"

        deviceID = payload.get('deviceID')
        serviceType = payload.get('serviceType')
        qosRequirements = payload.get('qosRequirements')
        keywords = payload.get('keywords')

        print ("Device ID: " + deviceID)
        print ("Service Type: " + serviceType)
        print ("Keywords: " + keywords)
        
        # Insert new requestID to Request table
        #cursor.execute("INSERT into Request (requestID, deviceID, serviceID) values ('" + requestID + "', '" + deviceID + "', '" + serviceID + "')")
        
        query = '{"id":' + deviceID + '}'
        db_client.publish(insert_db, query)


        # Insert qosRequirements to QosRequirement table
        for requirement in qosRequirements:
            metricName = requirement.get('metricName')
            metricUnit = requirement.get('metricUnit')
            metricValue = requirement.get('metricValue')
            metricOperator = requirement.get('metricOperator')
            #cursor.execute("INSERT into QosRequirement (requestID, metricName, metricUnit, metricValue, metricOperator) values ('" + requestID + "', '" + metricName + "', '" + metricUnit + "', '" + metricValue + "', '" + metricOperator + "')")
            query = '{"id":' + deviceID + '}'
            db_client.publish(insert_db, query)


        print ("\nRequest DB updated..")

        print ("\n ##Process Completed.\n")

        
        # Call Service request function
        #matchingResult = ServiceRequest(requestID);
       

        # test data
        if serviceType == 'Streaming':
            matchingResult = {"Matching":[{"NextDev":"","NextInterface":"","PrevDev":"001","PrevInterface":"WiFi","deviceID":"002","serviceType":"streaming"},{"NextDev":"002","NextInterface":"WiFi","PrevDev":"","PrevInterface":"","deviceID":"001","serviceType":"streaming"}],"requestID":requestID}
        else: 
            matchingResult = {"Matching":[{"NextDev":"","NextInterface":"","PrevDev":"002","PrevInterface":"BT","deviceID":"003","serviceType":"ImageTransfer"},{"NextDev":"003","NextInterface":"BT","PrevDev":"001","PrevInterface":"WiFi","deviceID":"002","serviceType":"ImageTransfer"},{"NextDev":"002","NextInterface":"WiFi","PrevDev":"","PrevInterface":"","deviceID":"001","serviceType":"ImageTransfer"}],"requestID":requestID}

        # send Matching result to the all device
        matching = matchingResult.get('Matching')
       
        print ("\n ##Process - Send Matching request to the Client\n")
        for result in matching:
            print ("Send Matching Request..")

        print ("\n ##Process Completed.\n")


    except Exception as e:
        print ("Request error: ", e)


def query_service(payload):
    print ("\n ##Process - Handling Request from the Client\n")
    try:
        #cursor.execute("SELECT MAX(CAST(requestID as  SIGNED)) From Request")

        #maxID = cursor.fetchall()[0][0]
        #if maxID is not None:
        #    requestID = str(int(maxID) + 1) # Generate new request ID
        #else:
        #    requestID = "1"

    

        deviceID = payload.get('deviceID')
        serviceType = payload.get('serviceType')
        qosRequirements = payload.get('qosRequirements')
        keywords = payload.get('keywords')

        print ("Device ID: " + deviceID)
        print ("Service Type: " + serviceType)
        print ("Keywords: " + keywords)
        
        # Insert qosRequirements to QosRequirement table
        for requirement in qosRequirements:
            metricName = requirement.get('metricName')
            metricUnit = requirement.get('metricUnit')
            metricValue = requirement.get('metricValue')
            metricOperator = requirement.get('metricOperator')
            #cursor.execute("INSERT into QosRequirement (requestID, metricName, metricUnit, metricValue, metricOperator) values ('" + requestID + "', '" + metricName + "', '" + metricUnit + "', '" + metricValue + "', '" + metricOperator + "')")

        relay = payload.get('relay')
        metric = payload.get('metric')
        order = payload.get('order')
        limit = payload.get('limit')

        print ("\n ##Process Completed.\n")
       

        print ("\n ##Process Completed.\n")


    except Exception as e:
        print ("Request error: ", e)


'''


# Send service request to the PoolingGW
def ServiceRequest(requestID):
    print "\n ##Process - Send Service request to the Pooling GW\n"
    try:
        res = requests.post('http://143.248.56.7:3334/PoolingGW/Request', json=data)
        requestID = {"requsetID":requestID}

        print "Send request info. to the PoolingGW..\n"
        # send requestID to the PoolingGW
        res = requests.post('http://147.46.216.250:3334/Matching', json=requestID)
        
        # return value
        jsonDict = res.json()
        print "Receive Matching Result.\n"
        
        #print jsonDict
        #requestID = int(jsonDict.get('requestID'))
        #requestID = str(requestID)
        #print "ID: " +requestID

        print "\n ##Process Completed.\n"
        return jsonDict


    except Exception as e:
        print "ServiceRequest error: ", e
        return str(e)

'''

def on_connect(client, userdata, flags, rc):
    print ("Connected with the Broker " + str(rc))
    # communication with end-user
    client.subscribe("/configuration/join/#")
    client.subscribe("/configuration/leave/#")
    client.subscribe("/configuration/register/#")
    client.subscribe("/configuration/update/#")
    client.subscribe("/utilization/query/#")
    client.subscribe("/utilization/request/#")

def on_db_connect(client, userdata, flags, rc):
    print ("DB! Connected with the Broker " + str(rc))
    # communication with DB
    client.subscribe("/dbquery/insert_ack/flexMnger/#")
    client.subscribe("/dbquery/select_ack/flexMnger/#")
    client.subscribe("/dbquery/update_ack/flexMnger/#")
    client.subscribe("/dbquery/delete_ack/flexMnger/#")



def on_message(client, userdata, msg):
    topic = msg.topic.split('/')
    payload = json.loads(msg.payload.decode('utf-8'))
    print(payload)
    if "configuration" == topic[1]:
        if "join" == topic[2]:
            deviceID = topic[3]
            join(deviceID, payload)
        elif "register" == topic[2]:
            if "service" == topic[3]:
                register_service(payload)
            elif "content" == topic[3]:
                register_content(payload)
            else:
                print("Register type error!")
        elif "leave" == topic[2]:
            deviceID = topic[3]
            leave(deviceID)
    
    elif "utilization" == topic[1]:
        if "request" == topic[2]:
            if "service" == topic[3]:
                request_service(payload)
            elif "content" == topic[3]:
                request_content(payload)
            else:
                pritn("Request type error!")
        if "query" == topic[2]:
            if "service" == topic[3]:
                query_service(payload)



def on_db_message(client, userdata, msg):
    topic = msg.topic.split('/')
    payload = json.loads(msg.payload.decode('utf-8'))
    print("DB on_message")
    print (msg.payload)
    if "select_ack" == topic[2]:
        if 'exist' in payload:
            deviceID_cache[payload.get('deviceID')] = payload.get('exist')
            



def on_publish(client, userdata, mid):
    print ("\nFlexID Manager: publishes the message\n")

def on_subscribe(client, userdata, mid, granted_qos):
    print ("\nFlexID Manager: subscribes the message\n")

def on_db_publish(client, userdata, mid):
    print ("\nDB! FlexID Manager: publishes the message\n")

def on_db_subscribe(client, userdata, mid, granted_qos):
    print ("\nDB! FlexID Manager: subscribes the message\n")


client = mqtt.Client()
db_client = mqtt.Client()


client.on_connect = on_connect
client.on_message = on_message
client.on_subscribe = on_subscribe
client.on_publish = on_publish
client.connect(broker ,1883, 60)

db_client.on_connect = on_db_connect
db_client.on_message = on_db_message
db_client.on_subscribe = on_db_subscribe
db_client.on_publish = on_db_publish
db_client.connect(broker, 1883, 60)

if __name__ == "__main__":
    while True:
        client.loop_start()
        db_client.loop_start()
