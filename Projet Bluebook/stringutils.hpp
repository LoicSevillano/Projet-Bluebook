#ifndef UTILS_STRINGUTILS_HPP_
#define UTILS_STRINGUTILS_HPP_
////////////////////////////////////////////////////////////////////////////////////////////////////
///
///  Created by Arthur Moncorge on April, 2009.
///  Copyright 2009. All rights reserved.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <cstring>
#include <sstream>
#include <bitset>
#include <string>
#include <cctype>
#include "vectorutils.hpp"
#include "lint.h" // for lint
#include <iomanip>
#include <algorithm>
#include "macro.h"

#ifdef _WIN32
#pragma warning(disable:4996)
#endif

static long long BEST_ERROR_CODE = -999999999999999;

typedef std::string String;

typedef const Vector<String> VecString_I;
typedef Vector<String> VecString, VecString_O, VecString_IO;

inline bool BEST_STRCMP(String a, String b)
{
    return (a==b);
}

inline String BEST_SUBSTRING(String a, int beg, int length)
{
    return a.substr(beg,length);
}

//inline void BEST_TRIMSTRING(String str)
//{
//    std::string::size_type pos = str.find_last_not_of(' ');
//    if(pos != std::string::npos)
//    {
//        str.erase(pos + 1);
//        pos = str.find_first_not_of(' ');
//        if (pos != std::string::npos) str.erase(0, pos);
//    }
//    else
//    {
//        str.erase(str.begin(), str.end());
//    }
//}

inline std::wstring BEST_STRING2WSTRING(const std::string s)
{
	std::wstring ws2 = L"";
	if (s.size() > 0)
	{
		std::wstring ws(s.size(), L' '); // Overestimate number of code points.
		size_t ierror = mbstowcs(&ws[0], s.c_str(), s.size());
		if (ierror != wstring::npos) ws.resize(ierror); // Shrink to fit.
		ws2 = ws;
	}
    return ws2;
}

inline std::string BEST_WSTRING2STRING(const std::wstring s)
{
	std::string str="";
	if (s.length() > 0)
	{
		size_t nConv = s.length() + 1;
		char * pBuf = new char[nConv];
#ifdef WIN32
		wcstombs_s(&nConv, pBuf, nConv, s.data(), nConv);
#else
		wcstombs(pBuf, s.data(), nConv);
#endif
		str = pBuf;
		delete[] pBuf;
	}

	return str;
}

inline String BEST_INT2STRING(int i)
{
    std::ostringstream sout;
    sout << i;
    return sout.str();
}

inline std::wstring BEST_INT2WSTRING(int i)
{
    std::ostringstream sout;
    sout << i;
    return BEST_STRING2WSTRING(sout.str());
}

inline String BEST_INT2STRING(lint i)
{
    std::ostringstream sout;
    sout << i;
    return sout.str();
}

inline std::wstring  BEST_INT2WSTRING(lint i)
{
    std::ostringstream sout;
    sout << i;
    return BEST_STRING2WSTRING(sout.str());
}

inline String BEST_FLOAT2STRING(float f)
{
    std::ostringstream sout;
    sout << f;
    return sout.str();
}

inline std::wstring BEST_FLOAT2WSTRING(float f)
{
    std::ostringstream sout;
    sout << f;
    return BEST_STRING2WSTRING(sout.str());
}

inline String BEST_FLOAT2STRING(float f, int decimal)
{
    std::ostringstream sout;
    sout << std::fixed << std::setprecision(decimal) << f;
    return sout.str();
}

inline std::wstring BEST_FLOAT2WSTRING(float f, int decimal)
{
    std::ostringstream sout;
    sout << std::fixed << std::setprecision(decimal) << f;
    return BEST_STRING2WSTRING(sout.str());
}

inline String BEST_DOUBLE2STRING(double d)
{
    std::ostringstream sout;
    sout << d;
    return sout.str();
}

inline std::wstring BEST_DOUBLE2WSTRING(double d)
{
    std::ostringstream sout;
    sout << d;
    return BEST_STRING2WSTRING(sout.str());
}

inline String BEST_DOUBLE2STRING(double d, int decimal)
{
    std::ostringstream sout;
    sout << std::fixed << std::setprecision(decimal) << d;
    return sout.str();
}

inline std::wstring BEST_DOUBLE2WSTRING(double d, int decimal)
{
    std::ostringstream sout;
    sout << std::fixed << std::setprecision(decimal) << d;
    return BEST_STRING2WSTRING(sout.str());
}

inline int BEST_STRING2INT(String is)
{
	int i = 0;;
    std::stringstream iss(is);
    if ((iss>>i).fail())
    {
		i = (int) BEST_ERROR_CODE;
    }

    return i;
}

inline long BEST_STRING2LONG(String is)
{
	long i = 0;
	std::stringstream iss(is);
	if ((iss >> i).fail())
	{
		i = (long) BEST_ERROR_CODE;
	}

	return i;
}

inline float BEST_STRING2FLOAT(String ds)
{
    float i;
    std::stringstream iss(ds);
    if ((iss>>i).fail())
    {
		i = (float) BEST_ERROR_CODE;
    }
    return i;
}

inline double BEST_STRING2DOUBLE(String ds)
{
    double i;
    std::stringstream iss(ds);
    if ((iss>>i).fail())
    {
		i = (double)BEST_ERROR_CODE;
    }

    return i;
}

inline int BEST_WSTRING2INT(std::wstring ws)
{
	return BEST_STRING2INT(BEST_WSTRING2STRING(ws));
}

inline long BEST_WSTRING2LONG(std::wstring ws)
{
	return BEST_STRING2LONG(BEST_WSTRING2STRING(ws));
}

inline double BEST_WSTRING2DOUBLE(std::wstring ws)
{
	return BEST_STRING2DOUBLE(BEST_WSTRING2STRING(ws));
}

inline void BEST_CHARCOPY(char* target, int size_target, const char* source, int size_source)
{
    memset(target,' ',size_target*sizeof(char));
    memcpy(target,source,size_source*sizeof(char));
}

inline void BEST_REPLACE_ALL(String& origin, const String& match,  const String& replaceBy)
{
    while (true)
    {
        lint pos = origin.find(match);
        if (pos==(lint) String::npos) break;
        origin.replace(pos,match.size(),replaceBy);
    }
}

inline void BEST_SPLIT_NO_BOOST(const std::string& origin, const std::string& delimiters, Vector<std::string>& elems, bool clean=true)
{
    elems.clear();
    const char* delimiter = delimiters.c_str();
    std::bitset<255> delims;
    //for (int i=0; i<255; i++) delims[i] = false; // RdL: don't know if explicit intialization is needed...
    while ( *delimiter )
    {
        unsigned char code = *delimiter++;
        delims[code] = true;
    }
    typedef std::string::const_iterator iter;
    iter beg;
    if (clean)
    {
        bool in_token = false;
        for (std::string::const_iterator it = origin.begin(), end = origin.end(); it != end; ++it )
        {
            if ( delims[*it] )
            {
                if ( in_token )
                {
                    elems.push_back(std::string(beg,it));
                    in_token = false;
                }
            }
            else if ( !in_token )
            {
                beg = it;
                in_token = true;
            }
        }
        if ( in_token )
        {
            elems.push_back(std::string(beg, origin.end()));
        }
    }
    else
    {
        size_t found, token = 0;
        found = origin.find_first_of(delimiters);
        while (found != std::string::npos)
        {
            elems.push_back( origin.substr(token,found-token));
            token = found+1;
            found = origin.find_first_of(delimiters,token);
        }
        elems.push_back(origin.substr(token,found-token));
    }
}

inline void BEST_SPLIT(const std::string& s, char delim, Vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim))
    {
        if (item.size()!=0 && item!=" ") elems.push_back(item);
    }
}

inline bool BEST_IS_FIND_LAST(const std::string& origin, const std::string& toFind)
{
    std::basic_string<char>::size_type position = origin.find_last_of(toFind.c_str());
    if (position != std::string::npos)
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

inline void BEST_ERASE_LAST(std::string& origin, const std::string& toDelete)
{
    std::basic_string<char>::size_type position = origin.find_last_of(toDelete.c_str());
    if (position != std::string::npos)
    {
        origin.erase(position);
    }
}

struct BEST_UPPER_CHAR
{
    char operator()(char c) const
    {
        return std::toupper(BEST_CCAST(unsigned char,c));
    }
};

inline void BEST_TO_UPPER(std::string& origin)
{
    std::transform(origin.begin(), origin.end(), origin.begin(), BEST_UPPER_CHAR());
}

struct BEST_LOWER_CHAR
{
    char operator()(char c) const
    {
        return std::tolower(BEST_CCAST(unsigned char,c));
    }
};

inline void BEST_TO_LOWER(std::string& origin)
{
    std::transform(origin.begin(), origin.end(), origin.begin(), BEST_LOWER_CHAR());
}

inline void BEST_ERASE_ALL(std::string& origin, const std::string& match)
{
    const char *caracteres = match.c_str();
    for (unsigned int i = 0; i < strlen(caracteres); ++i)
    {
        origin.erase(std::remove(origin.begin(), origin.end(), caracteres[i]), origin.end());
    }
}
inline std::string& BEST_TRIM_RIGHT_INPLACE(std::string&  s, const std::string& delimiters = " \f\n\r\t\v" )
{
    return s.erase( s.find_last_not_of( delimiters ) + 1 );
}

inline std::string& BEST_TRIM_LEFT_INPLACE(std::string& s, const std::string& delimiters = " \f\n\r\t\v" )
{
    return s.erase( 0, s.find_first_not_of( delimiters ) );
}

inline std::string& BEST_STRING_TRIM(std::string& s, const std::string& delimiters = " \f\n\r\t\v" )
{
    return BEST_TRIM_LEFT_INPLACE(BEST_TRIM_RIGHT_INPLACE( s, delimiters ), delimiters );
}


//inline void BEST_STRING_TRIM(std::string& origin) {
//        origin.erase(origin.begin(), std::find_if(origin.begin(), origin.end(), std::not1(std::ptr_fun<int, int>(isspace))));
//        origin.erase(std::find_if(origin.rbegin(), origin.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base(), origin.end());
//}


inline bool BEST_STRING_COMPARE_WITHSTAR(const std::string& s, const std::string& s2)
{
    // Consider that only s as * !!!
    VecString vsnew;
    String snew = s;
    String snew2 = s2;
    snew = BEST_STRING_TRIM(snew);
    snew2 = BEST_STRING_TRIM(snew2);

    String reconstruct = "";

    BEST_SPLIT(snew, '*', vsnew);

    if (vsnew.size() == 0)
    {
        if (snew[0] == '*')
        {
            return true; // Particular case where only * as been used
        }
        else
        {
            return false;
        }
    }
    else
    {
        lint istart = 0;
		lint iend   = 0;
        for (int i = 0; i < vsnew.size(); i++)
        {
            iend = (lint) snew2.find(vsnew[i], istart);
            if ((i > 0) && iend == istart)
            {
                return false;
            }
            else if (iend == (lint) String::npos)
            {
                return false;
            }
            else
            {
                if (iend - istart > 0) reconstruct += snew2.substr(istart, iend - istart);
                reconstruct += vsnew[i];
                istart = (int) (iend + vsnew[i].size());
            }
        }

        if (snew[snew.size() - 1] == '*')
        {
            if (snew2.size() - istart <= 0)
            {
                return false;
            }
            else
            {
                reconstruct += snew2.substr(istart, snew2.size() - istart);
            }
        }


        /*
        for (int i = 0; i < vsnew.size(); i++)
        {
            iend = snew2.find(vsnew[i], istart);
            if ((i > 0) && iend == istart) return false;
            
            if (iend == String::npos) iend = snew2.size();
            if (iend > istart) reconstruct += snew2.substr(istart, iend-istart);
            
            reconstruct += vsnew[i];
            istart = iend + vsnew[i].size();
        }


        if (istart < snew2.size()) reconstruct += snew2.substr(istart, snew2.size()-istart);
        */

        return BEST_STRCMP(reconstruct, snew2);
    }

    return false;
}

inline void BEST_READ_FILE(const std::string& input, Vector<Vector<std::string> >& data)
{
    std::ifstream myfile;

    // Convert string to char*
    char* cinput = new char[input.size()+1];
    memcpy(cinput, &input[0], input.size()*sizeof(char));
    cinput[input.size()] = '\0';

    // Open file and read it
    myfile.open(cinput);
    std::string line;
    if (myfile.is_open())
    {
        while (myfile.good())
        {
            getline(myfile, line);
            Vector<std::string> elems;
            BEST_SPLIT(line, ' ', elems);
            data.push_back(elems);
        }
        myfile.close();
    }

    // Free char* memory
    delete[] cinput;
}

inline String BEST_SPRINTF(const String& in, const String& format)
{
    BEST_ASSERT(in.size()<1024);
    char sprintf_buffer[1024]; // HACK
#if defined WIN32
    // Avoid compiler warning
    sprintf_s(sprintf_buffer, 1024, format.c_str(), in.c_str());
#else
    std::sprintf(sprintf_buffer, format.c_str(), in.c_str());
#endif
    return String(sprintf_buffer);
}

inline String BEST_SPRINTF2(const String& in, const String& in2, const String& format)
{
    BEST_ASSERT(in.size()<1024);
    char sprintf_buffer[1024]; // HACK
#if defined WIN32
    // Avoid compiler warning
    sprintf_s(sprintf_buffer, 1024, format.c_str(), in.c_str(),in2.c_str());
#else
    std::sprintf(sprintf_buffer, format.c_str(), in.c_str(), in2.c_str());
#endif
    return String(sprintf_buffer);
}

#endif
