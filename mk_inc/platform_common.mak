OUTPUT_FLAG = -o $@

ifneq ($(delay),1)
  CC_FLAG += -DDSC_NO_DELAY
endif

# MYSQL MACROS
###################################################################
#run "mysql_config --cflags & mysql_config --libs" obtain mysql flag
ifeq ($(mysql_db),1)
  CC_FLAG +=  -DUSING_MYSQL_DB -I/usr/include/mysql
  #CC_FLAG += `mysql_config --cflags`

  CC_LINK += -L/usr/lib64/mysql -lmysqlclient
  #CC_LINK += `mysql_config --libs`
endif
#
###################################################################

# SQLITE MACROS
###################################################################
ifeq ($(sqlite_db),1)
  CC_FLAG += -DUSING_SQLITE_DB -I$(WORK_ROOT)/src/3rd/sqlite
  CC_LINK += -lvbhsqlite
endif
#
###################################################################

# ACE MACROS
###################################################################
#
ifeq ($(ACE),1)
  CC_FLAG +=  -I$(ACE_ROOT) -DACE_HAS_EXCEPTIONS -D__ACE_INLINE__ -DACE_HAS_LIMITED_SELECT
  ifeq ($(release),1)
    CC_FLAG += -DACE_NDEBUG
  endif
  CC_LINK += -lACE
endif
#

###################################################################

# OPENSSL MACROS
###################################################################
#
ifeq ($(OPENSSL),1)
  CC_FLAG += -I$(SSL_ROOT)/include
  CC_LINK += -L$(SSL_ROOT)/lib

  ifeq ($(plat),ios)
    CC_LINK += -lcrypto -lssl
  else
    CC_LINK += -lcryptovbh -lsslvbh
  endif
endif

#
###################################################################

