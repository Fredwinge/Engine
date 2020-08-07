#include "IException.h"
#include <sstream>

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