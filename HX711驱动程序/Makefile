KERNELDIR := /home/lizhou/ti-processor-sdk-linux-rt-am57xx-evm-05.03.00.07/board-support_5.3.1/linux-rt-4.14.79+gitAUTOINC+a72bf1418c-ga72bf1418c
CURRENT_PATH := $(shell pwd)
obj-m := new_hx711.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR)  M=$(CURRENT_PATH) modules
clean:
	$(MAKE) -C $(KERNELDIR)  M=$(CURRENT_PATH) clean
