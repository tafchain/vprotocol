#ifndef DEMO_DB_SERVICE_H_324872348093246732187345234562341
#define DEMO_DB_SERVICE_H_324872348093246732187345234562341

#include "dsc/dsc_reactor.h"
#include "dsc/db/db_lib/db_database.h"

#include "demo/common/comm_id_def.h"

class PLUGIN_EXPORT CDemoDbService : public CDscReactor, public CDscServiceTimerHandler
{
public:
	enum
	{
		EN_SERVICE_TYPE = NS_DEMO_ID_DEF::DEMO_DB_SERVICE_TYPE
	};

public:
	CDemoDbService();

public:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);
	void OnTimer(void);

private:
	void InsertDemo(void);
	void UpdateDemo(void);
	void SelectDemo(void);
	void DeleteDemo(void);

private:
	CDscDatabase m_dscDatabase;
	CDBConnection m_dscConnect;
};

#endif