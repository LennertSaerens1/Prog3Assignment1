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

    float GetRightStickX() const
    {
        float x = static_cast<float>(currentState.Gamepad.sThumbRX) / 32767.0f;
        if (std::abs(x) < m_deadzone) x = 0.0f;
        return x;
    }

    float GetRightStickY() const
    {
        float y = static_cast<float>(currentState.Gamepad.sThumbRY) / 32767.0f;
        if (std::abs(y) < m_deadzone) y = 0.0f;
        return y;
    }

    float GetLeftTrigger() const
    {
        return static_cast<float>(currentState.Gamepad.bLeftTrigger) / 255.0f;
    }

    float GetRightTrigger() const
    {
        return static_cast<float>(currentState.Gamepad.bRightTrigger) / 255.0f;
    }

    void SetDeadzone(float deadzone)
    {
        // Clamp deadzone to reasonable values (0.0 to 1.0)
        m_deadzone = std::clamp(deadzone, 0.0f, 1.0f);
    }

    void Rumble(float leftMotorSpeed, float rightMotorSpeed, float durationInSeconds)
    {
        // Clamp motor speeds to valid range (0.0 to 1.0)
        leftMotorSpeed = std::clamp(leftMotorSpeed, 0.0f, 1.0f);
        rightMotorSpeed = std::clamp(rightMotorSpeed, 0.0f, 1.0f);

        // Convert to XInput expected value (0 to 65535)
        WORD leftMotor = static_cast<WORD>(leftMotorSpeed * 65535.0f);
        WORD rightMotor = static_cast<WORD>(rightMotorSpeed * 65535.0f);

        // Set vibration
        XINPUT_VIBRATION vibration;
        vibration.wLeftMotorSpeed = leftMotor;
        vibration.wRightMotorSpeed = rightMotor;

        if (XInputSetState(m_controllerIndex, &vibration) == ERROR_SUCCESS)
        {
            m_isRumbling = true;
            m_rumbleEndTime = std::chrono::steady_clock::now() +
                std::chrono::milliseconds(static_cast<int>(durationInSeconds * 1000.0f));
        }
    }
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

float dae::Controller::GetRightStickX() const { return pImpl->GetRightStickX(); }
float dae::Controller::GetRightStickY() const { return pImpl->GetRightStickY(); }
float dae::Controller::GetLeftTrigger() const { return pImpl->GetLeftTrigger(); }
float dae::Controller::GetRightTrigger() const { return pImpl->GetRightTrigger(); }
void dae::Controller::SetDeadzone(float deadzone) { pImpl->SetDeadzone(deadzone); }
void dae::Controller::Rumble(float leftMotorSpeed, float rightMotorSpeed, float durationInSeconds)
{
    pImpl->Rumble(leftMotorSpeed, rightMotorSpeed, durationInSeconds);
}
