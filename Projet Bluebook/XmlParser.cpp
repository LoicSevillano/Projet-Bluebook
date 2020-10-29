#include <stdlib.h>
#include <stdexcept>    
#include <string>
#include <list>
#include <string.h>
#include "Tools.h"
#include "XmlParser.h"
#include "stringutils.hpp"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef BOOL
typedef int BOOL;
#endif

CAttribute::CAttribute()
{
	MapUTF8['&'] = L"&#38;";
	MapUTF8['"'] = L"&#34;";
	MapUTF8['<'] = L"&#60;";
	MapUTF8['>'] = L"&#62;";
	MapUTF8[10]  = L"&#10;";
	MapUTF8[13]  = L"&#13;";
	MapUTF8['"'] = L"&#34;";
}

void CAttribute::SetName(wchar_t * szName)
{
	Name = szName;
}
void CAttribute::SetName(std::wstring szName)
{
	Name = szName;
}
std::wstring * CAttribute::GetName()
{
	return &Name;
}
std::wstring  * CAttribute::GetValue()
{
	return &Value;
}
void CAttribute::SetValue(wchar_t * Valw, bool bTransform)
{
	wstring Escape_characters = L"\"<>'°&";

	if (bTransform)
	{
		wstring AllTheUTF8;
		for (auto it = MapUTF8.begin(); it != MapUTF8.end(); it++) AllTheUTF8 += it->first;

		wstring Val = Valw;
		wstring ValRet;
		Value.clear();
		for (;;)
		{
			auto ifind = Val.find_first_of(AllTheUTF8);
			if (ifind == wstring::npos)break;
			wchar_t c = Val[ifind];
			ValRet += Val.substr(0, ifind) + MapUTF8[c];
			Val = Val.substr(ifind + 1);
		}
		ValRet += Val;
		Value = ValRet;
	}
	else
		Value = Valw;
}
void CAttribute::SetValue(std::wstring Val,bool bTransform)
{
	wstring ValRet;
	for (auto it = Val.begin(); it != Val.end(); it++)
	{
		if (!isascii(*it) || MapUTF8.count(*it))
		{
			ValRet += L"&#" + CTools::Format(L"%d", *it) + L";";
		}
		else
			ValRet += *it;
	}
/*	wstring Escape_characters = L"\"<>'°&";
	if (bTransform)
	{
		wstring AllTheUTF8;
		for (auto it = MapUTF8.begin(); it != MapUTF8.end(); it++) AllTheUTF8 += it->first;
		wstring ValRet;
		Value.clear();
		for (;;)
		{
			auto ifind = Val.find_first_of(AllTheUTF8);
			if (ifind == -1)break;
			wchar_t c = Val[ifind];
			ValRet += Val.substr(0, ifind) + MapUTF8[c];
			Val = Val.substr(ifind + 1);
		}
		ValRet += Val;
		Value = ValRet;
	}
	else*/
		Value = ValRet;
}
void CAttribute::SetValue(int Val,bool bTransform)
{
	wchar_t  pVal[255];
	swprintf( pVal,(size_t)255,L"%d", Val);
	Value=pVal;
}

void CAttribute::SetValue(double Val,bool bTransform)
{
	wchar_t pVal[255];
	swprintf(pVal,255,L"%f", Val);
	Value=pVal;
}

CNode::~CNode()
{
    std::list<CAttribute *>::iterator itbeginAt, itendAt, itAt;
	if (!m_AttributeList.empty())
	{
		itbeginAt = m_AttributeList.begin();
		itendAt = m_AttributeList.end();
		for (itAt = itbeginAt; itAt != itendAt; itAt++)
		{
			CAttribute *pAt = *itAt;
			delete pAt;
		}
		m_AttributeList.clear();
	}
    std::list<CNode *>::iterator  itChildren;
	while (!m_ListOfChildren.empty())
    {
		itChildren = m_ListOfChildren.begin();
        CNode *pNode = *itChildren;
		m_ListOfChildren.erase(itChildren);
        delete pNode;
		
    }
    m_ListOfChildren.clear();
}

void CNode::SetName(wchar_t * NewName)
{
	Name = NewName;
}
void CNode::SetName(std::wstring NewName)
{
	Name = NewName;
}
std::wstring* CNode::GetName()
{
	return &Name;
}
std::list<CAttribute*> * CNode::GetAttributeList()
{
	return &m_AttributeList;
}
std::list<CNode*> * CNode::GetChildrenList()
{
	return &m_ListOfChildren;
}

CNode * CNode::first_node()
{
	if (m_ListOfChildren.empty()) return NULL;
	std::list<CNode *>::iterator iter = m_ListOfChildren.begin();
	return *iter;
}
CNode * CNode::first_node(wchar_t * SearchNodeName)
{
	CNode * pChild;
	std::list<CNode *>::iterator iter;
	if (m_ListOfChildren.empty()) return NULL;
	for (iter = m_ListOfChildren.begin(); iter != m_ListOfChildren.end(); ++iter)
	{
		pChild = *iter;
		if (!wcscmp(pChild->Name.data(),SearchNodeName)) return pChild;
	}

	return NULL;
}
CNode *CNode::first_node(wchar_t * SearchNodeName, std::list<CNode *>::iterator iterFirst)
{
	CNode * pChild;
	std::list<CNode *>::iterator iter;
	if (m_ListOfChildren.empty()) return NULL;
	for (iter = iterFirst; iter != m_ListOfChildren.end(); ++iter)
	{
		pChild = *iter;
		if (pChild->Name == SearchNodeName) return pChild;
	}

	return NULL;
}

CNode * CNode::next_sibling()
{
	CNode *pParent = GetParent();
    if (pParent)
    {
                                
		std::list<CNode *>::iterator iter;
		for (iter=pParent->m_ListOfChildren.begin();iter!= pParent->m_ListOfChildren.end();iter++)
				if (*iter == this) break;
		if (iter != pParent->m_ListOfChildren.end())
		{
			iter++;
			return pParent->first_node((wchar_t *)this->Name.data(), iter);
		}
		else
			return NULL;
	}
    else
		return NULL;

}

CAttribute * CNode::first_attribute(wchar_t * SearchAtttribute)
{
    CAttribute* pAtt;
    std::list<CAttribute *>::iterator iter;
    if (this->GetAttributeList()->empty()) return NULL;
    for (iter = GetAttributeList()->begin(); iter != GetAttributeList()->end(); ++iter)
    {
		pAtt = *iter;
		if (!wcscmp(pAtt->GetName()->data(),SearchAtttribute)) return pAtt;
    }

    return NULL;
}



std::wstring CNode::GetValue(wchar_t* AttributeName)
{
	CAttribute *pAttribute = first_attribute(AttributeName);
	std::wstring ValueOfAttribute;
	if (pAttribute)
	{
		ValueOfAttribute =  *pAttribute->GetValue();
	}
	else
		ValueOfAttribute = L"";
	wstring szReturn;
	return  ValueOfAttribute;
}

 int  CNode::GetValueInt(wchar_t* AttributeName)
 {
	std::wstring s;
	int ValueOfAttribute;
	CAttribute *pAttribute = first_attribute(AttributeName);

	if (pAttribute)
	{
		s = GetValue(AttributeName);
		ValueOfAttribute = BEST_WSTRING2INT(s);
	}
	else
		ValueOfAttribute = 0;
	return ValueOfAttribute;
 }



 double CNode::GetValueDouble(wchar_t* AttributeName)
{
	std::wstring s;
	double ValueOfAttribute;
	CAttribute *pAttribute = first_attribute(AttributeName);

	if (pAttribute)
	{
		s = GetValue(AttributeName);
		ValueOfAttribute = BEST_WSTRING2DOUBLE(s);
	}
	else
		ValueOfAttribute = 0;
	return ValueOfAttribute;
}
CNode::CNode(CXmlParserDocument *pDoc)
{
	pParent = NULL;
	m_pParserDocument = pDoc;
	m_ListOfChildren.clear();
	pCurrentAttribute = NULL;
	Name.clear();
}
CXmlParserDocument *CNode::GetParserDocument()
{
	return m_pParserDocument;
}
CNode * CNode::GetParent()
{
	return pParent;
}
void CNode::SetParent(CNode *p)
{
	pParent = p;
}
CNode * CNode::AddFromNode(CNode *pAddedNode)
{
	CNode *pNew = AddNode(pAddedNode->GetName()->data());
//	*pNew = *pAddedNode;
	pNew->SetParent(this);
	for (auto it = pAddedNode->GetAttributeList()->begin(); it != pAddedNode->GetAttributeList()->end(); it++)
	{
		auto pNodeAttribute = *it;
		pNew->AddAttribute((wchar_t *)pNodeAttribute->GetName()->data(), pNodeAttribute->GetValue()->data(),false);
		
	}
	for (auto it = pAddedNode->GetChildrenList()->begin(); it != pAddedNode->GetChildrenList()->end(); it++)
	{
		pNew->AddFromNode(*it);
	}
	return pNew;
}
CAttribute * CNode::AddAttribute(wchar_t *AttributeName, wchar_t *AttributeValue, bool bTransform)
{
	CAttribute *pAttribute = new CAttribute;
	pAttribute->SetName(AttributeName);
	GetAttributeList()->push_back(pAttribute);
	pAttribute->SetValue(AttributeValue, bTransform);
	return pAttribute;
}
CAttribute * CNode::AddAttribute(wchar_t *AttributeName, std::wstring AttributeValue, bool bTransform)
{
	CAttribute *pAttribute = new CAttribute;
	pAttribute->SetName(AttributeName);
	GetAttributeList()->push_back(pAttribute);
	pAttribute->SetValue(AttributeValue,  bTransform);
	return pAttribute;
}
CAttribute * CNode::AddAttribute(wchar_t *AttributeName, int AttributeValue,bool bTransform)
{
	CAttribute *pAttribute = new CAttribute;
	pAttribute->SetName(AttributeName);
	GetAttributeList()->push_back(pAttribute);
	pAttribute->SetValue(AttributeValue,  bTransform);
	return pAttribute;
}

CAttribute * CNode::AddAttribute(wchar_t *AttributeName, double AttributeValue, bool bTransform)
{
	CAttribute *pAttribute = new CAttribute;
	pAttribute->SetName(AttributeName);
	GetAttributeList()->push_back(pAttribute);
	pAttribute->SetValue(AttributeValue,  bTransform);
	return pAttribute;
}

CNode * CNode::AddNode(wchar_t * Node)
{
	CNode *pNode = new CNode(GetParserDocument());
	pNode->Name = Node;
	pNode->SetParent(this);
	this->GetChildrenList()->push_back(pNode);
	return pNode;
}
CNode * CNode::AddNode(std::wstring  Node)
{
	CNode *pNode = new CNode(GetParserDocument());
	pNode->Name = Node;
	pNode->SetParent(this);
	this->GetChildrenList()->push_back(pNode);
	return pNode;
}
void CNode::ConvertToText(std::wstring *Content)
{
	*Content += L"<";
	*Content += Name;
	std::list<CAttribute *>::iterator itAttribute;
	for (itAttribute = GetAttributeList()->begin(); itAttribute != GetAttributeList()->end(); itAttribute++)
	{
		wchar_t c = '"';
		CAttribute * pAttribute = *itAttribute;
		*Content += L" ";
		*Content += *pAttribute->GetName();
		*Content += L"=";
		*Content += c;
		*Content += *pAttribute->GetValue();
		*Content += c;
	}
	*Content += L">";
	if (GetChildrenList()->empty())
	{
		*Content += L"</" + Name + L">\n";
	}
	else
	{
		*Content += L"\n";
		std::list<CNode *>::iterator itNode;
		for (itNode = GetChildrenList()->begin(); itNode != GetChildrenList()->end(); itNode++)
		{
			CNode * pNode = *itNode;
			pNode->ConvertToText(Content);
		}
		*Content += L"\n</";
		*Content += Name;
		*Content +=L">\n";

	}
}

char * CXmlParserDocument::ConvertToChar()
{
	wstring sZ = ConvertToText();
	char *sReturn = CTools::wCharToChar(sZ.data());
	return sReturn;
}


CXmlParserDocument::CXmlParserDocument()
{
	pRoot = new CNode(this);
	Text.clear();
	clear();
}
CXmlParserDocument::~CXmlParserDocument()
{
//	clear();
	delete pRoot;
}

void CXmlParserDocument::clear()
{
	if (pRoot)
	{
		delete pRoot;
		pRoot = new CNode(this);
		CurrentParseStatus = SEARCH_BALISE;
		pCurrentNode = pRoot;
	}
}
int CXmlParserDocument::Parse()
{
	std::wstring Check;
	pRoot->Name = L"Root";
	wstring::iterator itText = Text.begin();
	int iEndEscape;
	while (itText!=Text.end())
	{
		switch (CurrentParseStatus)
		{
			case SEARCH_BALISE:
			while (*itText == 9 || *itText == '\n' || *itText == '\r')
			{
				if (itText == Text.end()) break;
					itText++;
				if (itText == Text.end()) break;
			}
			
			if (itText == Text.end())
			{
				if (pCurrentNode == pRoot) CurrentParseStatus = END_OF_PARSE;
				break;
			}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (*itText != '<')
			{
				auto iEr = std::runtime_error("Balise Manquante");
				return 0;
			}
			CurrentParseStatus = SEARCH_TYPE_OF_BALISE;
			itText++;
			break;
			case WAIT_END_OF_COMMENT :
			CurrentParseStatus = WAIT_END_OF_COMMENT;

			while (*itText != '>' && itText != Text.end()) itText++;

			if (itText == Text.end()) break;
			itText++;
			CurrentParseStatus = SEARCH_BALISE;
			break;
			case SEARCH_TYPE_OF_BALISE:
			if (*itText == '!')
			{
				CurrentParseStatus = WAIT_END_OF_COMMENT;
				itText++;
				break;
			}

			else if (*itText == '/')
			{
				if (!pCurrentNode)
				{
					return 0;
//					MessageBox(NULL, L"No Current node", L"Error", MB_OK);
				}
				CurrentParseStatus = WAIT_END_OF_NODE;
				itText++;
				break;
			}
			else
			{
				//                      Il s'agit d'un Node
				CNode * pNode = new CNode(this);
				pNode->SetParent(pCurrentNode);
				pCurrentNode->GetChildrenList()->push_back(pNode);
				pCurrentNode = pNode;
				Check.clear();
				pCurrentNode->Name.clear();
				CurrentParseStatus = WAIT_END_OF_NAME_NODE;
				break;
			}
			break;
			case  WAIT_END_OF_NODE :
			Check.clear();
			if (!pCurrentNode)
			{
#ifdef WIN32
					MessageBox(NULL, L"End Of node without start", L"Error", MB_OK);
#else
					BEST_WARNING("End Of node without start");
#endif
				return 1;
			}

			while (itText != Text.end() && *itText != '>')
			{
				Check += *itText;
				itText++;
			}
			if (itText == Text.end()) break;
			if (Check != pCurrentNode->Name)
			{
//                                              std::runtime_error("Unknown name"); 
//                                              return 0;
			}
			pCurrentNode = pCurrentNode->GetParent();
			itText++;
			CurrentParseStatus = SEARCH_BALISE;

			break;
			///////////////////////////////////////////////////////////////////////////////////////////
			case WAIT_FOR_ATTRIBUTE_NAME:
			case WAIT_END_OF_ATTRIBUTE_NAME:
			case WAIT_END_OF_ESCAPE_IN_ATTRIBUTE_NAME:
			{
				if (CurrentParseStatus == WAIT_FOR_ATTRIBUTE_NAME) pCurrentNode->pCurrentAttribute = new CAttribute;

				iEndEscape = TreatEscape(&itText, pCurrentNode->pCurrentAttribute->GetName(), '=', CurrentParseStatus == WAIT_END_OF_ESCAPE_IN_ATTRIBUTE_NAME);
				if (iEndEscape)
				{
					CurrentParseStatus = WAIT_END_OF_ATTRIBUTE_NAME;
					if (iEndEscape == 2) CurrentParseStatus = WAIT_END_OF_ESCAPE_IN_ATTRIBUTE_NAME;
					break;
				}
				/*         Vérifier que le nom est valide sans <>                 */
//				int iFind = pCurrentNode->pCurrentAttribute->GetName()->find(L"Numero");
				pCurrentNode->GetAttributeList()->push_back(pCurrentNode->pCurrentAttribute);
				CurrentParseStatus = WAIT_FOR_VALUE_OF_ATTRIBUTE;

				itText++  ;
				break;
			}
			case WAIT_END_OF_NAME_NODE :
			while (itText != Text.end() && *itText != ' ' && *itText != '>')
			{
				*pCurrentNode->GetName() += *itText++;
			}
			if (itText == Text.end()) break;

			if (*itText == ' ') CurrentParseStatus = WAIT_FOR_ATTRIBUTE_NAME;
			else if (*itText == '>') CurrentParseStatus = SEARCH_BALISE;
			else
			{
				auto iError = std::runtime_error("illegal caracter");
			}
			itText++;

			break;
			case WAIT_FOR_VALUE_OF_ATTRIBUTE :
			if (*itText != '"')
			{
				auto iError = std::runtime_error("Unknown value of attribute");
			}
			itText++;
			case WAIT_END_OF_ATTRIBUTE_VALUE:
			case WAIT_END_OF_ESCAPE_IN_ATTRIBUTE_VALUE:
			iEndEscape = TreatEscape(&itText, pCurrentNode->pCurrentAttribute->GetValue(), '"', CurrentParseStatus == WAIT_END_OF_ATTRIBUTE_VALUE);
			if (iEndEscape)
			{
				
				CurrentParseStatus = WAIT_END_OF_ATTRIBUTE_VALUE;
				if (iEndEscape == 2) CurrentParseStatus = WAIT_END_OF_ESCAPE_IN_ATTRIBUTE_VALUE;
				break;
			}
			pCurrentNode->pCurrentAttribute->GetValue()->find(L"3096");
			if (itText != Text.end()) itText++;
			CurrentParseStatus = WHAT_IS_AFTER_ATTRIBUTE_VALUE;
			break;
			case WHAT_IS_AFTER_ATTRIBUTE_VALUE:
			if (itText == Text.end()) break;

			if (*itText == ' ')
			{
				CurrentParseStatus = WAIT_FOR_ATTRIBUTE_NAME;
				itText++;
			}
			else if (*itText == '>')
			{
				CurrentParseStatus = SEARCH_BALISE;
				itText++;
			}
			else if (*itText == '/')
			{
				if (!pCurrentNode)
				{
#ifdef WIN32
					MessageBox(NULL, L"No Current node", L"Error", MB_OK);
#else
					BEST_WARNING("No Current node");
#endif
				}

				CurrentParseStatus = WAIT_END_OF_NODE;
				itText++;
			}
			else
			{
				auto iEr = std::runtime_error("error after attribute value");
			}
			break;
		}
		if (itText == Text.end()) break;
	}
//	Text = TextIn;
	if (pCurrentNode == pRoot) CurrentParseStatus = END_OF_PARSE;
	return CurrentParseStatus;
}
CNode * CXmlParserDocument::first_node()
{
	return pRoot;
}
CNode *CXmlParserDocument::AddNode(wchar_t *Node)
{
	CNode *pNode = new CNode(this);
	pNode->Name = Node;
	pNode->SetParent(pRoot);
	pRoot->GetChildrenList()->push_back(pNode);
	return pNode;
}
CNode *CXmlParserDocument::AddNode(std::wstring Node)
{
	CNode *pNode = new CNode(this);
	pNode->Name = Node;
	pNode->SetParent(pRoot);
	pRoot->GetChildrenList()->push_back(pNode);
	return pNode;
}

wstring CXmlParserDocument::ConvertToText()
{
	std::wstring Content;
	Content.clear();
	std::list<CNode *>::iterator iter;
	for (iter = pRoot->GetChildrenList()->begin(); iter != pRoot->GetChildrenList()->end(); iter++)
	{
					CNode *pNode = *iter;
					pNode->ConvertToText(&Content);
	}
	return Content;
}

int CXmlParserDocument::TreatEscape(wstring::iterator * it, wstring *pTransform, wchar_t cEnd,int EndOfEscape)
{
	wstring::iterator itText = *it;
	BOOL bEscape = FALSE;
	if (EndOfEscape)
	{
		bEscape = TRUE;
	}
	else
	{
		CurrentEscapeSequence.empty();
	}
//	pTransform->clear();
	int iReturn = 0;
	for (;;)
	{
		if (itText == Text.end())
		{
			iReturn = 1;
			if (bEscape) iReturn = 2;
			break;
		}
		if (*itText != cEnd)
		{
			if (bEscape)
			{
				if (*itText == ';')
				{
					bEscape = FALSE;
					wchar_t cEsc = BEST_WSTRING2INT(CurrentEscapeSequence.data());
					*pTransform += cEsc;
					itText++;
				}
				else
				{
					CurrentEscapeSequence += *itText;
					itText++;
				}
			}
			else
			{
				if (*itText == '#')
				{
					if (!pTransform->empty())
					{
						if ((*pTransform)[pTransform->length() - 1] == '&')
						{
							bEscape = TRUE;
							pTransform->pop_back();
							CurrentEscapeSequence.clear();
							itText++;
						}
						else
						{
							*pTransform += *itText++;
						}
					}
					else
					{
						*pTransform += *itText++;
					}
				}
				else
				{
					*pTransform += *itText++;
				}
			}
		}
		else
		{
			iReturn = 0;
			break;
		}
	}
	*it = itText;
	return iReturn;
}



int CXmlParserDocument::CheckIfCompleted()
{
	wstring TextToAnalize = Text;
	size_t pos = 0;
	int nTotalOpen = 0;
	while ((pos = TextToAnalize.find('<')) != wstring::npos)
	{
		nTotalOpen++;
		TextToAnalize.erase(0, pos + 1);

	}

	TextToAnalize = Text;
	pos = 0;
	int nTotalClose = 0;
	while ((pos = TextToAnalize.find('>')) != wstring::npos)
	{
		nTotalClose++;
		TextToAnalize.erase(0, pos + 1);

	}

	TextToAnalize = Text;
	pos = 0;
	int nTotalClosedNode = 0;
	while ((pos = TextToAnalize.find(L"</")) != wstring::npos)
	{
		nTotalClosedNode++;
		TextToAnalize.erase(0, pos + 1);

	}

	int nRealyOpen = nTotalOpen - nTotalClosedNode;


	return nRealyOpen - nTotalClosedNode;
}
