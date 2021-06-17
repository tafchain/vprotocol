#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_time.h"
#include "ace/Get_Opt.h"

#include "dsc/dsc_license.h"

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])  
{
	CDscString strMatchineInfo;
	ACE_INT32 nLimitMonth = -1;
	ACE_INT32 nLimitNum = -1;

	ACE_Get_Opt get_opt(argc, argv, ACE_TEXT("m:M:l:L:n:N:p:P"), 1);
	int c = get_opt();

	while (-1 != c)
	{
		switch (c)
		{
		case 'l':
		case 'L':
		{// 设置模块ID
			ACE_TCHAR* parg = get_opt.opt_arg();
			if (parg)
			{
				DSC::DscAtoi(parg, nLimitMonth);
			}
			else
			{
				ACE_OS::printf("no limit months!\n");

				return -1;
			}

			break;
		}
		case 'm':
		case 'M':
		{
			ACE_TCHAR* parg = get_opt.opt_arg();
			if (parg)
			{
				strMatchineInfo = parg;
			}
			else
			{
				ACE_OS::printf("no matchine info!\n");

				return -1;
			}

			break;
		}
		case 'n':
		case 'N':
		{// 设置模块ID
			ACE_TCHAR* parg = get_opt.opt_arg();
			if (parg)
			{
				DSC::DscAtoi(parg, nLimitNum);
			}
			else
			{
				ACE_OS::printf("no limit num!\n");

				return -1;
			}

			break;
		}
		case 'p':
		case 'P':
		{
			ACE_OS::printf("collect_matchine_info -M matchine info -L limit months -n limit num !\n");

			return 0;

			break;
		}
		default:
		{// 其它参数, 默认不处理
			break;
		}
		}
		c = get_opt();
	}

	if (strMatchineInfo.empty())
	{
		ACE_OS::printf("no matchine info!\n");

		return -1;
	}

	if (-1 == nLimitMonth)
	{
		ACE_OS::printf("no limit months!\n");

		return -1;
	}

	if (-1 == nLimitNum)
	{
		ACE_OS::printf("no limit num!\n");

		return -1;
	}

	CDscLicense rLicense;

    if( rLicense.genLicenseFile(strMatchineInfo, nLimitMonth, nLimitNum) )
    {
        ACE_OS::printf("generate licese file failed!\n");

        return -1;
    }

    return 0; 
}





