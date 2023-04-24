MODULE_NAME := task_list_module
obj-m += task_list_module.o
CFLAGS_${obj-m} = # empty

define yellow
\e[01;33m$(1)\e[0m
endef

ifeq ($(colors), 1)
	NOESC := --noescape
	CFLAGS_${obj-m} += -DPRINTK_COLORS
endif

ifeq ($(mode), 1)
# This sets the module to list tasks using DFS.
	CFLAGS_${obj-m} += -DTASK_LIST_MODE_DFS
else
# Default behaviour: for any other value, set
# the module to list tasks in linear order.
	CFLAGS_${obj-m} += -DTASK_LIST_MODE_LINEAR
endif


.PHONY: all
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

.PHONY: clean
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	$(RM) *.out


# Recipe to run demo of module
.PHONY: demo
demo: all ldrm print clean


ifeq ($(clear), 1)
CLEAR := -c
endif

ifeq ($(less), 1)
LESS := | (head -14; echo "..."; tail -13)
endif

.PHONY: print
print:
	sudo dmesg --color=always ${CLEAR} ${NOESC} ${LESS}
# --color=always to keep colour when piping


IS_LOADED := $(shell lsmod | grep ${MODULE_NAME} | wc -l)

.PHONY: load
load:
# Loads kernel module
ifeq ($(IS_LOADED), 0)
	sudo dmesg -C
	sudo insmod ${MODULE_NAME}.ko
	@echo -e 'make: Loaded module $(call yellow,${MODULE_NAME})'
else
	@echo -e 'make: Module $(call yellow,${MODULE_NAME}) is already loaded.'
endif


.PHONY: remove
remove:
# Removes kernel module
ifeq ($(IS_LOADED), 1)
	sudo rmmod ${MODULE_NAME}
	@echo -e 'make: Removed module $(call yellow,${MODULE_NAME})'
else
	@echo -e 'make: Module $(call yellow,${MODULE_NAME}) is not loaded.'
endif


.PHONY: ldrm
ldrm:
# Helper for `demo`
	${MAKE} load
	${MAKE} remove


.PHONY: check
check:
# Checks whether module is loaded or not
	@echo -ne 'Module $(call yellow,${MODULE_NAME}) is '
ifeq (${IS_LOADED}, 0)
	@echo -n "not "
endif
	@echo "loaded."
