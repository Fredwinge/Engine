#include "IException.h"
#include <sstream>
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <string>

#ifndef NDEBUG
#   define M_Assert(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define M_Assert(Expr, Msg) ;
#endif

void __M_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg)
{
	if (!expr)
	{
		std::cerr << "Assert failed:\t" << msg << "\n"
			<< "Expected:\t" << expr_str << "\n"
			<< "Source:\t\t" << file << ", line " << line << "\n";
		abort();
	}
}

#define IAssert(x) if (!x) { assert_fail(#x, __FILE__, __LINE__); }

void assert_fail(const char* str, const char* file, int line)
{
	std::cerr << "Assertion failed " << str << " " << file << ":" << line << std::endl;
	abort();
}

IException::IException(int line, const char* file) noexcept
	:
	m_iExceptionLine(line),
	m_sExceptionFile(file)
{ }

const char* IException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	m_sWhatBuffer = oss.str();

	return m_sWhatBuffer.c_str();
}

std::string IException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << m_sExceptionFile << std::endl << "[Line] " << m_iExceptionLine;

	return oss.str();
}

void IException::DisplayError()
{
	MessageBoxA(nullptr, what(), GetType(), MB_OK | MB_ICONEXCLAMATION);
	exit(1);
	//assert(false);

	//M_Assert(false, what());

	//std::string str = what();

	//std::wostringstream wstm;
	//const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());
	//for (size_t i = 0; i < str.size(); ++i)
	//	wstm << ctfacet.widen(str[i]);
	//wstm.str();

	////wchar_t* pWhat = widen
	//_wassert(wstm.str().c_str(), (wchar_t*)__FILE__, __LINE__);

	//IAssert(false);
}