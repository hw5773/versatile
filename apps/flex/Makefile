CFLAGS=-DDEBUG -I/usr/local/include
LDFLAGS=-L/usr/local/lib -lflex -lssl -lcrypto -ldl
BIN=publisher subscriber subscriber10 subscriber15 publisher9

all:
	gcc -o publisher publisher.c $(CFLAGS) $(LDFLAGS)
	gcc -o publisher9 publisher9.c $(CFLAGS) $(LDFLAGS)
	gcc -o subscriber subscriber.c $(CFLAGS) $(LDFLAGS)
	gcc -o subscriber10 subscriber10.c $(CFLAGS) $(LDFLAGS)
	gcc -o subscriber15 subscriber15.c $(CFLAGS) $(LDFLAGS)

pub:
	gcc -o publisher publisher.c $(CFLAGS) $(LDFLAGS)

sub:
	gcc -o subscriber subscriber.c $(CFLAGS) $(LDFLAGS)

clean:
	rm $(BIN)
