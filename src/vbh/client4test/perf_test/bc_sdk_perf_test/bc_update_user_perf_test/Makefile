SHELL = /bin/sh

##################################################################### #
#1、定义您自己的宏，比方您自己模块根目录
VBH_SDK_PERF_TEST_DIR=$(WORK_ROOT)/src/bc/vbh_test/vbh_perf_test/vbh_sdk_perf_test/vbh_update_user_perf_test

#2、定义您自己的可执行文件名称
PROGRAM_NAME=$(WORK_ROOT)/bin/vbh_update_user_perf_test
##################################################################### #
#3、指定您必须生成的工程文件

SOURCE = $(wildcard $(VBH_SDK_PERF_TEST_DIR)/*.cpp)

##################################################################### #

#4、指定您附加的搜索编译目录，不要指到其他模块内部
USER_CC_FLAG = -I$(WORK_ROOT)/src/bc \
	 -I$(WORK_ROOT)/src/bc/vbh_sdk \
	  -I$(WORK_ROOT)/src/bc/vbh_test 
	
#6、指定您附加的库文件搜索目录，例如：-L./MYSUBDIR
#USER_CC_LINK = -L$(WORK_ROOT)/lib

#7、指定您附加的库文件，例如：-lhello（假设您的动态库名称为libhello.so）
USER_CC_LINK += -ldsc -lvbh_ca_client_sdk -lvbh_client_sdk -lvbh_sdk_comm -lvbh_comm

#####################################################################
#
#8、包含统一文件，用来定义编译环境，不要更改
.PHONY: all
all: $(PROGRAM_NAME)
	@echo "[Complier $(PROGRAM_NAME) end time -- `date`]"

ACE=1
OPENSSL=1
include $(WORK_ROOT)/mk_inc/rules.mak

