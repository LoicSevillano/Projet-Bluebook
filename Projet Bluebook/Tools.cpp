#include "Tools.h"
#ifdef WIN32
#include <Windows.h>
#include <strsafe.h>
//#include <atlbase.h>
#else
#include <iconv.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#define _vsnwprintf vswprintf
#endif
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstdarg>

#if __cplusplus < 201703L
#include <memory>
#endif
using namespace std;
CTools::CTools()
{
}


CTools::~CTools()
{
}

wchar_t* CTools::charToWchar(char * charText)
{
    const size_t cSize = strlen(charText) + 1;
    wchar_t* Text = new wchar_t[cSize];
#ifdef WIN32
    MultiByteToWideChar(CP_ACP, NULL, charText, -1, Text, (int) strlen(charText) + 1);
#else
    mbstowcs(Text, charText, cSize);
#endif
    return Text;
}

wchar_t* CTools::charToWchar(const char * charText)
{
    const size_t cSize = strlen(charText) + 1;
    wchar_t* Text = new wchar_t[cSize];
#ifdef WIN32
    MultiByteToWideChar(CP_ACP, NULL, charText, -1, Text, (int)strlen(charText) + 1);
#else
    mbstowcs(Text, charText, cSize);
#endif
    return Text;

}


wstring CTools::MakeUpper(wstring sZ)
{
    wstring Upr;
    Upr = sZ;
    transform(Upr.begin(), Upr.end(), Upr.begin(), towupper);
    return Upr;
}

wstring CTools::TrimLeft(wstring sZ)
{
    wstring szReturn;
    int k = (int) sZ.find_first_not_of(' ');
    if (k == -1) return L"";
    return sZ.substr(k);
}
wstring CTools::TrimRight(wstring sZ)
{
    wstring szReturn;
    int k = (int) sZ.find_first_of(' ');
    if (k == -1) return sZ;
    return sZ.substr(0, k);

}
wstring CTools::NoBlank(wstring sZ)
{
    wstring szReturn;
    for (auto it = sZ.begin(); it!=sZ.end(); it++)
        if (*it != ' ') szReturn += *it;
    return szReturn;
}
wstring CTools::charToWstring(const char * charText)
{
    
    wchar_t *Text = new wchar_t[strlen(charText) + 1];
#ifdef WIN32
	size_t nConv;
    mbstowcs_s(&nConv, Text, strlen(charText) + 1, charText, strlen(charText) + 1);
#else
    mbstowcs(Text, charText, strlen(charText) + 1);
#endif
    wstring wReturn = Text;
    delete []Text;
    return wReturn;
}

wstring CTools::charToWstring(char * charText)
{
    
    wchar_t *Text = new wchar_t[strlen(charText) + 1];
#ifdef WIN32
	size_t nConv;
    mbstowcs_s(&nConv, Text, strlen(charText) + 1, charText, strlen(charText) + 1);
#else
    mbstowcs(Text, charText, strlen(charText) + 1);
#endif
    wstring wReturn = Text;
    delete[] Text;
    return wReturn;
}

char *CTools::wCharToChar(wchar_t *pwBuf)
{
    int n = (int)wcslen(pwBuf);
    char * pBuf = new char[n + 1];
#ifdef WIN32
	size_t nConv;
    wcstombs_s(&nConv, pBuf, n + 1, pwBuf, n + 1);
#else
    wcstombs(pBuf, pwBuf, n + 1);
#endif
    return pBuf;
}
char *CTools::wCharToChar(std::wstring pwBuf)
{
    int n = (int)pwBuf.length();
    char * pBuf = new char[n + 1];
#ifdef WIN32
	size_t nConv;
    wcstombs_s(&nConv, pBuf, n + 1, pwBuf.data(), n + 1);
#else
    wcstombs(pBuf, pwBuf.data(), n + 1);
#endif
    return pBuf;
}

bool CTools::CompareNoCase(std::wstring pwBuf1, std::wstring pwBuf2)

{
    if (pwBuf1.length() != pwBuf2.length()) return true;
    for (int i = 0; i < (int)pwBuf1.length(); i++)
    {
        if (toupper(pwBuf1[i]) != toupper(pwBuf2[i])) return true;
    }
    return false;
}
bool CTools::CompareNoCase(std::wstring pwBuf1, wchar_t* pwwBuf2)
{
    std::wstring pwBuf2 = pwwBuf2;
    if (pwBuf1.length() != pwBuf2.length()) return true;
    for (int i = 0; i < (int)pwBuf1.length(); i++)
    {
        if (toupper(pwBuf1[i]) != toupper(pwBuf2[i])) return true;
    }
    return false;
}

wstring CTools::ShortNameFromPathName(wstring PathName)
{
    int First = -1;
    wstring Result;
    int j;
	bool isfound = false;
    for (int i = 0; i < (int)PathName.length(); i++)
    {
        j = (int)PathName.length() - i - 1;
		if (PathName[j] == '\\' || PathName[j] == '/')
		{
			isfound = true;
			break;
		}
    }
	if (isfound) First = j + 1;
	else         First = 0;
    Result = PathName.substr(First);
    j = (int)Result.find('.');
    if (j != -1)
        Result = Result.substr(0, j);
    return Result;
}

string CTools::FormatA(string szFormat, ...)
{

    va_list arglist;
    char szBuf[1024];

    va_start(arglist, szFormat);
    vsnprintf(szBuf, 1024, szFormat.data(), arglist);
    va_end(arglist);
    string szResult = szBuf;

    return szResult;
}


wstring CTools::Format(wstring szFormat, ...)
{
    va_list arglist;
    wchar_t szBuf[1024];

    va_start(arglist, szFormat);
    _vsnwprintf(szBuf, 1024, szFormat.data(), arglist);
    va_end(arglist);
    wstring szResult = szBuf;

    return szResult;
}

std::string Format(const char *fmt, ...)
{
    char buf[256];

    va_list args;
    va_start(args, fmt);
    const auto r = vsnprintf(buf, sizeof buf, fmt, args);
    va_end(args);

    if (r < 0)
        // conversion failed
        return{};

    const size_t len = r;
    if (len < sizeof buf)
        // we fit in the buffer
        return{ buf, len };

#if __cplusplus >= 201703L
    // C++17: Create a string and write to its underlying array
    std::string s(len, '\0');
    va_start(args, fmt);
    std::vsnprintf(s.data(), len + 1, fmt, args);
    va_end(args);

    return s;
#else
    // C++11 or C++14: We need to allocate scratch memory
    auto vbuf = std::unique_ptr<char[]>(new char[len + 1]);
    va_start(args, fmt);
    std::vsnprintf(vbuf.get(), len + 1, fmt, args);
    va_end(args);

    return{ vbuf.get(), len };
#endif
}
bool CTools::IsInteger(wchar_t c)
{
    wstring szIntegers = L"0123456789";
    if (szIntegers.find_last_of(c) == wstring::npos) return false;
    return true;
}
bool CTools::IsNumeric(wstring Text)
{
    size_t iCar;
    wchar_t CarToChecK;
    bool  bExponent = false;
    bool  bChangeSignExponent = false;
    bool  bDecimalPoint = false;
    bool  bReturn = true;

    for (iCar = 0; iCar < Text.length(); iCar++)
    {
        CarToChecK = Text[iCar];
        if (iCar == 0)
        {
            switch (CarToChecK)
            {
            case '.':
                bDecimalPoint = true;
                break;
            case '-':
                break;
            case '+':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            default:
                bReturn = false;
                break;
            }
        }

        else if (iCar == Text.length() - 1)
        {
            switch (CarToChecK)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            case '.':
                bDecimalPoint = true;
                break;

            case 'e':
            case 'E':

                if (!bExponent)
                {
                    bExponent = true;
                    break;
                }
            default:
                bReturn = false;
                break;
            }
        }
        else
        {
            switch (CarToChecK)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            case '.':
                if (bDecimalPoint)
                {
                    bReturn = false;
                    break;
                }
                bDecimalPoint = true;
                break;
            case '-':
            case '+':
                if (Text[iCar - 1] == 'E' || Text[iCar - 1] == 'e')
                {
                    if (!bChangeSignExponent)
                    {
                        bChangeSignExponent = true;
                        break;
                    }
                }
                bReturn = false;
                break;
            case 'e':
            case 'E':
                if (!bExponent)
                {
                    bExponent = true;
                    break;
                }
                bReturn = false;
                break;
            default:
                bReturn = false;
            }
        }
        if (!bReturn) break;
    }

    return bReturn;
}
/*sMacroCommand CTools::CryptMacroCommand(char * szMacro)
{
    wstring sZMacro = CTools::charToWstring(szMacro);
    auto iBracket = sZMacro.find(L"[");
    sZMacro = sZMacro.substr(iBracket + 1);
    iBracket = sZMacro.find(L"]");
    sZMacro = sZMacro.substr(0, iBracket);

    sMacroCommand MacroCommand;
    auto iComma = sZMacro.find(L",");
    if (iComma == -1) iComma = sZMacro.length();
    MacroCommand.m_MacroTitle = sZMacro.substr(0, iComma);
    if (iComma + 1 < sZMacro.length()) sZMacro = sZMacro.substr(iComma + 1);
    else
        sZMacro.clear();
    while (!sZMacro.empty())
    {
        iComma = sZMacro.find(L",");
        if (iComma == -1) iComma = sZMacro.length();
        auto iEqual = sZMacro.find(L"=");
        if (iEqual == -1) iEqual = iComma;
        MacroCommand.m_MapOfAttributes[sZMacro.substr(0, iEqual)] = sZMacro.substr(iEqual + 1, iComma - iEqual - 1);
        if (iComma + 1 < sZMacro.length()) sZMacro = sZMacro.substr(iComma + 1);
        else
            sZMacro.clear();
    }
    return MacroCommand;
}*/
sMacroCommand CTools::CryptMacroCommand(const char * szMacro_i)
{
    enum Wait_Stait { WAIT_EQUAL_OR_PARENTHESIS, WAIT_END_PARENTHESIS_VALUE_NEXT_COMMA, WAIT_END_PARENTHESIS_VALUE_NEXT_EQUAL ,WAIT_COMMA_OR_PARENTHESIS };
    sMacroCommand MacroCommand;

    wstring KeyWord,KeyValue;
    wstring sZMacro = CTools::charToWstring(szMacro_i);
    auto iBracket = sZMacro.find('[');
    sZMacro = sZMacro.substr(iBracket + 1);
    iBracket = sZMacro.find(L"]");
    sZMacro = sZMacro.substr(0, iBracket);
    auto itText = sZMacro.data();
    while (*itText != ',')
    {
        MacroCommand.m_MacroTitle += *itText;
        itText++;
        if (*itText == 0) break;
    }


    if (*itText) itText++;
    int WaitingWhat = WAIT_EQUAL_OR_PARENTHESIS;
    KeyWord.clear();
    KeyValue.clear();
    for (;;)
    {
        if (*itText == 0) break;
        switch (WaitingWhat)
        {
            case WAIT_EQUAL_OR_PARENTHESIS:
                while (*itText != '=' && *itText !='(')
                {
                    KeyWord += *itText;
                    itText++;
                    if (*itText == 0) break;
                }
                if (*itText == '(')
                {
                    WaitingWhat = WAIT_END_PARENTHESIS_VALUE_NEXT_EQUAL;
                    KeyWord += *itText;
                    itText++;
                }
                else
                {
                    WaitingWhat = WAIT_COMMA_OR_PARENTHESIS;
                    if (*itText) itText++;
                }
            break;
            case WAIT_COMMA_OR_PARENTHESIS :
                KeyValue.clear();
                while (*itText != ',' && *itText != '(')
                {
                    KeyValue += *itText;
                    itText++;
                    if (*itText == 0) break;
                }
                if (*itText == '(')
                {
                    WaitingWhat = WAIT_END_PARENTHESIS_VALUE_NEXT_COMMA;
                    itText++;
                }
                else
                {
                    MacroCommand.m_MapOfAttributes[KeyWord] = KeyValue;
                    if (*itText) itText++;
                    KeyWord.clear();
                    KeyValue.clear();
                    WaitingWhat = WAIT_EQUAL_OR_PARENTHESIS;
                }
                break;
            case WAIT_END_PARENTHESIS_VALUE_NEXT_EQUAL:
                while (*itText != ')')
                {
                    KeyWord += *itText;
                    itText++;
                }

                KeyWord += *itText++;
                WaitingWhat = WAIT_EQUAL_OR_PARENTHESIS;
                break;
            case WAIT_END_PARENTHESIS_VALUE_NEXT_COMMA :
                while (*itText != ')')
                {
                    KeyValue += *itText;
                    itText++;
                    if (*itText == 0) break;
                }
                itText++;
                WaitingWhat = WAIT_COMMA_OR_PARENTHESIS;
                break;
    
        }
        if (!*itText) break;
    }
    return MacroCommand;
}
wstring CTools::OpenTextFile(char * szPath)
{
    FILE *ptr;
    wstring szReturn;
    ptr = fopen(szPath, "rt");
    fseek(ptr, 0, SEEK_END);
    auto nBytes = ftell(ptr);

    fseek(ptr, 0, 0);
    char * szRead = new char[nBytes + 1];
    auto nRead = fread(szRead, sizeof(char), nBytes, ptr);
    szRead[nRead] = 0;
    wchar_t *szwRead = CTools::charToWchar(szRead);
    szReturn = szwRead;

    delete[] szRead;
    delete[] szwRead;
    fclose(ptr);
    return szReturn;

}

wstring CTools::GetCurrentDllPath()
{
/*#ifdef WIN32
	LPTSTR  strDLLPath = new TCHAR[_MAX_PATH];
	::GetModuleFileName((HINSTANCE)&__ImageBase, strDLLPath, _MAX_PATH);
	wstring szExecName = wstring(strDLLPath);

	wstring szExecNameLow = szExecName;

	size_t k = szExecNameLow.find(L".dll");
	while (szExecNameLow.substr(k, 1) != L"\\" && k > 1) k--;

	szExecName = szExecName.substr(0, k) + L"\\";
	return (szExecName);
#else
	return (L"");
#endif
*/
    return (L"");

}