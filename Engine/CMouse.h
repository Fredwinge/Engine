#pragma once
#include <queue>
#include "Maths\Vectors.h"

class CMouse
{

	friend class CWindow;	//Let CWindow reach private variables

public:

	class Event
	{
	public:

		//TODO: Implement middle mouse button
		enum class Type
		{
			L_PRESS,
			L_RELEASE,
			R_PRESS,
			R_RELEASE,
			WHEEL_UP,
			WHEEL_DOWN,
			MOVE,
			ENTER,
			LEAVE,
			INVALID
		};

	private:

		Type m_eType;

		bool m_bLeftIsPressed;
		bool m_bRightIsPressed;

		Vector2 m_Pos;

	public:

		Event() noexcept
			:
			m_eType(Type::INVALID),
			m_bLeftIsPressed(false),
			m_bRightIsPressed(false),
			m_Pos(0.0f, 0.0f)
		{
		}

		Event(Type type, const CMouse& parent) noexcept
			:
			m_eType(type),
			m_bLeftIsPressed(parent.m_bLeftIsPressed),
			m_bRightIsPressed(parent.m_bRightIsPressed),
			m_Pos(parent.m_Pos)
		{
		}

		bool IsValid() const noexcept				{ return m_eType != Type::INVALID; }

		Type GetType() const noexcept				{ return m_eType; }

		Vector2 GetPos()const noexcept				{ return m_Pos; }

		bool LeftIsPressed()						{ return m_bLeftIsPressed; }
		bool RightIsPressed()						{ return m_bRightIsPressed; }
	};

public:

	CMouse() = default;
	CMouse(const CMouse&) = delete;
	CMouse& operator=(const CMouse&) = delete;

	CMouse::Event Read() /*noexcept*/;

	Vector2 GetPos() const noexcept					{ return m_Pos; }

	bool IsInWindow() const noexcept				{ return m_bIsInWindow; }
	bool LeftIsPressed() const noexcept				{ return m_bLeftIsPressed; }
	bool RightIsPressed() const noexcept			{ return m_bRightIsPressed; }

	bool IsEmpty() const noexcept					{ return m_EventBuffer.empty(); }

	void Flush() noexcept							{ m_EventBuffer = std::queue<Event>(); }

private:

	void OnMouseMove(Vector2 newPos);
	void OnMouseEnter();
	void OnMouseLeave();

	void OnLeftPressed();
	void OnLeftReleased();

	void OnRightPressed();
	void OnRightReleased();

	void OnWheelUp();
	void OnWheelDown();

	void OnWheelDelta(int delta);

	void TrimBuffer();

private:

	static constexpr unsigned int s_bufferSize = 16u;

	Vector2 m_Pos;

	bool m_bLeftIsPressed = false;
	bool m_bRightIsPressed = false;
	bool m_bIsInWindow = false;

	//A regular snap up or down with your mousewheel == 120, however "finer resolution" mousewheels might have this at
	//a lower value (such as 60, which would require two snaps for an effect), wheelDeltaCarry is used to accumulate these values
	//until they reach 120 or above (or below).
	int m_wheelDeltaCarry = 0;

	std::queue<Event> m_EventBuffer;
};