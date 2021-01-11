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

		Type m_eEventType;					//Type of event
		unsigned char m_cKeyCode;			//Keycode involved in the event

	public:

		Event()
			:
			m_eEventType(Type::INVALID),
			m_cKeyCode(0u)
		{ }

		Event(Type type, unsigned char code) /*noexcept*/ :
			m_eEventType(type),
			m_cKeyCode(code)
		{ }

		bool IsPress() const noexcept			{ return m_eEventType == Type::PRESS; }

		bool IsRelease() const noexcept			{ return m_eEventType == Type::RELEASE; }

		bool IsValid() const noexcept			{ return m_eEventType != Type::INVALID; }

		unsigned char GetCode() const noexcept	{ return m_cKeyCode; }
	};

	CKeyboard() = default;
	CKeyboard(const CKeyboard&) = delete;

	//key event stuff
	Event ReadKey() /*noexcept*/;																		
	bool KeyIsPressed(unsigned char keycode) const noexcept { return m_KeyStates[keycode]; }
	bool KeyIsEmpty() const noexcept						{ return m_EventBuffer.empty(); }			//Check if there are any events in the event queue
	void ClearKey() noexcept								{ m_EventBuffer = std::queue<Event>(); }	//Clears the queue

	//char event stuff
	char ReadChar() /*noexcept*/;
	bool CharIsEmpty() const noexcept						{ return m_CharBuffer.empty(); }
	void ClearChar() noexcept								{ m_CharBuffer = std::queue<char>(); }

	void Flush() noexcept									{ ClearKey(); ClearChar(); }			//Clears both the char and key queues

	//autorepeat control
	void EnableAutoRepeat() noexcept						{ autoRepeatEnabled = true; }
	void DisableAutoRepeat() noexcept						{ autoRepeatEnabled = false; }
	bool AutoRepeatIsEnabled() const noexcept				{ return autoRepeatEnabled; }

private:

	void OnKeyPressed(unsigned char keycode) /*noexcept*/;
	void OnKeyReleased(unsigned char keycode) /*noexcept*/;

	void OnChar(char character) /*noexcept*/;

	void ClearState() noexcept	{ m_KeyStates.reset(); }				//Resets keystate bitset

	template<typename T>
	static void TrimBuffer(std::queue<T> & buffer) /*noexcept*/;			//Removes items from the buffer if its size grows larger than 16

private:

	static constexpr unsigned int s_nKeys = 256u;						//The max amount of virtual keycodes we can have
	static constexpr unsigned int s_bufferSize = 16u;

	bool autoRepeatEnabled = false;

	std::bitset<s_nKeys> m_KeyStates;									//Packs all our 256 keycodes into 1-bit bools

	std::queue<Event> m_EventBuffer;
	std::queue<char> m_CharBuffer;

};