obj-m += task_list_module.o
ifeq ($(colors),1)
	NOESC := --noescape
	CFLAGS_${obj-m} := -DPRINTK_COLORS
endif	

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
