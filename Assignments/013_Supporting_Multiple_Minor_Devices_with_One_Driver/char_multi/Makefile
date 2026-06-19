ifneq ($(KERNELRELEASE),)

obj-m += 01_multi_dev.o
obj-m += custom_multi_dev.o

else

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

auto:
	$(MAKE) -C $(KDIR) M=$(PWD) modules \
		obj-m="01_multi_dev.o"

custom:
	$(MAKE) -C $(KDIR) M=$(PWD) modules \
		obj-m="02_custom_multi_dev.o"

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

endif
