SHELL = /bin/sh

.PHONY: all
all:
	cd $(WORK_ROOT)/src/bc/vbh_sdk/vbh_sdk_comm && $(MAKE)
	cd $(WORK_ROOT)/src/bc/vbh_sdk/vbh_ca_client_sdk && $(MAKE)
	cd $(WORK_ROOT)/src/bc/vbh_sdk/vbh_client_sdk && $(MAKE)
	cd $(WORK_ROOT)/src/bc/vbh_sdk/vbh_ca_client_c_sdk && $(MAKE)
	cd $(WORK_ROOT)/src/bc/vbh_sdk/vbh_client_c_sdk && $(MAKE)
	cd $(WORK_ROOT)/src/bc/vbh_sdk/plugin && $(MAKE)

clean:
	for subdir in `ls -d */`; do \
		if [ -f $$subdir\Makefile ]; then \
			$(MAKE) clean -C $$subdir; \
		fi \
	done
