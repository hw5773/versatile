obj-m 	+= flex.o
EXTRA_CFLAGS=-I/usr/local/include -DDEBUG

# Add every objects below
flex-y	:= af_flex.o flex_input.o flex_output.o flex_common.o flex_reliable.o flex_unreliable.o flex_idtable.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
