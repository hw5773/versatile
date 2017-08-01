from flask import Flask, request, session, g, redirect, url_for, abort, render_template, flash, jsonify
from flaskext.mysql import MySQL
import json
import os
from flask_socketio import SocketIO, emit
import unicodedata

app = Flask(__name__)
mysql = MySQL()

app.config['MYSQL_DATABASE_USER'] = 'root'
app.config['MYSQL_DATABASE_PASSWORD'] = 'mmlab'
#app.config['MYSQL_DATABASE_USER'] = 'FIFuser1'
#app.config['MYSQL_DATABASE_PASSWORD'] = 'FIFuser!'
app.config['MYSQL_DATABASE_DB'] = 'diversity'
#app.config['MYSQL_DATABASE_HOST'] = 'localhost'
#app.config['MYSQL_DATABASE_HOST'] = '202.30.19.234'
#app.config['MYSQL_DATABASE_PORT'] = 13306
mysql.init_app(app)

app.secret_key = "diversity_secret"
socketio = SocketIO(app)

# global variable for manage number in service ID
manageNum = 0


@app.route("/")
def index():
    return "Versatile Test"


# Register the device
@socketio.on('Join')
def Join(js):
    print "\n ##Process - Join\n"
    content = json.loads(js)
    deviceID = "-1"
    try:
        error = 0
        conn = mysql.connect()
        cursor = conn.cursor()
        cursor.execute("SELECT MAX(CAST(deviceID as  SIGNED)) From Device")

        maxID = cursor.fetchall()[0][0]
        if maxID is not None:
            deviceID = str(int(maxID) + 1) # Generate new device ID
        else:
            deviceID = "1"

        print "Generated Device ID: " + deviceID

        # socket init
        if 'session' in session and 'user-id' in session:
            pass
        else:
            session['session'] = os.urandom(24)
            session['username'] = deviceID
        #print "Username:" + session['username']
        #print "Session in Join: ", session

        relay = content.get('relay')
        if (relay is None) or (relay == "none"):
            relay = "none"
            print "\nNeed relay? No"
        else:
            relay = int(relay)
            print "\nNeed relay? Yes - Device: " + str(relay)

        neighbors = content.get('neighbors')
        if neighbors is None:
            neighbors = "NULL"
       
        print "\n----- Neighbor info.-----"
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
            
            print "neighborIface: " + neighborIface
            print "neighborIpv4: " + neighborIpv4
            print "neighborHwAddress: " + neighborHwAddress
            
            cursor.execute("INSERT into Neighbor (deviceID, neighborIface, neighborIpv4, neighborHwAddress) values ('" + deviceID + "', '" + neighborIface + "', '" + neighborIpv4 + "', '" + neighborHwAddress + "')")

        print "\n-----Device Info.-----"
        uniqueCodes = content.get('uniqueCodes')
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
            
            print "Interface: " + ifaceType
            print "Mac address: " + hwAddress
            print "IPv4 address: " + ipv4
            print "wifiSSID: " + wifiSSID

            cursor.execute("INSERT into Device (deviceID, interface, mac, ip, wifiSSID, relay) values ('" + deviceID + "', '"  + ifaceType + "', '" + hwAddress + "', '" + ipv4 + "', '" + wifiSSID + "', '" + str(relay) + "')")
            print "\n"
        conn.commit()

        print "\nDevice DB updated.."
       
        print "\n ##Process Completed.\n"


        emit('Jack', {"error":error, "id":deviceID})

    except Exception as e:
        error = 1
        print "Join error: ", e
        emit('Jack', {"error":error, "id":deviceID})


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



@socketio.on('Update')
def Update(js):
    content = json.loads(js)
    print "\n ##Process - Update\n"
    try:
        error = 0
        conn = mysql.connect()
        cursor = conn.cursor()

        deviceID = int(content.get('deviceID'))
        deviceID = str(deviceID)
        exist = cursor.execute ("SELECT dummyID from Device where deviceID='" + deviceID +"'")
        # Check whether the device exists
        if exist <= 0:
            error = 1
            print "This device is not registered\n"
            return {"error":error}

        print "Device ID: " + deviceID
      
        serviceList = content.get('services')
      
        cursor.execute("SELECT count(*) from information_schema.columns where table_name = 'Service'")
        # The number of attributes column in the current Service table
        attrCount = cursor.fetchall()[0][0] - 4;
        for service in serviceList:
            serviceType = service.get('serviceType')
            # Generate serviceID according to serviceType - Streaming, Web, Voice, FileTransfer, GAme, BestEffor
            print "\nGenerate Service ID..."
            serviceID = genServiceID(serviceType)
            attributes = service.get('attributes')
            if len(attributes) > attrCount:
                for i in range(len(attributes) - attrCount):
                    attrNum = "attr"+str(attrCount+1+i)
                    cursor.execute("ALTER table Service add "+ attrNum + " varchar(50) default '.'")
            attrIdx = ''
            attrStr = ''
            for i in range(len(attributes)):
                attrIdx = attrIdx + ", attr" + str(i+1)
                attrStr = attrStr + ", '" + attributes[i] + "'"
            
            print "Service ID: " + serviceID
            print "Service Type: " + serviceType

            cursor.execute("INSERT into Service (serviceID, deviceID, serviceType"+ attrIdx + ") values ('" + str(serviceID) + "', '" + deviceID + "', '" + serviceType + "'" + attrStr + ")")
                
        conn.commit()

        print "\nService DB updated.."

        print "\n ##Process Completed.\n"
        emit('Uack', {"error":error})

    except Exception as e:
        error = 1
        print "Update error: ", e 
        emit('Uack', {"error":error})



# Unregister the target device
@socketio.on('Leave')
def Leave(js):
    print "\n ##Process - Leave\n"
    content = json.loads(js)
    try:
        error = 0
        deviceID = int(content.get('deviceID'))
        deviceID = str(deviceID)

        print "Device ID: " + deviceID

        conn = mysql.connect()
        cursor = conn.cursor()
        cursor.execute("DELETE from Device where deviceID='" + deviceID + "'")
        cursor.execute("DELETE from Service where deviceID='" + deviceID + "'")
        cursor.execute("DELETE from Neighbor where deviceID='" + deviceID + "'")
        conn.commit()

        print "\n ##Process Completed.\n"

        emit('Lack', {"error":error})

    except Exception as e:
        error = 1
        print "Leave error: ", e
        emit('Lack', {"error":error})


# Service request
@socketio.on('Request')
def Request(js):
    print "\n ##Process - Handling Request from the Client\n"
    content = json.loads(js)
    try:
        conn = mysql.connect()
        cursor = conn.cursor()
        cursor.execute("SELECT MAX(CAST(requestID as  SIGNED)) From Request")

        maxID = cursor.fetchall()[0][0]
        if maxID is not None:
            requestID = str(int(maxID) + 1) # Generate new request ID
        else:
            requestID = "1"

        print "Generate Request ID..."
        print "Request ID: " + requestID
    
        # Temporary service ID, TODO: should modify this
        cursor.execute("SELECT MAX(serviceID) from Service")
        serviceID = cursor.fetchall()[0][0]
        if serviceID is None:
            raise ValueError('Service is not Available')

        deviceID = content.get('deviceID')
        serviceType = content.get('serviceType')
        qosRequirements = content.get('qosRequirements')
        keywords = content.get('keywords')

        print "Device ID: " + deviceID
        print "Service Type: " + serviceType
        print "Keywords: " + keywords
        
        # Insert new requestID to Request table
        cursor.execute("INSERT into Request (requestID, deviceID, serviceID) values ('" + requestID + "', '" + deviceID + "', '" + serviceID + "')")

        #print qosRequirements

        # Insert qosRequirements to QosRequirement table
        for requirement in qosRequirements:
            metricName = requirement.get('metricName')
            metricUnit = requirement.get('metricUnit')
            metricValue = requirement.get('metricValue')
            metricOperator = requirement.get('metricOperator')
            cursor.execute("INSERT into QosRequirement (requestID, metricName, metricUnit, metricValue, metricOperator) values ('" + requestID + "', '" + metricName + "', '" + metricUnit + "', '" + metricValue + "', '" + metricOperator + "')")

        print "\nRequest DB updated.."

        print "\n ##Process Completed.\n"

        conn.commit()
        
        
        # Call Service request function
        #matchingResult = ServiceRequest(requestID);
        TestServiceRequest()
       

        # test data
        if serviceType == 'Streaming':
            matchingResult = {"Matching":[{"NextDev":"","NextInterface":"","PrevDev":"001","PrevInterface":"WiFi","deviceID":"002","serviceType":"streaming"},{"NextDev":"002","NextInterface":"WiFi","PrevDev":"","PrevInterface":"","deviceID":"001","serviceType":"streaming"}],"requestID":requestID}
        else: 
            matchingResult = {"Matching":[{"NextDev":"","NextInterface":"","PrevDev":"002","PrevInterface":"BT","deviceID":"003","serviceType":"ImageTransfer"},{"NextDev":"003","NextInterface":"BT","PrevDev":"001","PrevInterface":"WiFi","deviceID":"002","serviceType":"ImageTransfer"},{"NextDev":"002","NextInterface":"WiFi","PrevDev":"","PrevInterface":"","deviceID":"001","serviceType":"ImageTransfer"}],"requestID":requestID}

        # send Matching result to the all device
        matching = matchingResult.get('Matching')
       
        print "\n ##Process - Send Matching request to the Client\n"
        for result in matching:
            print "Send Matching Request.."
            emit('MatchingRequest', result, broadcast=True)

        # send ack
        emit ('Rack', {"error": 0})

        print "\n ##Process Completed.\n"



    except Exception as e:
        print "Request error: ", e
        emit ('Rack', {"error": 1})


def TestServiceRequest():
    print "\n ##Process - Send Service request to the Pooling GW\n"

    print "Send request info. to the Pooling GW..\n"

    print "Receive Matching Result.\n"

    print "\n ##Process Completed.\n"


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





if __name__ == "__main__":
    socketio.run(app, host='0.0.0.0', port=3333, debug=True)
    #app.run(debug=True, host='0.0.0.0', port=3334)
