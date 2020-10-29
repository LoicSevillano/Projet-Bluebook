
#pragma once
// By Jacques
#ifdef WIN32
#include <winsock2.h>
#endif
#include<stdlib.h>
#include <stdexcept>    
#include <string>
#include <list>
#include <map>
#ifdef WIN32
#include <WinSock2.h>
#endif
using namespace std;
class CXmlParserDocument;
class CAttribute
{
public:
    CAttribute();
    void SetName(wchar_t * szName);
    void SetName(std::wstring szName);
    std::wstring  *GetName();
    std::wstring  *GetValue();
    std::wstring GetRawValue();
    void SetValue(wchar_t * Val, bool bTransform = true);
    void SetValue(std::wstring Val, bool bTransform = true);
    void SetValue(int Val, bool bTransform = true);
    void SetValue(double Val, bool bTransform = true);
    std::wstring Name;
private:
    map<wchar_t, wstring> MapUTF8;
    std::wstring Value;
};

class CNode
{
private:
    CNode *pParent;
    std::list<CAttribute*> m_AttributeList;
    std::list<CNode*> m_ListOfChildren;
    CXmlParserDocument *m_pParserDocument;
public:
    ~CNode();
    CNode(CXmlParserDocument *pDoc);

    CXmlParserDocument *GetParserDocument();
    std::list<CAttribute*> * GetAttributeList();
    std::list<CNode*> * GetChildrenList();
    void SetName(wchar_t * NewName);
    void SetName(std::wstring NewName);
    std::wstring *GetName();
    CNode *first_node();
    CNode *first_node(wchar_t * SearchNodeName);
    CNode *first_node(wchar_t * SearchNodeName, std::list<CNode *>::iterator iterFirst);
    CNode * next_sibling();
    CAttribute * first_attribute(wchar_t * SearchAtttribute);
    std::wstring GetValue(wchar_t* AttributeName);
    CAttribute * pCurrentAttribute;

    int  GetValueInt(wchar_t* AttributeName);
    double GetValueDouble(wchar_t* AttributeName);
    std::wstring Name;
    CNode * GetParent();
    void SetParent(CNode *p);
    CAttribute * AddAttribute(wchar_t *AttributeName, wchar_t *AttributeValue, bool bTransform = true);
    CAttribute * AddAttribute(wchar_t *AttributeName, std::wstring AttributeValue, bool bTransform = true);
    CAttribute * AddAttribute(wchar_t *AttributeName, int AttributeValue, bool bTransform = true);
    CAttribute * AddAttribute(wchar_t *AttributeName, double AttributeValue, bool bTransform = true);
    CNode * AddNode(wchar_t * Node);
    CNode * AddNode(std::wstring  Node);
    CNode * AddFromNode(CNode *pAddedNode);
    void ConvertToText(std::wstring *Content);

};
class CXmlParserDocument
{
private:

    CNode *pRoot;
    int CurrentParseStatus;
    wstring CurrentEscapeSequence;

public:
    enum state
    {
        SEARCH_BALISE, SEARCH_VALUE, SEARCH_TYPE_OF_BALISE, WAIT_FOR_ATTRIBUTE_NAME, WAIT_FOR_VALUE_OF_ATTRIBUTE,
        WAIT_END_OF_NODE, WAIT_END_OF_NAME_NODE, WAIT_END_OF_COMMENT, END_OF_PARSE, WAIT_END_OF_ATTRIBUTE_VALUE, WAIT_END_OF_ESCAPE_IN_ATTRIBUTE_VALUE,
        WAIT_END_OF_ATTRIBUTE_NAME, WAIT_END_OF_ESCAPE_IN_ATTRIBUTE_NAME, WHAT_IS_AFTER_ATTRIBUTE_VALUE
    };

    CXmlParserDocument();

    ~CXmlParserDocument();
    static	wstring ChangeEscapeSequenceToChar(wstring sZ);
    wstring ChangeCharToEscapeSequence(wstring sZ);
    wstring Text;
    std::wstring Content;
    int Parse();
    CNode * first_node();
    CNode *AddNode(wchar_t *Node);
    CNode *AddNode(std::wstring Node);
    wstring ConvertToText();
    char *ConvertToChar();
    void clear();
    CNode *pCurrentNode;
    int TreatEscape(wstring::iterator * it, wstring *pTransform, wchar_t cEnd, int WaitEndOfEscape);
#ifdef WIN32
    int TransmitTCP(SOCKET sock);
    int ReceiveTCP(SOCKET sock);
#endif
    int CheckIfCompleted();
};
