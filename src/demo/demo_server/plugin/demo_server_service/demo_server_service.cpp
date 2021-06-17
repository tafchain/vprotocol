#include "dsc/storage/block_storage/dsc_block_format.h"
#include "dsc/storage/record_storage/record_storage.h"

#include "demo_server_service.h"

CDemoServerService::CTestTimer::CTestTimer(CDemoServerService& rDemoServerService)
: m_rDemoServerService(rDemoServerService)
{
}

void CDemoServerService::CTestTimer::OnTimer(void)
{
	m_rDemoServerService.OnTimeOut(this);
}

void CDemoServerService::OnTimeOut(CTestTimer* pTestTimer)
{
	//if (pTestTimer = &m_TestTimer1)
	//{
	//	this->CancelDscTimer(&m_TestTimer1);
	//	this->CancelDscTimer(&m_TestTimer2);
	//	this->CancelDscTimer(&m_TestTimer3);
	//	this->CancelDscTimer(&m_TestTimer4);
	//	this->CancelDscTimer(&m_TestTimer5);
	//	this->CancelDscTimer(&m_TestTimer6);
	//	this->CancelDscTimer(&m_TestTimer7);
	//	this->CancelDscTimer(&m_TestTimer8);
	//	this->CancelDscTimer(&m_TestTimer9);
	//	this->CancelDscTimer(&m_TestTimer10);

	//	this->SetDscTimer(&m_TestTimer2, 1, true);
	//	this->SetDscTimer(&m_TestTimer3, 1, true);
	//	this->SetDscTimer(&m_TestTimer4, 1, true);
	//	this->SetDscTimer(&m_TestTimer5, 1, true);
	//	this->SetDscTimer(&m_TestTimer6, 1, true);
	//	this->SetDscTimer(&m_TestTimer7, 1, true);
	//	this->SetDscTimer(&m_TestTimer8, 1, true);
	//	this->SetDscTimer(&m_TestTimer9, 1, true);
	//	this->SetDscTimer(&m_TestTimer10, 1, true);

	//	this->CancelDscTimer(&m_TestTimer2);
	//	this->CancelDscTimer(&m_TestTimer3);
	//	this->CancelDscTimer(&m_TestTimer4);
	//	this->CancelDscTimer(&m_TestTimer5);
	//	this->CancelDscTimer(&m_TestTimer6);
	//	this->CancelDscTimer(&m_TestTimer7);
	//	this->CancelDscTimer(&m_TestTimer8);
	//	this->CancelDscTimer(&m_TestTimer9);
	//	this->CancelDscTimer(&m_TestTimer10);
	//}
}

CDemoServerService::CDemoServerService()
//: m_TestTimer1(*this)
//, m_TestTimer2(*this)
//, m_TestTimer3(*this)
//, m_TestTimer4(*this)
//, m_TestTimer5(*this)
//, m_TestTimer6(*this)
//, m_TestTimer7(*this)
//, m_TestTimer8(*this)
//, m_TestTimer9(*this)
//, m_TestTimer10(*this)
{
}

ACE_INT32 CDemoServerService::OnInit(void)
{
	if(CDscReactor::OnInit())
	{
		DSC_RUN_LOG_ERROR("demo server service init failed!");

		return -1;
	}
	
	//this->SetDscTimer(&m_TestTimer1, 1, true);
	//this->SetDscTimer(&m_TestTimer2, 1, true);
	//this->SetDscTimer(&m_TestTimer3, 1, true);
	//this->SetDscTimer(&m_TestTimer4, 1, true);
	//this->SetDscTimer(&m_TestTimer5, 1, true);
	//this->SetDscTimer(&m_TestTimer6, 1, true);
	//this->SetDscTimer(&m_TestTimer7, 1, true);
	//this->SetDscTimer(&m_TestTimer8, 1, true);
	//this->SetDscTimer(&m_TestTimer9, 1, true);
	//this->SetDscTimer(&m_TestTimer10, 1, true);
	//CDscString strBlockIdPathName("D:\\work\\storage\\id.dat");
	//CDscString strStoragePathName("D:\\work\\storage\\data.dat");

	//BlockStorageFormat(strBlockIdPathName, strStoragePathName, 4096, 4096);

	//CRecordStorage rst;
	//char* record;
	//BLOCK_STORAGE::BLOCK_ID recordID;
	//ACE_INT32 nRet = 0;

	//rst.Open(strBlockIdPathName, strStoragePathName);

	//char* pRecord4Read = NULL;
	//ACE_UINT32 nRecordLen4Read = 0;

	//for (int i = 0; i < 128; ++i)
	//{
	//	record = (char*)malloc(1024);
	//	memset(record, 0, 1024);
	//	nRet = rst.Insert(record, 1024, recordID);
	//	printf("recordID:%lld, nRet:%d\n", recordID, nRet);

	//	rst.Read(pRecord4Read, nRecordLen4Read, recordID);
	//	memset(pRecord4Read, 1, nRecordLen4Read);
	//	rst.Update(pRecord4Read, nRecordLen4Read, recordID);
	//	DSC_THREAD_SIZE_FREE(pRecord4Read, nRecordLen4Read);

	//	rst.Read(pRecord4Read, nRecordLen4Read, recordID);
	//	DSC_THREAD_SIZE_FREE(pRecord4Read, nRecordLen4Read);
	//	printf("read record succeed\n");
	//}

	return 0;
}

//void CDemoServerService::OnDscMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& rDemoReqMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
//{
//	NS_DEMO_MSG_DEF::CDemoRspMsg rspMsg;
//
//	rspMsg.m_demoNestMag = rDemoReqMsg.m_demoNestMag;
//	rspMsg.m_lstdemoNestMag = rDemoReqMsg.m_lstdemoNestMag;
//
//	for (int i = 1; i < 2; ++i)
//	{
//		rspMsg.m_lstAttr.push_back(i);
//	}
//
//	rspMsg.m_strAttr = "456";
//	rspMsg.m_dtAttr.m_nYear = 2018;
//	rspMsg.m_dtAttr.m_nYear = 2018;
//	rspMsg.m_dtAttr.m_nMon = 10;
//	rspMsg.m_dtAttr.m_nDay = 28;
//	rspMsg.m_dtAttr.m_nHour = 11;
//	rspMsg.m_dtAttr.m_nMin = 18;
//	rspMsg.m_dtAttr.m_nSec = 30;
//
//	if (this->SendDscMessage(rspMsg, rSrcMsgAddr))
//	{
//		DSC_RUN_LOG_WARNING("send demo start mag failed!\n");
//	}
//}

void CDemoServerService::OnDscMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& rDemoReqMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	NS_DEMO_MSG_DEF::CDemoRspMsg rspMsg;

	rspMsg.m_nAttr = 456;

	if (this->SendDscMessage(rspMsg, rSrcMsgAddr))
	{
		DSC_RUN_LOG_WARNING("send demo start mag failed!\n");
	}
}

ACE_INT32 CDemoServerService::OnExit( void )
{
	return CDscReactor::OnExit();
}
