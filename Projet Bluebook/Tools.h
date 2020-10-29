#pragma once
#include <string>
#include "MacroCommandeStructure.h"
using namespace std;
class CTools
{
public:
    CTools();
    ~CTools();
    static char *wCharToChar(wchar_t *pwBuf);
    static wchar_t* charToWchar(char * charText);
    static char *wCharToChar(std::wstring pwBuf);
    static wchar_t* charToWchar(const char * charText);
    static wstring charToWstring(const char * charText);
    static wstring charToWstring(char * charText);
    static bool CompareNoCase(std::wstring pwBuf1, std::wstring pwBuf2);
    static bool CompareNoCase(std::wstring pwBuf1, wchar_t* pwBuf2);
    static wstring MakeUpper(wstring sZ);

    static wstring ShortNameFromPathName(wstring PathName);
    static wstring Format(wstring szFormat,...);
    static string FormatA(string szFormat, ...);
    static bool IsNumeric(wstring sZ);
    static bool IsInteger(wchar_t c);
    static sMacroCommand CryptMacroCommand(const char * szMacro);
    static wstring TrimLeft(wstring sZ);
    static wstring TrimRight(wstring sZ);
    static wstring NoBlank(wstring sZ);

    static wstring OpenTextFile(char * szPath);
	static wstring GetCurrentDllPath();
};

