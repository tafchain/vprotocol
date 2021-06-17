SHELL = /bin/sh

.PHONY: all
all:
	cd $(WORK_ROOT)/src/dsc && $(MAKE) sqlite_db=1
	cd $(WORK_ROOT)/src/ztools && $(MAKE)
	cd $(WORK_ROOT)/src/bc && $(MAKE)
#	cd $(WORK_ROOT)/src/mq && $(MAKE)
	cd $(WORK_ROOT)/src/demo && $(MAKE)
	cd $(WORK_ROOT)/src/test && $(MAKE)
#	cd $(WORK_ROOT)/src/id_chain_app && $(MAKE)
	
clean:
	cd $(WORK_ROOT)/src/dsc && $(MAKE) sqlite_db=1 clean
	cd $(WORK_ROOT)/src/ztools && $(MAKE) clean
	cd $(WORK_ROOT)/src/bc && $(MAKE) clean
#	cd $(WORK_ROOT)/src/mq && $(MAKE) clean
	cd $(WORK_ROOT)/src/demo && $(MAKE) clean
	cd $(WORK_ROOT)/src/test && $(MAKE) clean
#	cd $(WORK_ROOT)/src/id_chain_app && $(MAKE) clean
	