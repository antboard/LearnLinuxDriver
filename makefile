obj-m := hello.o

# KERNELRELEASE	?= `uname -r`
# KERNEL_DIR	?= /lib/modules/$(KERNELRELEASE)/build
# PWD		:= $(shell pwd)
# $(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules