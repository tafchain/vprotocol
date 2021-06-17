#include "dsc/mem_mng/dsc_allocator.h"

template<typename TYPE>
CTypeArray<TYPE>::CTypeArray()
{
	m_ppArray = (type_ptr*)DSC_THREAD_SIZE_MALLOC(sizeof(type_ptr) * CTypeArray::EN_STEP_ENLARGE_NUM);
}

template<typename TYPE>
CTypeArray<TYPE>::~CTypeArray()
{
	DSC_THREAD_SIZE_FREE((char* )m_ppArray, sizeof(type_ptr) * m_nAllocNum);
}

template<typename TYPE>
void CTypeArray<TYPE>::Insert(TYPE* p)
{
	ACE_UINT32 nIndex = p->GetIndex();

	if (-1 == nIndex)
	{
		p->SetIndex(m_nCurNum);
		m_ppArray[m_nCurNum] = p;
		++m_nCurNum;
		if (m_nCurNum == m_nAllocNum)
		{
			type_ptr* ppArray;

			m_nAllocNum += CTypeArray::EN_STEP_ENLARGE_NUM;
			ppArray = (type_ptr*)DSC_THREAD_SIZE_MALLOC(sizeof(type_ptr) * m_nAllocNum);
			::memcpy(ppArray, m_ppArray, sizeof(type_ptr) * m_nCurNum);
			DSC_THREAD_SIZE_FREE((char*)m_ppArray, sizeof(type_ptr) * m_nCurNum);
			m_ppArray = ppArray;
		}
	}
}

template<typename TYPE>
TYPE* CTypeArray<TYPE>::Delete(TYPE* p)
{
	ACE_UINT32 nIndex = p->GetIndex();

	if (-1 == nIndex)
	{
		return NULL;
	}
	else
	{
		TYPE* pNext;

		--m_nCurNum;
		if (nIndex < m_nCurNum)
		{
			pNext = m_ppArray[m_nCurNum];
			m_ppArray[nIndex] = pNext;
			pNext->SetIndex(nIndex);			
		}
		else
		{
			++nIndex;
			if (nIndex < m_nCurNum)
			{
				pNext = m_ppArray[nIndex];
			}
			else
			{
				pNext = NULL;
			}
		}
		if (m_nIndex >= m_nCurNum)
		{
			m_nIndex = 0;
		};

		p->SetIndex(-1);

		return pNext;
	}
}

template<typename TYPE>
TYPE* CTypeArray<TYPE>::Get(void)
{
	if (m_nCurNum)
	{
		++m_nIndex;
		if (m_nIndex < m_nCurNum)
		{
			return m_ppArray[m_nIndex];
		}
		else
		{
			m_nIndex = 0;

			return m_ppArray[m_nIndex];
		}
	}
	else
	{
		return NULL;
	}
}

template<typename TYPE>
inline bool CTypeArray<TYPE>::IsEmpty(void)
{
	return 0 == m_nCurNum;
}

template<typename TYPE>
TYPE* CTypeArray<TYPE>::Begin(void)
{
	if (m_nCurNum)
	{
		return m_ppArray[0];
	}
	else
	{
		return NULL;
	}
}

template<typename TYPE>
TYPE* CTypeArray<TYPE>::Next(TYPE* p)
{
	ACE_UINT32 nIndex = p->GetIndex();

	++nIndex;
	if (nIndex < m_nCurNum)
	{
		return m_ppArray[nIndex];
	}
	else
	{
		return NULL;
	}
}

template<typename TYPE>
inline ACE_UINT32 CTypeArray<TYPE>::Size(void)
{
	return m_nCurNum;
}
