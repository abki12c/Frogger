#pragma once
#include "sgg/graphics.h"
#include "moving_object.hpp"

class Log : public MovingObject
{
public:
    Log(const std::string name, float x, float y, float speed, const float width, const float height, const std::string& sprite);
    ~Log();
protected:
    void debugDraw() override;
};