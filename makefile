ifneq ($(KERNELRELEASE),)
	obj-m := hello.o
else
	KERNELRELEASE	?= `uname -r`
	KERNEL_DIR	?= /lib/modules/$(KERNELRELEASE)/build
	PWD		:= $(shell pwd)
default:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
endif
