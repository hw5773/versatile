obj-m 	+= flex.o

# Add every objects below
flex-y	:= af_flex.o flex_input.o flex_output.o flex_common.o flex_reliable.o flex_unreliable.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
