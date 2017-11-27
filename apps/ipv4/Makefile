server: proto_flex.h
	gcc -DDEBUG -I/usr/local -L/usr/local -o server server.c process_flex.c -lflex -lssl -lcrypto -ldl

client: proto_flex.h
	gcc -DDEBUG -I/usr/local -L/usr/local -o client client.c process_flex.c -lflex -lssl -lcrypto -ldl

test: proto_flex.h
	gcc -DDEBUG -o test_flex test_flex.c process_flex.c

clean:
	rm *.o server client test_flex
