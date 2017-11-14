server: proto_flex.h
	gcc -DDEBUG -o server server.c process_flex.c

client: proto_flex.h
	gcc -DDEBUG -o client client.c process_flex.c

test: proto_flex.h
	gcc -DDEBUG -o test_flex test_flex.c process_flex.c

clean:
	rm *.o server client test_flex
