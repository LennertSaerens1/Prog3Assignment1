#pragma once
#include <vector>
#include <algorithm>
#include <memory>
class Component {
public:
    virtual ~Component() {} // Virtual destructor for proper cleanup

    virtual void Update(float deltaTime) = 0; // Pure virtual function for updates
	virtual void FixedUpdate(const float fixedTime) = 0; // Pure virtual function for fixed updates
	virtual void Render(float x, float y) const = 0; // Pure virtual function for rendering

    

};

