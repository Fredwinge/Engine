#pragma once
#include <queue>
#include <bitset>

class CKeyboard
{

	friend class CWindow;	//Let CWindow reach private variables

public:

	class Event
	{

	public:

		enum class Type
		{
			PRESS,
			RELEASE,
			INVALID
		};

	private:

		Type m_eType;
		unsigned char m_cCode;

	public:

		Event()
			:
			m_eType(Type::INVALID),
			m_cCode(0u)
		{ }

		Event(Type type, unsigned char code) noexcept :
			m_eType(type),
			m_cCode(code)
		{ }

		bool IsPress() const noexcept			{ return m_eType == Type::PRESS; }

		bool IsRelease() const noexcept			{ return m_eType == Type::RELEASE; }

		bool IsValid() const noexcept			{ return m_eType != Type::INVALID; }

		unsigned char GetCode() const noexcept	{ return m_cCode; }
	};


};