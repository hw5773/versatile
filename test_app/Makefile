CFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local/lib -lflex
BIN=publisher subscriber

all:
	gcc $(CFLAGS) $(LDFLAGS) -o publisher publisher.c
	gcc $(CFLAGS) $(LDFLAGS) -o subscriber subscriber.c

pub:
	gcc $(CFLAGS) $(LDFLAGS) -o publisher publisher.c

sub:
	gcc $(CFLAGS) $(LDFLAGS) -o subscriber subscriber.c

clean:
	rm $(BIN)
