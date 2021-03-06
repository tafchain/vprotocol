#include "dsc/dsc_database_factory.h"

#include "demo_db_service.h"
#include "per_user_def.h"

CDemoDbService::CDemoDbService()
{
}

ACE_INT32 CDemoDbService::OnInit(void)
{
	if(CDscReactor::OnInit())
	{
		DSC_RUN_LOG_ERROR("demo server service init failed!");

		return -1;
	}
	
	if (CDscDatabaseFactory::instance()->CreateDatabase(m_dscDatabase, m_dscConnect))
	{
		DSC_RUN_LOG_ERROR("connect to database failed!");

		return -1;
	}

	this->SetDscTimer(this, 1, true);

	return 0;
}

ACE_INT32 CDemoDbService::OnExit( void )
{
	this->CancelDscTimer(this);

	return CDscReactor::OnExit();
}

void CDemoDbService::OnTimer(void)
{
	this->DeleteDemo();
	this->InsertDemo();
	this->UpdateDemo();
	this->SelectDemo();
}

void CDemoDbService::InsertDemo(void)
{
	CTableWrapper< CPerUser > tblPerUser("T_USER");
	CPerUser& user = *tblPerUser;
	CDBDateTime& userDateTime = *user.m_user_date;

	userDateTime = CDBDateTime::GetDBDateTime(this->GetCurTime());
	user.m_user_id.SetValue(123);
	user.m_user_name.SetValue("fw123");
	user.m_user_date.SetChanged(true);

	if (::PerInsert(tblPerUser, m_dscDatabase, m_dscConnect))
	{
		DSC_RUN_LOG_ERROR("insert user info failed!");
	}
}

void CDemoDbService::UpdateDemo(void)
{
	CTableWrapper< CPerUser > tblPerUser("T_USER");
	CPerUserUpdateCriterion userCriterion;

	tblPerUser->m_user_name.SetValue("fw456");

	if (::PerUpdate(tblPerUser, m_dscDatabase, m_dscConnect, userCriterion))
	{
		DSC_RUN_LOG_ERROR("insert user info failed!");
	}
}

void CDemoDbService::SelectDemo(void)
{
	CTableWrapper< CPerUser > tblPerUser("T_USER");
	CPerUserSelectCriterion userCriterion;
	
	if (::PerSelect(tblPerUser, m_dscDatabase, m_dscConnect, &userCriterion))
	{
		DSC_RUN_LOG_ERROR("insert user info failed!");
	}
	else
	{
		ACE_OS::printf("user id:%d, user name:%s, date->year:%d, month:%d, day:%d, hour:%d, minute:%d, second:%d.\n",
			*tblPerUser->m_user_id, tblPerUser->m_user_name->c_str(), tblPerUser->m_user_date->GetYear(),
			tblPerUser->m_user_date->GetMon(), tblPerUser->m_user_date->GetDay(),
			tblPerUser->m_user_date->GetHour(), tblPerUser->m_user_date->GetMinute(), tblPerUser->m_user_date->GetSecond());
	}
}

void CDemoDbService::DeleteDemo(void)
{
	CDscString strUser("T_USER");
	CPerUserDeleteCriterion userCriterion;

	if (::PerDelete(strUser, m_dscDatabase, m_dscConnect, &userCriterion))
	{
		DSC_RUN_LOG_ERROR("insert user info failed!");
	}
}
