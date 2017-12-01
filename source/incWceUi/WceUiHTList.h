// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once

#include "WceUiCommon.h"

//
//inner use
//

//////////////////////////////////////////////////////////////////////////
////////////////////////////////H list begin//////////////////////////////
//////////////////////////////////////////////////////////////////////////
//only add element from head
//eg. add order is 1, 2, 3, 4, 5, then access order is 5, 4, 3, 2, 1
class CWceUiHList
{
public:
	CWceUiHList(size_t nNextOffset);
	void Construct(size_t nNextOffset);

	BOOL IsEmpty() const;
	void* AddHead(void* p);
	void RemoveAll();
	void* GetHead() const;
	void* GetNext(void* p) const;
	BOOL Remove(void* p);
	int GetCount();

private:
	void*	m_pHead;
	size_t	m_nNextOffset;
	int		m_nCount;

	void** GetNextPtr(void* p) const;
private:
	CWceUiHList (const CWceUiHList  &);
	CWceUiHList & operator=(const CWceUiHList &);
};


template<class TYPE>
class CWceUiTypedHList : public CWceUiHList
{
public:
	CWceUiTypedHList(size_t nNextOffset)
		: CWceUiHList(nNextOffset) 
	{
	}

	TYPE AddHead(TYPE p)
	{ 
		return (TYPE)CWceUiHList::AddHead(p); 
	}

	TYPE GetHead()
	{ 
		return (TYPE)CWceUiHList::GetHead(); 
	}

	TYPE GetNext(TYPE p)
	{ 
		return (TYPE)CWceUiHList::GetNext(p); 
	}

	BOOL Remove(TYPE p)
	{
		return CWceUiHList::Remove((TYPE)p); 
	}
};
//////////////////////////////////////////////////////////////////////////
////////////////////////////////H list End////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
////////////////////////////////T list begin//////////////////////////////
//////////////////////////////////////////////////////////////////////////
//only add element from tail
//eg. add order is 1, 2, 3, 4, 5, then access order is 1, 2, 3, 4, 5

class CWceUiTList
{
public:
	CWceUiTList(size_t nNextOffset);
	void Construct(size_t nNextOffset);

	BOOL IsEmpty() const;
	void* AddTail(void* p);
	void RemoveAll();
	void* GetHead() const;
	void* GetNext(void* p) const;
	BOOL Remove(void* p);
	void* FindIndex(int nIndex) const;
	int GetCount() const;

private:
	void* m_pHead;
	void* m_pTail;
	size_t m_nNextOffset;
	int		m_nCount;

	void** GetNextPtr(void* p) const;

private:
	CWceUiTList & operator=(const CWceUiTList &);
	CWceUiTList (const CWceUiTList  &);
};


template<class TYPE>
class CWceUiTypedTList : public CWceUiTList
{
public:
	CWceUiTypedTList(size_t nNextOffset)
		: CWceUiTList(nNextOffset) 
	{
	}

	TYPE AddTail(TYPE p)
	{ 
		return (TYPE)CWceUiTList::AddTail(p); 
	}

	TYPE GetHead()
	{ 
		return (TYPE)CWceUiTList::GetHead(); 
	}

	TYPE GetNext(TYPE p)
	{ 
		return (TYPE)CWceUiTList::GetNext(p); 
	}

	BOOL Remove(TYPE p)
	{
		return CWceUiTList::Remove((TYPE)p); 
	}

	TYPE FindIndex(int nIndex) const
	{
		return (TYPE)CWceUiTList::FindIndex(nIndex);
	}
};
//////////////////////////////////////////////////////////////////////////
////////////////////////////////T list End////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
////////////////////////////////HT list begin/////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CWceUiHTList
{
public:
	CWceUiHTList(size_t nPreOffset, size_t nNextOffset);
	void Construct(size_t nPreOffset, size_t nNextOffset);

	BOOL IsEmpty() const;
	void* AddHead(void* p);
	void* AddTail(void* p);
	void RemoveAll();
	void* GetHead() const;
	void* GetTail() const;
	void* RemoveHead();
	void* RemoveTail();
	void* GetPre(void* p) const;
	void* GetNext(void* p) const;
	void* InsertBefore(void* pBeforeThis, void* pInsert);
	void* InsertAfter(void* pAfterThis, void* pInsert);
	void* Find(void* pSearch, void* pStartAfter = NULL) const;
	void* FindIndex(int nIndex ) const;
	BOOL Remove(void* p);
	int GetCount() const;
private:
	void* m_pHead;
	void* m_pTail;
	size_t m_nNextOffset;
	size_t m_nPreOffset;
	int		m_nCount;

	void** GetPrePtr(void* p) const;
	void** GetNextPtr(void* p) const;

private:
	CWceUiHTList (const CWceUiHTList  &);
	CWceUiHTList & operator=(const CWceUiHTList &);
};


template<class TYPE>
class CWceUiTypedHTList : public CWceUiHTList
{
public:
	CWceUiTypedHTList(size_t nPreOffset, size_t nNextOffset)
		: CWceUiHTList(nPreOffset, nNextOffset) 
	{
	}

	TYPE AddHead(TYPE p)
	{
		return (TYPE)CWceUiHTList::AddHead(p);
	}

	TYPE AddTail(TYPE p)
	{ 
		return (TYPE)CWceUiHTList::AddTail(p); 
	}

	TYPE GetHead() const
	{ 
		return (TYPE)CWceUiHTList::GetHead(); 
	}

	TYPE GetTail() const
	{
		return (TYPE)CWceUiHTList::GetTail();
	}

	TYPE RemoveHead()
	{
		return (TYPE)CWceUiHTList::RemoveHead();
	}

	TYPE RemoveTail()
	{
		return (TYPE)CWceUiHTList::RemoveTail();
	}

	TYPE GetPre(TYPE p) const
	{
		return (TYPE)CWceUiHTList::GetPre(p);
	}

	TYPE GetNext(TYPE p) const
	{
		return (TYPE)CWceUiHTList::GetNext(p);
	}

	TYPE InsertBefore(TYPE pBeforeThis, TYPE pInsert)
	{
		return (TYPE)CWceUiHTList::InsertBefore(pBeforeThis, pInsert);
	}

	TYPE InsertAfter(TYPE pAfterThis, TYPE pInsert)
	{
		return (TYPE)CWceUiHTList::InsertAfter(pAfterThis, pInsert);
	}

	TYPE Find(TYPE pSearch, TYPE pStartAfter = NULL )
	{
		return (TYPE)CWceUiHTList::Find(pSearch, pStartAfter);
	}

	TYPE FindIndex(int nIndex) const
	{
		return (TYPE)CWceUiHTList::FindIndex(nIndex);
	}

	BOOL Remove(TYPE p)
	{ 
		return CWceUiHTList::Remove((TYPE)p); 
	}
};


//////////////////////////////////////////////////////////////////////////
////////////////////////////////HT list End///////////////////////////////
//////////////////////////////////////////////////////////////////////////