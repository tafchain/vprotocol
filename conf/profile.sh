#!/bin/sh

umask 022
export PWD=`pwd`
export PS1='$PWD > ' 

export ACE_ROOT=$HOME/work/src/3rd/ACE_wrappers
export WORK_ROOT=$HOME/work
export SSL_ROOT=$HOME/tools/openssl

export LD_LIBRARY_PATH=$WORK_ROOT/lib:$SSL_ROOT/lib
export PATH=./:$WORK_ROOT/bin:$PATH

alias h='history'
ulimit -c unlimited