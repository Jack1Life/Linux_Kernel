NAME = home
obj-m := $(NAME).o 
$(NAME)-objs := hw.o kparens.o kobj.o
all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
test:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	sudo dmesg -C
	sudo insmod $(NAME).ko
	dmesg
del:
	sudo rmmod $(NAME)
	-sudo rm /dev/$(NAME)