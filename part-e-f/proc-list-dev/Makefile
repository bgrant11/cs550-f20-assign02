CONFIG_MODULE_SIG=n
obj-m += process_list.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

CC := $(CROSS_COMPILE)gcc

all:
	$(MAKE) -C $(KDIR) M=${shell pwd} modules
        
clean:
	-$(MAKE) -C $(KDIR) M=${shell pwd} clean || true
	-rm *.o *.ko *.mod.{c,o} modules.order Module.symvers || true

in:
	sudo insmod process_list.ko

out:
	sudo rmmod process_list

#Username: bgrant11    
#IP: 128.226.28.59
#Password: NWKqjEi73

#/home/bgrant11/cs550-f20-assign02

#//home/work/cs/20fall/550/cs550-f20-assign02


