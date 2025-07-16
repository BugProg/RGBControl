//
// Created by adrien on 7/16/25.
//

#pragma once
#include <Arduino.h>

/**
 * @struct Rgb
 * @brief Represents a color in RGB format.
 */
struct Rgb {
    uint8_t r, g, b;
};

/**
 * @brief Converts HSV (Hue, Saturation, Value) to RGB.
 *
 * @param h Hue value in degrees [0.0 - 360.0]
 * @param s Saturation value [0.0 - 1.0]
 * @param v Brightness/Value [0.0 - 1.0]
 * @return Rgb struct with corresponding RGB values [0 - 255]
 */
Rgb hsvToRgb(float h, float s, float v);

/**
 * @brief Linearly interpolates between two 8-bit values.
 *
 * @param a Start value
 * @param b End value
 * @param t Interpolation factor [0.0 - 1.0]
 * @return Interpolated value between a and b
 */
uint8_t linearInterpolate(uint8_t a, uint8_t b, float t);

/**
 * @brief Applies gamma correction to a color value.
 *
 * @param value Color intensity (0–255)
 * @param gamma Gamma correction factor (default is 2.2)
 * @return Gamma-corrected value (0–255)
 */
uint8_t applyGammaCorrection(uint8_t value, float gamma = 2.2);