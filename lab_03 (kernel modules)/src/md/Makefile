CONFIG_MODULE_SIG=n

ifneq ($(KERNELRELEASE),)
	obj-m := md1.o md2.o md3.o

else
	CURRENT = $(shell uname -r)
	KDIR = /lib/modules/$(CURRENT)/build
	PWD = $(shell pwd)

default:
	sudo $(MAKE) -C $(KDIR) M=$(PWD) modules
	sudo make clean

clean:
	rm -rf .tmp_versions
	rm .md1.*
	rm .md2.*
	rm .md3.*
	rm *.o
	rm *.mod.c
	rm *.symvers
	rm *.order

endif
