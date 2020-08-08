#include "CMouse.h"
#include <Windows.h>

//Read mouse event from event queue
CMouse::Event CMouse::Read() noexcept
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

void CMouse::OnMouseMove(int newX, int newY) noexcept
{
	m_xPos = newX;
	m_yPos = newY;

	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::MOVE, *this));
	TrimBuffer();
}

// MOUSE CAPTURE

void CMouse::OnMouseEnter() noexcept
{
	m_bIsInWindow = true;
	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::ENTER, *this));

	TrimBuffer();
}

void CMouse::OnMouseLeave() noexcept
{
	m_bIsInWindow = false;
	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::LEAVE, *this));

	TrimBuffer();
}

// LEFT

void CMouse::OnLeftPressed(int x, int y) noexcept
{
	m_bLeftIsPressed = true;

	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::L_PRESS, *this));
	TrimBuffer();
}

void CMouse::OnLeftReleased(int x, int y) noexcept
{
	m_bLeftIsPressed = false;

	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::L_RELEASE, *this));
	TrimBuffer();
}

// RIGHT

void CMouse::OnRightPressed(int x, int y) noexcept
{
	m_bRightIsPressed = true;

	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::R_PRESS, *this));
	TrimBuffer();
}

void CMouse::OnRightReleased(int x, int y) noexcept
{
	m_bRightIsPressed = false;

	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::R_RELEASE, *this));
	TrimBuffer();
}

// WHEEL

void CMouse::OnWheelUp(int x, int y) noexcept
{
	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::WHEEL_UP, *this));

	TrimBuffer();
}

void CMouse::OnWheelDown(int x, int y) noexcept
{
	m_EventBuffer.push(CMouse::Event(CMouse::Event::Type::WHEEL_DOWN, *this));

	TrimBuffer();
}

void CMouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	m_wheelDeltaCarry += delta;

	//Generate events for every 120 deltas steps
	while (m_wheelDeltaCarry >= WHEEL_DELTA)
	{
		m_wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (m_wheelDeltaCarry <= -WHEEL_DELTA)
	{
		m_wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}


void CMouse::TrimBuffer() noexcept
{
	while (m_EventBuffer.size() > s_bufferSize)
	{
		m_EventBuffer.pop();
	}
}