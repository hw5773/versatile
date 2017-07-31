import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import json

def on_connect(client, userdata, rc):
	print ("Connected with result code " + str(rc))
	client.subscribe("hello/world")

def on_message(client, userdata, msg):
	print (msg.topic + " " + str(msg.payload))
	json_str = json.loads(msg.payload.decode('utf-8'))
	print (json_str)

	if "message" in json_str.keys():
		print (json_str["message"])

def on_publish(client, userdata, mid):
	print ("Client publishes the message")

def on_subscribe(client, userdata, mid, granted_qos):
	print ("Client subscribes the message")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.on_subscribe = on_subscribe

client.connect("test.mosquitto.org", 1883, 60)
client.loop_forever()
