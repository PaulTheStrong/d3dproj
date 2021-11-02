#include "Mouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept
{
    return { x, y };
}

int Mouse::GetPosX() const noexcept
{
    return x;
}

int Mouse::GetPosY() const noexcept
{
    return y;
}

bool Mouse::LeftIsPressed() const noexcept
{
    return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
    return rightIsPressed;
}

Mouse::Event Mouse::Read() noexcept
{
    if (buffer.size() > 0) 
    {
        auto event = buffer.front();
        buffer.pop();
        return event;
    }
    else
    {
        return Event();
    }
    return Mouse::Event();
}

bool Mouse::IsEmpty() const noexcept
{
    return buffer.empty();
}

void Mouse::Flush() noexcept
{
    buffer = std::queue<Mouse::Event>();
}

void Mouse::OnMouseMove(int x, int y) noexcept
{
    this->x = x;
    this->y = y;
    buffer.push(Mouse::Event(Event::Type::Move, *this));
    TrimBuffer();
}

void Mouse::OnLeftButtonPressed(int x, int y) noexcept
{
    leftIsPressed = true;
    buffer.push(Mouse::Event(Event::Type::LPress, *this));
    TrimBuffer();
}

void Mouse::OnLeftButtonReleased(int x, int y) noexcept
{
    leftIsPressed = false;
    buffer.push(Mouse::Event(Event::Type::LRelease, *this));
    TrimBuffer();
}

void Mouse::OnRightButtonPressed(int x, int y) noexcept
{
    this->x = x;
    this->y = y;
    rightIsPressed = true;
    buffer.push(Mouse::Event(Event::Type::RPress, *this));
    TrimBuffer();
}

void Mouse::OnRightButtonReleased(int x, int y) noexcept
{
    rightIsPressed = false;
    buffer.push(Mouse::Event(Event::Type::RRelease, *this));
    TrimBuffer();
}

void Mouse::OnWheeUp(int x, int y) noexcept
{
    buffer.push(Mouse::Event(Event::Type::WheelUp, *this));
    TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
    buffer.push(Mouse::Event(Event::Type::WheelDown, *this));
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}
