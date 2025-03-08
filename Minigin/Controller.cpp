#include "Controller.h"
#include <cstring> // For memset
#include <iostream>
#include <cmath>
#include <chrono>
#include <algorithm>

dae::Controller::Controller(int controllerIndex)
    : m_controllerIndex(static_cast<DWORD>(controllerIndex)),
    buttonsPressedThisFrame(0),
    buttonsReleasedThisFrame(0),
    m_isConnected(false)
{
    memset(&currentState, 0, sizeof(XINPUT_STATE));
    memset(&previousState, 0, sizeof(XINPUT_STATE));


}

void dae::Controller::ProcessInput()
{
    //TestController();

    // Copy previous state
    previousState = currentState;

    // Clear the current state
    ZeroMemory(&currentState, sizeof(XINPUT_STATE));

    // Try to get the controller state
    DWORD result = XInputGetState(m_controllerIndex, &currentState);

    if (result == ERROR_SUCCESS)
    {
        m_isConnected = true;

        // Compute button changes
        auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
        buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
        buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
    }
    else
    {
        // Controller is disconnected, reset state
        m_isConnected = false;
        ZeroMemory(&currentState, sizeof(XINPUT_STATE));
        buttonsPressedThisFrame = 0;
        buttonsReleasedThisFrame = 0;
    }

    // Check if rumble is still active
    if (m_isRumbling && std::chrono::steady_clock::now() >= m_rumbleEndTime)
    {
        // Stop rumbling after the specified duration
        XINPUT_VIBRATION vibration = { 0, 0 }; // Stop vibration
        XInputSetState(m_controllerIndex, &vibration);
        m_isRumbling = false;
    }
}

bool dae::Controller::IsDownThisFrame(unsigned int button) const
{
    return (buttonsPressedThisFrame & button) != 0;
}

bool dae::Controller::IsUpThisFrame(unsigned int button) const
{
    return (buttonsReleasedThisFrame & button) != 0;
}

bool dae::Controller::IsPressed(unsigned int button) const
{
    return (currentState.Gamepad.wButtons & button) != 0;
}

void dae::Controller::TestController()
{
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    /*DWORD result = XInputGetState(0, &state);
    if (result == ERROR_SUCCESS)
    {
        std::cout << "Controller connected!\n";
    }
    else
    {
        std::cout << "No controller detected.\n";
    }*/
    if (XInputGetState(0, &state) == ERROR_SUCCESS)
    {
        std::cout << "Buttons Pressed: " << std::hex << state.Gamepad.wButtons << std::endl;
    }
    else
    {
        std::cout << "Controller disconnected!\n";

    }
}

// Get the X value of the left analog stick with deadzone
float dae::Controller::GetLeftStickX() const
{
    float x = static_cast<float>(currentState.Gamepad.sThumbLX) / 32767.0f; // Normalize to -1 to 1
    if (std::abs(x) < m_deadzone)
        x = 0.0f; // Apply deadzone by setting to 0 if within threshold
    return x;
}

// Get the Y value of the left analog stick with deadzone
float dae::Controller::GetLeftStickY() const
{
    float y = static_cast<float>(currentState.Gamepad.sThumbLY) / 32767.0f; // Normalize to -1 to 1
    if (std::abs(y) < m_deadzone)
        y = 0.0f; // Apply deadzone by setting to 0 if within threshold
    return y;
}

// Get the X value of the right analog stick with deadzone
float dae::Controller::GetRightStickX() const
{
    float x = static_cast<float>(currentState.Gamepad.sThumbRX) / 32767.0f; // Normalize to -1 to 1
    if (std::abs(x) < m_deadzone)
        x = 0.0f; // Apply deadzone by setting to 0 if within threshold
    return x;
}

// Get the Y value of the right analog stick with deadzone
float dae::Controller::GetRightStickY() const
{
    float y = static_cast<float>(currentState.Gamepad.sThumbRY) / 32767.0f; // Normalize to -1 to 1
    if (std::abs(y) < m_deadzone)
        y = 0.0f; // Apply deadzone by setting to 0 if within threshold
    return y;
}

// Get the value of the left trigger (0 to 1)
float dae::Controller::GetLeftTrigger() const
{
    return static_cast<float>(currentState.Gamepad.bLeftTrigger) / 255.0f;
}

// Get the value of the right trigger (0 to 1)
float dae::Controller::GetRightTrigger() const
{
    return static_cast<float>(currentState.Gamepad.bRightTrigger) / 255.0f;
}

// Set a custom deadzone
void dae::Controller::SetDeadzone(float deadzone)
{
    m_deadzone = deadzone;
}

// Rumble function: Activates rumble for a specified duration
void dae::Controller::Rumble(float leftMotorSpeed, float rightMotorSpeed, float durationInSeconds)
{
    // Ensure that the rumble values are between 0 and 1
    leftMotorSpeed = std::clamp(leftMotorSpeed, 0.0f, 1.0f);
    rightMotorSpeed = std::clamp(rightMotorSpeed, 0.0f, 1.0f);

    // Create the vibration structure
    XINPUT_VIBRATION vibration;
    vibration.wLeftMotorSpeed = static_cast<WORD>(leftMotorSpeed * 65535);  // Max is 65535
    vibration.wRightMotorSpeed = static_cast<WORD>(rightMotorSpeed * 65535); // Max is 65535

    // Set the vibration
    XInputSetState(m_controllerIndex, &vibration);

    // Set the rumble end time
    m_rumbleEndTime = std::chrono::steady_clock::now() + std::chrono::seconds(static_cast<int>(durationInSeconds));
    m_isRumbling = true;
}
