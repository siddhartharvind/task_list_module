obj-m += task_list_module.o
MODULE_NAME := task_list_module

define yellow
\e[01;33m$(1)\e[0m
endef

ifeq ($(colors),1)
# -D<name> => #define <name> in source file
	NOESC := --noescape
	CFLAGS_${obj-m} := -DPRINTK_COLORS
endif

.PHONY: all clean print load remove check

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


ifeq ($(clear), 1)
CLEAR := -c
endif

ifeq ($(less), 1)
LESS := | (head -14; echo "..."; tail -13)
endif

print:
	sudo dmesg --color=always ${CLEAR} ${NOESC} ${LESS}
# --color=always to keep colour when piping


IS_LOADED := $(shell lsmod | grep ${MODULE_NAME} | wc -l)

load:
# Loads kernel module
ifeq ($(IS_LOADED), 0)
	sudo dmesg -C
	sudo insmod ${MODULE_NAME}.ko
	@echo -e 'make: Loaded module $(call yellow,${MODULE_NAME})'
else
	@echo -e 'make: Module $(call yellow,${MODULE_NAME}) is already loaded.'
endif


remove:
# Removes kernel module
ifeq ($(IS_LOADED), 1)
	sudo rmmod ${MODULE_NAME}
	@echo -e 'make: Removed module $(call yellow,${MODULE_NAME})'
else
	@echo -e 'make: Module $(call yellow,${MODULE_NAME}) is not loaded.'
endif


check:
# Checks whether module is loaded or not
	@echo -ne 'Module $(call yellow,${MODULE_NAME}) is '
ifeq (${IS_LOADED}, 0)
	@echo -n "not "
endif
	@echo "loaded."
