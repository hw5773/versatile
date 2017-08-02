from flask import Flask, request, session, g, redirect, url_for, abort, render_template, flash, jsonify
from flaskext.mysql import MySQL
import json

app = Flask(__name__)
mysql = MySQL()

app.config['MYSQL_DATABASE_USER'] = 'root'
app.config['MYSQL_DATABASE_PASSWORD'] = 'mmlab'
app.config['MYSQL_DATABASE_DB'] = 'diversity'
app.config['MYSQL_DATABASE_HOST'] = 'localhost'
mysql.init_app(app)

# global variable for manage number in service ID
manageNum = 0

@app.route("/")
def hello():
    return "Versatile Test"


# Register the device
@app.route("/Join", methods=["POST"])
def Join():
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
       
        jsonDict = request.json
        relay = jsonDict.get('relay')
        if relay is None:
            relay = "none"
        else:
            relay = int(relay)

        uniqueCodes = jsonDict.get('uniqueCodes')
        for data in uniqueCodes:
            ifaceType = data.get('ifaceType')
            hwAddress = data.get('hwAddress')
            ipv4 = data.get('ipv4')
            wifiSSID = data.get('wifiSSID')
            if ifaceType is None:
                ifaceType = "."
            if hwAddress is None:
                ifaceType = "."
            if ipv4 is None:
                ipv4 = "."
            if wifiSSID is None:
                wifiSSID = "."
            cursor.execute("INSERT into Device (deviceID, interface, mac, ip, wifiSSID, relay) values ('" + deviceID + "', '"  + ifaceType + "', '" + hwAddress + "', '" + ipv4 + "', '" + wifiSSID + "', '" + str(relay) + "')")
        conn.commit()
        return json.dumps({"error":error, "id":deviceID})

    except Exception as e:
        error = 1
        return json.dumps({"error":error, "id":deviceID})

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
    else:
        return 1



@app.route("/Update", methods=["POST"])
def Update():
    try:
        error = 0
        conn = mysql.connect()
        cursor = conn.cursor()

        jsonDict = request.json
        deviceID = int(jsonDict.get('deviceID'))
        deviceID = str(deviceID)
        exist = cursor.execute ("SELECT dummyID from Device where deviceID='" + deviceID +"'")
        # Check whether the device exists
        if exist <= 0:
            error = 1
            return json.dumps({"error":error})
      
        serviceList = jsonDict.get('services')
      
        cursor.execute("SELECT count(*) from information_schema.columns where table_name = 'Service'")
        # The number of attributes column in the current Service table
        attrCount = cursor.fetchall()[0][0] - 4;
        for service in serviceList:
            serviceType = service.get('serviceType')
            # Generate serviceID according to serviceType - Streaming, Web, Voice, FileTransfer, GAme, BestEffor
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

            cursor.execute("INSERT into Service (serviceID, deviceID, serviceType"+ attrIdx + ") values ('" + str(serviceID) + "', '" + deviceID + "', '" + serviceType + "'" + attrStr + ")")
                
        conn.commit()
        return json.dumps({"error":error})

    except Exception as e:
        error = 1
        return json.dumps({"error":error})



# Unregister the target device
@app.route("/Leave", methods=["POST"])
def Leave():
    try:
        error = 0
        jsonDict = request.json
        deviceID = int(jsonDict.get('deviceID'))
        deviceID = str(deviceID)
        conn = mysql.connect()
        cursor = conn.cursor()
        cursor.execute("DELETE from Device where deviceID='" + deviceID + "'")
        cursor.execute("DELETE from Service where deviceID='" + deviceID + "'")
        conn.commit()
        return json.dumps({"error":error})

    except Exception as e:
        error = 1
        return json.dumps({"error":error})


# Service request
@app.route("/Request", methods=["POST"])
def Request():
    try:
        conn = mysql.connect()
        cursor = conn.cursor()
        cursor.execute("SELECT MAX(requestID) From Request") # Find a maximum request ID value

        maxID = cursor.fetchall()[0][0]
        if maxID is not None:
            requestID = str(int(maxID) + 1) # Generate new request ID
        else:
            requestID = "1"

        # Temporary service ID, TODO: should modify this
        cursor.execute("SELECT MAX(serviceID) from Service")
        serviceID = cursor.fetchall()[0][0]
        if serviceID is not None:
            print serviceID
        else:
            raise ValueError('Service is not Available')

        jsonDict = request.json
        deviceID = jsonDict.get('deviceID')
        serviceType = jsonDict.get('serviceType')
        qosRequirements = jsonDict.get('qosRequirements')
        keywords = jsonDict.get('keywords')
        additionalFields = jsonDict.get('additionalFields')

        # Insert new requestID to Request table
        cursor.execute("INSERT into Request (requestID, deviceID, serviceID) values ('" + requestID + "', '" + deviceID + "', '" + serviceID + "')")

        # Insert qosRequirements to QosRequirement table
        for requirement in qosRequirements:
            metricName = requirement.get('metricName')
            metricUnit = requirement.get('metricUnit')
            metricValue = requirement.get('metricValue')
            metricOperator = requirement.get('metricOperator')
            cursor.execute("INSERT into QosRequirement (requestID, metricName, metricUnit, metricValue, metricOperator) values ('" + requestID + "', '" + metricName + "', '" + metricUnit + "', '" + metricValue + "', '" + metricOperator + "')")

        conn.commit()
        return "Good"


    except Exception as e:
        error = 1
        return str(e)



if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0', port=3334)
