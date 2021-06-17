SHELL = /bin/sh

##################################################################### #
#1、定义您自己的宏，比方您自己模块根目录
VBH_KERNEL_DIR=$(WORK_ROOT)/src/bc/vbh_kernel/vbh_kernel

#2、定义您自己的可执行文件名称
PROGRAM_NAME=$(WORK_ROOT)/bin/vbh_kernel
##################################################################### #
#3、指定您必须生成的工程文件

SOURCE  = $(wildcard $(VBH_KERNEL_DIR)/*.cpp)

ifeq ($(DSC_TEST),1)
VBH_ENDORSER_SERVICE_DIR=$(WORK_ROOT)/src/bc/vbh_kernel/plugin/vbh_endorser_service
VBH_ORDER_SERVICE_DIR=$(WORK_ROOT)/src/bc/vbh_kernel/plugin/vbh_order_service
VBH_ANCHOR_SERVICE_DIR=$(WORK_ROOT)/src/bc/vbh_kernel/plugin/vbh_anchor_service
VBH_COMMITTER_SERVICE_DIR=$(WORK_ROOT)/src/bc/vbh_kernel/plugin/vbh_committer_service
VBH_STORAGE_DIR=$(WORK_ROOT)/src/bc/vbh_kernel/plugin/vbh_storage

VBH_CC_DEMO_SERVICE_DIR=$(WORK_ROOT)/src/bc/vbh_cc/cc_demo_service
VBH_CC_JSON_DEMO_SERVICE_DIR=$(WORK_ROOT)/src/bc/vbh_cc/cc_json_demo_service

SOURCE += $(wildcard $(VBH_ENDORSER_SERVICE_DIR)/*.cpp)
SOURCE += $(wildcard $(VBH_ORDER_SERVICE_DIR)/*.cpp)
SOURCE += $(wildcard $(VBH_COMMITTER_SERVICE_DIR)/*.cpp)
SOURCE += $(wildcard $(VBH_ANCHOR_SERVICE_DIR)/*.cpp)
SOURCE += $(wildcard $(VBH_STORAGE_DIR)/*.cpp)

SOURCE += $(wildcard $(VBH_CC_DEMO_SERVICE_DIR)/*.cpp)
SOURCE += $(wildcard $(VBH_CC_JSON_DEMO_SERVICE_DIR)/*.cpp)
SOURCE += $(wildcard $(WORK_ROOT)/src/bc/vbh_cc/cc_comm/*.cpp)
endif

##################################################################### #

#4、指定您附加的搜索编译目录，不要指到其他模块内部
USER_CC_FLAG = -I$(WORK_ROOT)/src/bc \
			-I$(WORK_ROOT)/src/bc/vbh_kernel \

ifeq ($(DSC_TEST),1)
USER_CC_FLAG += -DDSC_TEST -I$(WORK_ROOT)/src/bc/vbh_kernel/plugin \
	-I$(WORK_ROOT)/src/3rd/leveldb/include \
	-I$(WORK_ROOT)/src/bc/vbh_cc \
	-I$(WORK_ROOT)/src/bc/vbh_test
	
endif

#6、指定您附加的库文件搜索目录，例如：-L./MYSUBDIR
#USER_CC_LINK = -L$(WORK_ROOT)/lib

#7、指定您附加的库文件，例如：-lhello（假设您的动态库名称为libhello.so）
USER_CC_LINK += -ldsc -lvbh_comm
ifeq ($(DSC_TEST),1)
USER_CC_LINK += -lvbh_comm
endif

ifeq ($(DSC_TEST),1)
USER_CC_LINK += -lleveldb 
endif
#####################################################################
#
#8、包含统一文件，用来定义编译环境，不要更改
.PHONY: all
all: $(PROGRAM_NAME)
	@echo "[Complier $(PROGRAM_NAME) end time -- `date`]"

ACE=1
OPENSSL=1

include $(WORK_ROOT)/mk_inc/rules.mak

