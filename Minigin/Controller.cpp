#include "Controller.h"


dae::Controller::Controller(int controllerIndex)
    : m_controllerIndex(controllerIndex)
{
}

void dae::Controller::ProcessInput()
{
    
    CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
    ZeroMemory(&currentState, sizeof(XINPUT_STATE));
    XInputGetState(m_controllerIndex, &currentState);

    auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
    buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
    buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
}

bool dae::Controller::IsDownThisFrame(unsigned int button) const
{
    return buttonsPressedThisFrame & button;
}
bool dae::Controller::IsUpThisFrame(unsigned int button) const
{
    return buttonsReleasedThisFrame & button;
}
bool dae::Controller::IsPressed(unsigned int button) const
{
    return currentState.Gamepad.wButtons & button;
}
