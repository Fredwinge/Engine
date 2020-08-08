#pragma once
#include <queue>

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

		int m_xPos;
		int m_yPos;

	public:

		Event() noexcept
			:
			m_eType(Type::INVALID),
			m_bLeftIsPressed(false),
			m_bRightIsPressed(false),
			m_xPos(0),
			m_yPos(0)
		{
		}

		Event(Type type, const CMouse& parent) noexcept
			:
			m_eType(type),
			m_bLeftIsPressed(parent.m_bLeftIsPressed),
			m_bRightIsPressed(parent.m_bRightIsPressed),
			m_xPos(parent.m_xPos),
			m_yPos(parent.m_yPos)
		{
		}

		bool IsValid() const noexcept				{ return m_eType != Type::INVALID; }

		Type GetType() const noexcept				{ return m_eType; }

		std::pair<int, int> GetPos()const noexcept	{ return { m_xPos, m_yPos }; }

		int GetPosX() const noexcept				{ return m_xPos; }
		int GetPosY() const noexcept				{ return m_yPos; }

		bool LeftIsPressed()						{ return m_bLeftIsPressed; }
		bool RightIsPressed()						{ return m_bRightIsPressed; }
	};

public:

	CMouse() = default;
	CMouse(const CMouse&) = delete;
	CMouse& operator=(const CMouse&) = delete;

	CMouse::Event Read() noexcept;

	//TODO: Implement 2DVector class for this instead
	std::pair<int, int> GetPos() const noexcept		{ return{ m_xPos, m_yPos }; }

	int GetPosX() const noexcept					{ return m_xPos; }
	int GetPosY() const noexcept					{ return m_yPos; }

	bool IsInWindow() const noexcept				{ return m_bIsInWindow; }
	bool LeftIsPressed() const noexcept				{ return m_bLeftIsPressed; }
	bool RightIsPressed() const noexcept			{ return m_bRightIsPressed; }

	bool IsEmpty() const noexcept					{ return m_EventBuffer.empty(); }

	void Flush() noexcept							{ m_EventBuffer = std::queue<Event>(); }

private:

	void OnMouseMove(int x, int y) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;

	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;

	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;

	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;

	void OnWheelDelta(int x, int y, int delta) noexcept;

	void TrimBuffer() noexcept;

private:

	static constexpr unsigned int s_bufferSize = 16u;

	int m_xPos;
	int m_yPos;

	bool m_bLeftIsPressed = false;
	bool m_bRightIsPressed = false;
	bool m_bIsInWindow = false;

	//A regular snap up or down with your mousewheel == 120, however "finer resolution" mousewheels might have this at
	//a lower value (such as 60, which would require to snaps for an effect), wheelDeltaCarry is used to accumulate these values
	//until they reach 120 or above (or below).
	int m_wheelDeltaCarry = 0;

	std::queue<Event> m_EventBuffer;
};