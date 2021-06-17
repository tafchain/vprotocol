CC_FLAG += -I$(WORK_ROOT)/src -D_REENTRANT -w -D_GNU_SOURCE -D_LINUX_PLATFORM_ -pthread -m64 -pipe

ifeq ($(release),1)
  CC_FLAG += -DUSING_GARBAGE_RECLAIM -DNDEBUG -w -O3 
  ifeq ($(native),1)
    CC_FLAG += -mtune=native -march=native -flto
  endif
else
  #CC_FLAG += -DDONNT_USING_MEMORY_POOL -ggdb -fstack-protector -fno-inline -fno-default-inline  -fno-strict-aliasing -fsanitize=address -fsanitize=leak
  CC_FLAG += -DDONNT_USING_MEMORY_POOL -DUSING_DSC_LOG_BIN -ggdb -fstack-protector -fno-inline -fno-default-inline
endif



ifeq ($(release),1)
  CC_LINK += -O3
  ifeq ($(native),1)
    CC_LINK += -mtune=native -march=native -flto
  endif
else
  CC_LINK = -ggdb -fstack-protector
endif

CC_LINK += -L$(WORK_ROOT)/lib -laxis2c -pthread -m64

include $(WORK_ROOT)/mk_inc/platform_common.mak

#define compiler
##################################################
CXX           = g++
CCC           = CXX
CC            = gcc
PIC           = -fPIC
AR            = ar
ARFLAGS       = rsuv
SOLINK.cc = $(CXX) $(PIC) -shared -o
##################################################

#compile switch
##################################################
