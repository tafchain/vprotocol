SHELL = /bin/sh

##################################################################### #
#1、定义您自己的宏，比方您自己模块根目录
TEST_MQ_2_DIR=$(WORK_ROOT)/src/mq/test_mq/test_mq_2

#2、定义您自己的可执行文件名称
PROGRAM_NAME=$(WORK_ROOT)/bin/test_mq_2
##################################################################### #
#3、指定您必须生成的工程文件

SOURCE = $(wildcard $(TEST_MQ_2_DIR)/*.cpp)
##################################################################### #

#4、指定您附加的搜索编译目录，不要指到其他模块内部
USER_CC_FLAG = -I$(WORK_ROOT)/src/comm \
							-I$(WORK_ROOT)/src/mq \
							-I$(WORK_ROOT)/src/mq/test_mq

#6、指定您附加的库文件搜索目录，例如：-L./MYSUBDIR
#USER_CC_LINK = -L$(WORK_ROOT)/lib

#7、指定您附加的库文件，例如：-lhello（假设您的动态库名称为libhello.so）
USER_CC_LINK += -lmq_client_sdk -lcodec -ldsc

#####################################################################
#
#8、包含统一文件，用来定义编译环境，不要更改
.PHONY: all
all: $(PROGRAM_NAME)

include $(WORK_ROOT)/mk_inc/rules.mak

