SHELL = /bin/sh

.PHONY: all
all:
	chmod +x $(WORK_ROOT)/src/3rd/ACE_wrappers/bin/add_rel_link.sh
	cd $(WORK_ROOT)/src/3rd && $(MAKE)
	cd $(WORK_ROOT)/src && $(MAKE) 
	
clean:
	cd $(WORK_ROOT)/src/3rd && $(MAKE) clean
	cd $(WORK_ROOT)/src && $(MAKE) clean
	