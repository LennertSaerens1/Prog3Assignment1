#include "Controller.h"
#include <Windows.h>  // Now it's safe to include Windows.h here.
#include <Xinput.h>
#include <cstring>
#include <iostream>
#include <cmath>
#include <chrono>
#include <algorithm>

// Define the PImpl class here
class dae::Controller::Impl
{
public:
    DWORD m_controllerIndex;
    XINPUT_STATE currentState;
    XINPUT_STATE previousState;
    unsigned int buttonsPressedThisFrame;
    unsigned int buttonsReleasedThisFrame;
    bool m_isConnected = false;
    float m_deadzone;  // Deadzone threshold for analog sticks
    std::chrono::steady_clock::time_point m_rumbleEndTime;
    bool m_isRumbling;

    Impl(int controllerIndex)
        : m_controllerIndex(static_cast<DWORD>(controllerIndex)),
        buttonsPressedThisFrame(0),
        buttonsReleasedThisFrame(0),
        m_isConnected(false),
        m_isRumbling(false),
        m_deadzone(0.1f) // Set default deadzone to 0.1f
    {
        memset(&currentState, 0, sizeof(XINPUT_STATE));
        memset(&previousState, 0, sizeof(XINPUT_STATE));
    }

    void ProcessInput()
    {
        previousState = currentState;
        ZeroMemory(&currentState, sizeof(XINPUT_STATE));

        DWORD result = XInputGetState(m_controllerIndex, &currentState);

        if (result == ERROR_SUCCESS)
        {
            m_isConnected = true;
            auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
            buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
            buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
        }
        else
        {
            m_isConnected = false;
            ZeroMemory(&currentState, sizeof(XINPUT_STATE));
            buttonsPressedThisFrame = 0;
            buttonsReleasedThisFrame = 0;
        }

        if (m_isRumbling && std::chrono::steady_clock::now() >= m_rumbleEndTime)
        {
            XINPUT_VIBRATION vibration = { 0, 0 };
            XInputSetState(m_controllerIndex, &vibration);
            m_isRumbling = false;
        }
    }

    bool IsDownThisFrame(unsigned int button) const
    {
        return (buttonsPressedThisFrame & button) != 0;
    }

    bool IsUpThisFrame(unsigned int button) const
    {
        return (buttonsReleasedThisFrame & button) != 0;
    }

    bool IsPressed(unsigned int button) const
    {
        return (currentState.Gamepad.wButtons & button) != 0;
    }

    float GetLeftStickX() const
    {
        float x = static_cast<float>(currentState.Gamepad.sThumbLX) / 32767.0f;
        if (std::abs(x) < m_deadzone) x = 0.0f;
        return x;
    }

    float GetLeftStickY() const
    {
        float y = static_cast<float>(currentState.Gamepad.sThumbLY) / 32767.0f;
        if (std::abs(y) < m_deadzone) y = 0.0f;
        return y;
    }

    // Other methods...
};

// Constructor that initializes the PImpl
dae::Controller::Controller(int controllerIndex)
    : pImpl(std::make_unique<Impl>(controllerIndex))
{
}

// Destructor
dae::Controller::~Controller() = default;

// Delegate methods to the PImpl implementation
void dae::Controller::ProcessInput() { pImpl->ProcessInput(); }
bool dae::Controller::IsDownThisFrame(unsigned int button) const { return pImpl->IsDownThisFrame(button); }
bool dae::Controller::IsUpThisFrame(unsigned int button) const { return pImpl->IsUpThisFrame(button); }
bool dae::Controller::IsPressed(unsigned int button) const { return pImpl->IsPressed(button); }

float dae::Controller::GetLeftStickX() const { return pImpl->GetLeftStickX(); }
float dae::Controller::GetLeftStickY() const { return pImpl->GetLeftStickY(); }

// Other methods...
