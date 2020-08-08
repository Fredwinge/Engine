#include "CKeyboard.h"

//Pull an event off the event queue
CKeyboard::Event CKeyboard::ReadKey() noexcept
{
	//Check if there's anything on the queue
	if (m_EventBuffer.size() > 0u)
	{
		//Copy first event on queue
		CKeyboard::Event e = m_EventBuffer.front();
		//Pop it after copying
		m_EventBuffer.pop();

		return e;
	}
	else
	{
		return CKeyboard::Event();
	}
}

char CKeyboard::ReadChar() noexcept
{
	//Check if there's anything on the queue
	if (m_CharBuffer.size() > 0u)
	{
		//Copy first char on queue
		unsigned char charCode = m_CharBuffer.front();
		//Pop it afterwards
		m_CharBuffer.pop();

		return charCode;
	}
	else
	{
		return 0;
	}
}


void CKeyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	m_KeyStates[keycode] = true;													//Set keyState to true, indicating that the button has been pressed
	m_EventBuffer.push(CKeyboard::Event(CKeyboard::Event::Type::PRESS, keycode));	//Push event type and keycode onto buffer
	TrimBuffer(m_EventBuffer);														//Make sure the buffer doesn't go oversize
}

void CKeyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	m_KeyStates[keycode] = false;													//Set keyState to true, indicating that the button has been released
	m_EventBuffer.push(CKeyboard::Event(CKeyboard::Event::Type::RELEASE, keycode));	//Push event type and keycode onto buffer
	TrimBuffer(m_EventBuffer);														//Make sure the buffer doesn't go oversize
}

void CKeyboard::OnChar(char character) noexcept
{
	m_CharBuffer.push(character);													//Push char onto buffer, no state here
	TrimBuffer(m_CharBuffer);														//Make sure the buffer isn't oversized
}

template<typename T>
void CKeyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	//Trims the buffer untils it's size is appropriate
	while (buffer.size() > s_bufferSize)
	{
		buffer.pop();
	}
}