SHELL = /bin/sh

#1、定义您自己的宏，比如您自己模块根目录
VBH_CLIENT_SDK_DIR=$(WORK_ROOT)/src/bc/vbh_sdk/vbh_client_sdk
#2、定义您自己的可执行文件名称
SHARED_LIB_NAME=$(WORK_ROOT)/lib/libvbh_client_sdk.so

##################################################################### #

#3、指定您必须生成的工程文件
SOURCE = $(wildcard $(VBH_CLIENT_SDK_DIR)/*.cpp)
##################################################################### #

#5、指定您附加的搜索编译目录，不要指到其他模块内部
USER_CC_FLAG = -I$(WORK_ROOT)/src/comm \
			-I$(WORK_ROOT)/src/bc \
		  -I$(WORK_ROOT)/src/bc/vbh_sdk

######################################################################

#5、指定您附加的编译开关，通常为宏定义，例如：-DMMS_DEBUG  暂时只对下面3种系统的编译选项修改
# AIX 需要加 -qtls=local-dynamic  HP-UX 需要加 -mt  Linux 需要加  -fvisibility=hidden -fvisibility-inlines-hidden
USER_CC_FLAG += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
######################################################################

#7、指定您附加的库文件搜索目录，例如：-L./MYSUBDIR

######################################################################

#8、指定您附加的库文件，例如：-lhello（假设您的动态库名称为libhello.so）
USER_CC_LINK = -ldsc -lvbh_comm -lvbh_sdk_comm

######################################################################

#9、包含统一文件，用来定义编译环境，不要更改
.PHONY: all
all: $(SHARED_LIB_NAME)
	@echo "[Complier $(SHARED_LIB_NAME) end time -- `date`]"

ACE=1
include $(WORK_ROOT)/mk_inc/rules.mak

