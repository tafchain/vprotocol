SHELL = /bin/sh

.PHONY: all
all:
	cd $(WORK_ROOT)/src/bc/vbh_comm && $(MAKE)
	cd $(WORK_ROOT)/src/bc/vbh_ca_server && $(MAKE)
	cd $(WORK_ROOT)/src/bc/vbh_cc && $(MAKE)
	cd $(WORK_ROOT)/src/bc/vbh_kernel && $(MAKE)
	cd $(WORK_ROOT)/src/bc/vbh_sdk && $(MAKE)
	cd $(WORK_ROOT)/src/bc/vbh_test && $(MAKE)
	
clean:
	for subdir in `ls -d */`; do \
		if [ -f $$subdir\Makefile ]; then \
			$(MAKE) clean -C $$subdir; \
		fi \
	done
