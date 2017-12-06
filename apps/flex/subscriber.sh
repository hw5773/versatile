!/bin/sh

echo "This is the Test Application to conduct Flex ID Networking"
sleep 1
echo "We are going to retrieve the value of the fine dust in the building by
its Content ID"
sleep 1

echo "From the prior example, we get the Content ID by the Query/Reply message with Flex ID
Manager"
sleep 1
echo "Content ID for the fine dust in the building is:"
echo "80 40 f0 67 c1 cd 83 96 2a bf 35 99 de 79 c1 ab b7 7b 2f 02 6d"
sleep 1

/usr/local/bin/flex_subscriber10

sleep 1
echo "Because the cache bit is enabled in the Content ID, the data is cached in
the Cache"
sleep 1

/usr/local/bin/flex_subscriber15

sleep 1
echo "The result shows the data comes from the Cache"
