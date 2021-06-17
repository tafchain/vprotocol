CC_ARG = -I$(ACE_ROOT) -I$(WORK_ROOT)/src -I$(WORK_ROOT)/src/vbh/committer/plugin -DACE_HAS_EXCEPTIONS -D__ACE_INLINE__ -DACE_HAS_LIMITED_SELECT -DACE_NDEBUG -D_FREEBSD_PLATFORM_
LINK_ARG = -L$(WORK_ROOT)/lib -lACE -pthread

ifeq ($(arm_plat),1)
CXX           = arm-linux-gnueabihf-g++
CC            = arm-linux-gnueabihf-gcc
AR            = arm-linux-gnueabihf-ar
else
CXX           = g++
CC            = gcc
AR            = ar
endif

CCC           = CXX
PIC           = -fPIC
ARFLAGS       = rsuv
SOLINK.cc = $(CXX) $(PIC) -shared -o

ifeq ($(release),1)
CC_ARG += -O2 -g
LINK_ARG += -O2 -g
else
CC_ARG += -ggdb -fstack-protector -fno-inline -fno-default-inline
LINK_ARG +=  -ggdb -fstack-protector 
endif

PROGRAM_NAME=$(WORK_ROOT)/bin/test

SHARED_MEMORY_DIR=$(WORK_ROOT)/src/dsc/shared_memory

SOURCE = $(wildcard $(WORK_ROOT)/src/test/*.cpp)
SOURCE += $(wildcard $(WORK_ROOT)/src/vbh/committer/plugin/xcm_xcs/bc_index.cpp)

OBJECTS = $(SOURCE:.cpp=.o)

OUTPUT_FLAG = -o $@

.PHONY: all
all: $(PROGRAM_NAME)

display_env:
	@echo "[ Compiler: \"$(CCC)\"    Args: \"$(CC_ARG)\" ]"
	@echo "[ Linker: \"$(CCC)\"    Args: \"$(LINK_ARG)\" ]"

clean:
	@echo "[Cleanning...]"
	@rm -rf $(PROGRAM_NAME)
	@rm -rf $(OBJECTS) core* ir.out
	@rm -rf *.a *.so

cl:
	@echo "[Cleanning...]"
	@rm -rf $(PROGRAM_NAME)
	@rm -rf *.o core ir.out

%.o: %.cpp
	$(COMPILE.cc) $(CC_ARG) $< $(OUTPUT_FLAG)

$(PROGRAM_NAME): $(OBJECTS)
	$(LINK.cc) $(OUTPUT_FLAG) $(OBJECTS) $(CC_ARG) $(LINK_ARG) 
