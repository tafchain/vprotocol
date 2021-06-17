#ifndef PER_USER_DEF_H_4328423083218321978547043284236864231
#define PER_USER_DEF_H_4328423083218321978547043284236864231

#include "dsc/db/per/persistence.h"

class CPerUser
{
public:
	CPerUser()
		: m_user_id("user_id")
		, m_user_name("user_name")
		, m_user_date("user_date")
	{
	}

public:
	PER_BIND_ATTR(m_user_id, m_user_name, m_user_date);

public:
	CColumnWrapper< ACE_INT32 > m_user_id;
	CColumnWrapper< CDscString > m_user_name;
	CColumnWrapper< CDBDateTime > m_user_date;
};

class CPerUserSelectCriterion : public CSelectCriterion
{
public:
	virtual void SetCriterion(CPerSelect& rPerSelect)
	{
		rPerSelect.Where(rPerSelect["user_id"] == 123);
	}
};

class CPerUserUpdateCriterion : public CUpdateCriterion
{
public:
	virtual void SetCriterion(CPerUpdate& rPerUpdate)
	{
		rPerUpdate.Where(rPerUpdate["user_id"] == 123);
	}
};

class CPerUserDeleteCriterion : public CDeleteCriterion
{
public:
	virtual void SetCriterion(CPerDelete& rPerDelete)
	{
		rPerDelete.Where(rPerDelete["user_id"] == 123);
	}
};

#endif