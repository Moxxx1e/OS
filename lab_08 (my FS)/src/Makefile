CONFIG_MODULE_SIG=n

ifneq ($(KERNELRELEASE),)
	obj-m := myfs.o

else
	CURRENT = $(shell uname -r)
	KDIR = /lib/modules/$(CURRENT)/build
	PWD = $(shell pwd)

default:
	sudo $(MAKE) -C $(KDIR) M=$(PWD) modules
	sudo make clean

clean:
	rm -rf .tmp_versions
	rm .myfs.*
	rm *.o
	rm *.mod.c
	rm *.symvers
	rm *.order

endif
