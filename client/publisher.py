import paho.mqtt.client as mqtt
import time

broker = "202.30.19.96"

client = mqtt.Client("python_pub")
client.connect(broker, 1333)
#client.publish("hello/snu/test", '{"id": "snu", "message": "Mbox, Hello World!"}')
#client.publish("hello/snu/aaa", '{"id": "snu", "message": "22 Mbox, Hello World!"}')

### Join

client.publish('/configuration/join/a32adf12345678', '{"uniqueCodes":[{"ifaceType":"wifi","hwAddress":"00-1a-e9-8d-08-73","ipv4":"143.248.30.13","wifiSSID":"Welcome_KAIST"},{"ifaceType":"lte","hwAddress":"00:1a:e9:8d:08:74","ipv4":"10.0.3.15"}],"relay":["fh2gj1g", "d3hsv5a35"],"neighbors":[{"neighborIface":"wifi", "neighborIpv4":"10.0.0.42", "neighborFlexID":"asdf"}, {"neighborIface":"bluetooth", "neighborHwAddress":"00:11:22:33:aa:bb", "neighborFlexID":"asdf12"}], "pubKey":"a32adf"}')

#time.sleep(5)

#client.publish('/dbquery/sack/flexMnger/0x001', '{"error":0, "data":"[test]"}')

#time.sleep(1)

#client.publish('/dbquery/sack/flexMnger/0x001', '{"error":0, "data":""}')

#time.sleep(1)

#client.publish('/dbquery/iack/flexMnger/0x001', '{"error":0}')

#time.sleep(1)

#client.publish('/dbquery/iack/flexMnger/0x001', '{"error":0}')

#time.sleep(5)



### Leave
#client.publish('/configuration/leave/asdfgwef', '{"relay":["d3hsv5a350"]}')

#time.sleep(3)

#client.publish('/dbquery/dack/flexMnger/0x001', '{"error":0, "data":""}')

#time.sleep(3)

#client.publish('/dbquery/dack/flexMnger/0x001', '{"error":0, "data":""}')

#time.sleep(3)

#client.publish('/dbquery/dack/flexMnger/0x001', '{"error":0, "data":""}')



### Register

#client.publish('/configuration/register/afgw123gf', '{"registerID":"asdf1230", "registerList":[{"index":"0", "hash":"a84ljkfh", "registerType":"Content", "category":"Video", "attributes":["resloution=720p", "size=50123B"], "cache":true, "segment":true}, {"index":"1", "hash":"d3nbh5", "registerType":"Service", "category":"Web", "attributes":["bandwidth=10Mbps"], "cache":false, "segment":false, "collisionAvoid":true}], "relay":["fh2gj1g", "d3hsv5a350"]}')

#time.sleep(3)

#client.publish('/dbquery/sack/flexMnger/0x001', '{"error":0, "data":"asdf"}')

#time.sleep(3)

#client.publish('/dbquery/sack/flexMnger/0x001', '{"error":0, "data":"aa"}')

#time.sleep(3)

#client.publish('/dbquery/sack/flexMnger/0x001', '{"error":0, "data":""}')

#time.sleep(5)

#client.publish('/dbquery/iack/flexMnger/0x001', '{"error":0}')


### Update

#client.publish('/configuration/update/afgw123gf', '{"updateID":"asi39s", "id":"asdfg1235s", "deregister":false, "attributes":["resloution=1080p", "size=701123B"], "relay":["fh2gj1g", "d3hsv5a35"]}')

#time.sleep(4)

#client.publish('/dbquery/sack/flexMnger/0x001', '{"error":0, "data":"test"}')

#time.sleep(4)

#client.publish('/dbquery/sack/flexMnger/0x001', '{"error":0, "data":"test"}')

#time.sleep(4)

#client.publish('/dbquery/iack/flexMnger/0x001', '{"error":0}')



### Query

#client.publish('/utilization/query/fk3gfb77', '{"deviceID":"fk3gfb77", "queryType":"Content", "category":"Any", "relay":["fh2gj1g", "d3hsv5a35"], "order":"resolution", "desc":true, "limit":3, "qosRequirements":[{"metricName":"bandwidth", "metricUnit":"mbps", "metricValue":"5", "metricOperator":"gt"}, {"metricName":"delay", "metricUnit":"ms", "metricValue":"100", "metricOperator":"lt"}], "additionalFields":[]}')

#time.sleep(3)

#client.publish('/dbquery/sack/flexMnger/0x001', '{"data":["gwef"]}')

#time.sleep(3)

#client.publish('/dbquery/sacl/flexMnger/0x001', )

client.loop(2)
