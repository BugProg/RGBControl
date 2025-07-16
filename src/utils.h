//
// Created by adrien on 7/16/25.
//

#pragma once
#include <Arduino.h>

struct Rgb {
    uint8_t r, g, b;
};

Rgb hsvToRgb(float h, float s, float v);

uint8_t linearInterpolate(uint8_t a, uint8_t b, float t);