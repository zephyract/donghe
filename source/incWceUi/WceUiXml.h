// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once

#include "WceUiCommon.h"
#include "WceUiHTList.h"

#define XML_MAX_LEVEL 32 //the supported max level
#define MAX_TAG_NAME_LEN 64 //the max length of xml tag name, include the '\0'

enum WceUiXmlElemType
{
	XML_ELEM_COMMENT,
	XML_ELEM_DECLARATION,
	XML_ELEM_NODE,
};

class CWceUiXmlElem;
typedef CWceUiXmlElem* PCWceUiXmlElem;

typedef struct tWceUiXmlStack
{
	PCWceUiXmlElem pElem; //the pointer of element
	LPCTSTR        pName; //the name of the element, for check the xml tag is in pair.
}WceUiXmlStack, *PWceUiXmlStack;

typedef struct tWceUiXmlAttr
{
	LPCTSTR pName;
	LPCTSTR pValue;
public:NO_WRITE_POINTER(tWceUiXmlAttr, Pre);
public:NO_WRITE_POINTER(tWceUiXmlAttr, Next);
}WceUiXmlAttr, *PWceUiXmlAttr;

class CWceUiXmlDoc
{
	friend class CWceUiXmlElem;
public:
	CWceUiXmlDoc(void);
	~CWceUiXmlDoc(void);

	BOOL IsLoaded();
	//BOOL SetXmlFilePath(LPCTSTR pFilePath);

	BOOL LoadXmlFile(LPCTSTR pFilePath, BOOL bLoadComment = FALSE);
	BOOL LoadXmlStr (LPCTSTR pXmlStr,   BOOL bLoadComment = FALSE);

	BOOL SaveXmlFile();
	BOOL SaveXmlFileAs(LPCTSTR pFilePath);

	PCWceUiXmlElem RootElement();

	//save xml file to pNewXmlString
	//if success, user should free pNewXmlString use delete[]
	//if failed,  pNewXmlString will be set to NULL
	BOOL SaveToString(LPTSTR *pNewXmlString);

	//save the pXmlElem to pNewXmlString
	//if success, user should free pNewXmlString use delete[]
	//if failed,  pNewXmlString will be set to NULL
	BOOL SaveElemToString(LPTSTR *pNewXmlString, const PCWceUiXmlElem pXmlElem);

	void Clean();

	//create a xml dom tree, 
	//pStrRootTag is the name of root tag
	//if success, return the root element
	//if failed, return NULL
	PCWceUiXmlElem NewXml(LPCTSTR pStrRootTag);

	//add xml declaration
	//<?xml version="1.0" encoding="unicode" standalone="yes" ?>
	BOOL InsertDeclaration();

private:
	//before parse xml tag, remove all useless characters from end.
	void TrimRight(LPTSTR pStrXml, int nLen);
	BOOL ParseXml(LPTSTR pStrXml);

	//the root element of xml dom tree, the kind must be XML_ELEM_NODE
	PCWceUiXmlElem	m_pRootElem;

	BOOL ParseXmlTag(LPTSTR pXmlTag, LPTSTR* pNext);

	PCWceUiXmlElem HandleXmlTagContent(LPCTSTR pStrTagName, LPTSTR pStrTagContent, BOOL bHaveAttr);

	void UnLoad();


	PCWceUiXmlElem GetNextNodeElem(PCWceUiXmlElem pElem);

	PCWceUiXmlElem GetPreNodeElem(PCWceUiXmlElem pElem);

	int GetSaveStringLen(); 

	int GetSaveElemLen(const PCWceUiXmlElem pXmlElem);

	BOOL IsStrNewed(LPCTSTR pStr);

	LPTSTR CopyString(LPTSTR pDstStr, LPCTSTR pSrcStr);

	LPTSTR m_pMemXmlStr;
	int m_nMemXmlStr; 
	BOOL m_bLoaded;
	TCHAR m_strPath[MAX_PATH];
	BOOL	m_bLoadComment;

private:
	//////////////////////////////////////////////////////////////////////////
	///////////////////////////#xml_extent_include ///////////////////////////
	//////////////////////////////////////////////////////////////////////////
	struct XmlSegment
	{
		LPCTSTR pXmlStr;
		int		nXmlStr;
	};
	struct IncludeParam
	{
		LPCTSTR pBeforeIncLine;		//文件的开头或者是行的开头
		LPCTSTR pAfterIncLine;		//文件的结尾,或者是行的结尾
		TCHAR	szRelativePath[MAX_PATH];
	};
	BOOL PrePareInclude();
	BOOL IsIncludeValid(LPCTSTR pInclude, IncludeParam* pIncludeParam);
	BOOL ReadIncludeFile(LPCTSTR pIncludeFilePath, LPTSTR* ppFileBuf, int* nBufLen);
	void GetCurPath(LPTSTR pCurPath);

	//////////////////////////////////////////////////////////////////////////
	////////                      new,delete                           ///////
	//////////////////////////////////////////////////////////////////////////
private:
	PCWceUiXmlElem NewElem(LPCTSTR pTagName);
	void DeleteElem(PCWceUiXmlElem pElem);


	//////////////////////////////////////////////////////////////////////////
	////////                      quick stack                           //////
	//////////////////////////////////////////////////////////////////////////
private:
	BOOL Push(PCWceUiXmlElem pElem, LPCTSTR pName);
	PWceUiXmlStack Pop();
	PWceUiXmlStack Top();
	BOOL IsStackEmpty();

	WceUiXmlStack m_pStack[XML_MAX_LEVEL];
	int			  m_nStack;

	//////////////////////////////////////////////////////////////////////////
	////////                     access order                          ///////
	//////////////////////////////////////////////////////////////////////////
private:
	CWceUiTypedHTList<PCWceUiXmlElem> m_AccessList;
	void Access_Remove(PCWceUiXmlElem pDelElem);
	PCWceUiXmlElem GetNoNameElem(PCWceUiXmlElem pElem);

private:
	CWceUiXmlDoc (const CWceUiXmlDoc  &);
	CWceUiXmlDoc & operator=(const CWceUiXmlDoc &);
};

class CWceUiXmlElem
{
	friend class CWceUiXmlDoc;
public:
	CWceUiXmlElem(CWceUiXmlDoc *pXmlDoc, WceUiXmlElemType type = XML_ELEM_NODE);
	~CWceUiXmlElem(void);

	PCWceUiXmlElem GetElemFromTree(LPCTSTR pStrTree);
	PCWceUiXmlElem GetChildElem(LPCTSTR pChildName);

	PCWceUiXmlElem GetParent();


	LPCTSTR GetName();

	int	GetChildCount();
	PCWceUiXmlElem FirstChild();
	PCWceUiXmlElem LastChild();
	PCWceUiXmlElem PreChild(PCWceUiXmlElem pCurElem);
	PCWceUiXmlElem NextChild(PCWceUiXmlElem pCurElem);
	PCWceUiXmlElem GetLeftBrother();
	PCWceUiXmlElem GetRightBrother();

	int GetAttrNameCount();
	const PWceUiXmlAttr GetFirstAttr();
	const PWceUiXmlAttr GetNextAttr(const PWceUiXmlAttr pCurAttr) const;
	
	CWceUiXmlDoc* GetXmlDoc();

public:
	//////////////////////////////////////////////////////////////////////////
	////                 add,delete node, add, delete attribute           ////
	//////////////////////////////////////////////////////////////////////////
	PCWceUiXmlElem InsertAfterChild(PCWceUiXmlElem pAfterThis, LPCTSTR pNewTagName);
	PCWceUiXmlElem InsertBeforeChild(PCWceUiXmlElem pBeforeThis, LPCTSTR pNewTagName);
	BOOL RemoveChild(PCWceUiXmlElem pChildElem);

	BOOL    SetAttr(LPCTSTR pAttrName, LPCTSTR pAttrValue);
	LPCTSTR GetAttr(LPCTSTR pAttrName);
	BOOL	RemoveAttr(LPCTSTR pAttrName);


	//////////////////////////////////////////////////////////////////////////
	////////////////////////        Comment      /////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void	SetComment(LPCTSTR pStrComment);
	void    RemoveComment();	//remove the comment if have.

private:
	BOOL SetAttribute(LPCTSTR pAttrName, LPCTSTR pAttrValue);

	int m_nLevel;	//level,from 0
	LPCTSTR m_pTagName;
	CWceUiTypedHTList<WceUiXmlAttr*> m_AttributeList;

	TCHAR *m_pTagBegin;
	TCHAR *m_pTagEnd;
	WceUiXmlElemType m_type;

	CWceUiXmlDoc *m_pXmlDoc;
	PCWceUiXmlElem m_pParent;

	//////////////////////////////////////////////////////////////////////////
	////                       access node link                         //////
	//////////////////////////////////////////////////////////////////////////
	NO_WRITE_POINTER(CWceUiXmlElem, NextAccess);
	NO_WRITE_POINTER(CWceUiXmlElem, PreAccess);

	//////////////////////////////////////////////////////////////////////////
	////                        child node link                         //////
	//////////////////////////////////////////////////////////////////////////
	CWceUiTypedHTList<PCWceUiXmlElem> m_ChildList;
	NO_WRITE_POINTER(CWceUiXmlElem, LeftBrother);
	NO_WRITE_POINTER(CWceUiXmlElem, RightBrother);
private:
	CWceUiXmlElem (const CWceUiXmlElem  &);
	CWceUiXmlElem & operator=(const CWceUiXmlElem &);
};
