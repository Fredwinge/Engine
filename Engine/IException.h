#pragma once
//#include <exception>
#include <string>

//Base exception class
class IException //: public std::exception
{
public:

	IException(int line, const char* file) noexcept;
	virtual const char* what() const noexcept;

	std::string GetOriginString() const noexcept;

	virtual const char* GetType() const noexcept	{ return "Exception"; }
	int GetLine() const noexcept					{ return m_iExceptionLine; }
	const std::string& GetFile() const noexcept		{ return m_sExceptionFile; }

	void DisplayError();

private:

	int m_iExceptionLine;			//Line number which exception was thrown from
	std::string m_sExceptionFile;	//File which exception was thrown from

protected:

	mutable std::string m_sWhatBuffer;
};