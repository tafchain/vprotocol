SHELL = /bin/sh

.PHONY: all
all:
	cd $(WORK_ROOT)/src/demo/demo_server/plugin && $(MAKE)
	cd $(WORK_ROOT)/src/demo/demo_server/demo_server && $(MAKE)
	
clean:
	cd $(WORK_ROOT)/src/demo/demo_server/plugin && $(MAKE) clean
	cd $(WORK_ROOT)/src/demo/demo_server/demo_server && $(MAKE) clean
