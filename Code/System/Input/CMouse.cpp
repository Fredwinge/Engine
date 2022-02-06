#include "CMouse.h"
#include <Windows.h>

//Read mouse event from event queue
CMouse::Event CMouse::Read() //noexcept
{
	if (m_EventBuffer.size() > 0u)
	{
		CMouse::Event e = m_EventBuffer.front();
		m_EventBuffer.pop();

		return e;
	}
	else
	{
		return CMouse::Event();
	}
}

void CMouse::SetPos(Vector2 newPos) //noexcept
{
	m_Pos = newPos;

	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::MOVE, *this));
	TrimBuffer();
}

// MOUSE CAPTURE

void CMouse::OnMouseEnter() //noexcept
{
	m_bIsInWindow = true;
	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::ENTER, *this));

	TrimBuffer();
}

void CMouse::OnMouseLeave() //noexcept
{
	m_bIsInWindow = false;
	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::LEAVE, *this));

	TrimBuffer();
}

// LEFT

void CMouse::OnLeftPressed() //noexcept
{
	m_bLeftIsPressed = true;

	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::L_PRESS, *this));
	TrimBuffer();
}

void CMouse::OnLeftReleased() //noexcept
{
	m_bLeftIsPressed = false;

	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::L_RELEASE, *this));
	TrimBuffer();
}

// RIGHT

void CMouse::OnRightPressed() //noexcept
{
	m_bRightIsPressed = true;

	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::R_PRESS, *this));
	TrimBuffer();
}

void CMouse::OnRightReleased() //noexcept
{
	m_bRightIsPressed = false;

	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::R_RELEASE, *this));
	TrimBuffer();
}

// WHEEL

void CMouse::OnWheelUp() //noexcept
{
	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::WHEEL_UP, *this));

	TrimBuffer();
}

void CMouse::OnWheelDown() //noexcept
{
	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::WHEEL_DOWN, *this));

	TrimBuffer();
}

void CMouse::OnWheelDelta(int delta) //noexcept
{
	m_wheelDeltaCarry += delta;

	//Generate events for every 120 deltas steps
	while (m_wheelDeltaCarry >= WHEEL_DELTA)
	{
		m_wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp();
	}
	while (m_wheelDeltaCarry <= -WHEEL_DELTA)
	{
		m_wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown();
	}
}


void CMouse::TrimBuffer() //noexcept
{
	while (m_EventBuffer.size() > s_bufferSize)
	{
		m_EventBuffer.pop();
	}
}

//RAW DATA
void CMouse::MoveRaw(Vector2 rawDelta)
{
	m_LastRawPos = m_RawPos;
	m_RawPos += rawDelta;
}