#pragma once
#include <Windows.h>
#include <Xinput.h>
#include <chrono>

namespace dae
{
    class Controller
    {
    public:
        Controller(int controllerIndex);
        void ProcessInput();
        bool IsDownThisFrame(unsigned int button) const;
        bool IsUpThisFrame(unsigned int button) const;
        bool IsPressed(unsigned int button) const;

        void TestController();

        // Stick input methods
        float GetLeftStickX() const;
        float GetLeftStickY() const;
        float GetRightStickX() const;
        float GetRightStickY() const;

        // Trigger input methods
        float GetLeftTrigger() const;
        float GetRightTrigger() const;

        // Deadzone configuration
        void SetDeadzone(float deadzone);

        // Rumble function
        void Rumble(float leftMotorSpeed, float rightMotorSpeed, float durationInSeconds);

    private:
        DWORD m_controllerIndex;

        XINPUT_STATE currentState;
        XINPUT_STATE previousState;
        unsigned int buttonsPressedThisFrame;
        unsigned int buttonsReleasedThisFrame;

        bool m_isConnected = false;
        float m_deadzone; // Deadzone threshold for analog sticks

        // Rumble timer
        std::chrono::steady_clock::time_point m_rumbleEndTime;
        bool m_isRumbling;
    };
}
