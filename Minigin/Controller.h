#pragma once
#include <memory> // For std::unique_ptr

namespace dae
{
    class Controller
    {
    public:
        Controller(int controllerIndex);
        ~Controller();

        void ProcessInput();
        bool IsDownThisFrame(unsigned int button) const;
        bool IsUpThisFrame(unsigned int button) const;
        bool IsPressed(unsigned int button) const;

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
        // Pointer to the PImpl class
        class Impl;
        std::unique_ptr<Impl> pImpl;  // PImpl idiom
    };
}
