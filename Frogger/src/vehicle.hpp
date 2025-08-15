#pragma once
#include "sgg/graphics.h"
#include "moving_object.hpp"

class Vehicle : public MovingObject
{
public:
	Vehicle(const std::string& name, float x, float y, float speed, const float width, const float height, const std::string& sprite);
	~Vehicle();
protected:
	void debugDraw() override;
};