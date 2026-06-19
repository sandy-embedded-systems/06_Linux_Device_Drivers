ifneq ($(KERNELRELEASE),)

obj-m += 01_waitq_driver.o
obj-m += 02_custom_waitq_driver.o

else

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

auto:
	$(MAKE) -C $(KDIR) M=$(PWD) modules \
		obj-m="01_waitq_driver.o"

custom:
	$(MAKE) -C $(KDIR) M=$(PWD) modules \
		obj-m="02_custom_waitq_driver.o"

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

endif
