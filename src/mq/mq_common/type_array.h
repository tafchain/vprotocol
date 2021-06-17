#ifndef TYPE_ARRAY_H_749342047896724921679346783456
#define TYPE_ARRAY_H_749342047896724921679346783456

#include "ace/Basic_Types.h"

template<typename TYPE>
class CTypeArray
{
private:
	enum
	{
		EN_STEP_ENLARGE_NUM = 64
	};
	using type_ptr = TYPE * ;

public:
	CTypeArray();
	~CTypeArray();

public:
	void Insert(TYPE* p);
	TYPE* Delete(TYPE* p);
	TYPE* Get(void);
	bool IsEmpty(void);
	TYPE* Begin(void);
	TYPE* Next(TYPE* p);
	ACE_UINT32 Size(void);

private:
	type_ptr* m_ppArray;
	ACE_UINT32 m_nAllocNum = CTypeArray::EN_STEP_ENLARGE_NUM;
	ACE_UINT32 m_nCurNum = 0;
	ACE_UINT32 m_nIndex = 0;
};

#include "mq_common/type_array.inl"

#endif
