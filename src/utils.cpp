//
// Created by adrien on 7/16/25.
//

#include "utils.h"

Rgb hsvToRgb(const float h, const float s, const float v) {
    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;
    float r, g, b;

    if (h < 60)      { r = c; g = x; b = 0; }
    else if (h < 120){ r = x; g = c; b = 0; }
    else if (h < 180){ r = 0; g = c; b = x; }
    else if (h < 240){ r = 0; g = x; b = c; }
    else if (h < 300){ r = x; g = 0; b = c; }
    else             { r = c; g = 0; b = x; }

    return Rgb{
        static_cast<uint8_t>((r + m) * 255),
        static_cast<uint8_t>((g + m) * 255),
        static_cast<uint8_t>((b + m) * 255)
    };
}

uint8_t linearInterpolate(const uint8_t a, const uint8_t b, float t) {
    t = constrain(t, 0.0f, 1.0f);
    return static_cast<uint8_t>(static_cast<float>(a) + static_cast<float>(b - a) * t);
}
