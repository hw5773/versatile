CFLAGS=-DDEBUG -I/usr/local/include
LDFLAGS=-L/usr/local/lib -lflex -lssl -lcrypto -ldl
BIN=publisher subscriber

all:
	gcc -o publisher publisher.c $(CFLAGS) $(LDFLAGS)
	gcc -o subscriber subscriber.c $(CFLAGS) $(LDFLAGS)

pub:
	gcc -o publisher publisher.c $(CFLAGS) $(LDFLAGS)

sub:
	gcc -o subscriber subscriber.c $(CFLAGS) $(LDFLAGS)

clean:
	rm $(BIN)
