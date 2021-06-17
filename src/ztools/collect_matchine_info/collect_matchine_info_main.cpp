#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_time.h"

#include "dsc/dsc_license.h"

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])  
{
	CDscLicense rLicense;
    CDscString strHardware;

    if( rLicense.getMatchineStr(strHardware) )
    {
        ACE_OS::printf("get local identifier failed!\n");

        return -1;
    }

    ACE_OS::printf("matchine info:<%s>\n",strHardware.c_str());

    return 0; 
}





