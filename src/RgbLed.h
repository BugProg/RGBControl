#pragma once
#include <Arduino.h>
#include <cstdint>

enum class Color : uint8_t {
    Black,
    White,
    Red,
    Green,
    Blue,
    Yellow,
    Cyan,
    Magenta,
    Orange,
    Purple,
    Pink,
    Custom
};

struct Rgb {
    uint8_t r, g, b;
};

enum class Transition : uint8_t {
    None,
    Fade,
    Wipe,
};

class RgbLed {
public:
    RgbLed(const uint8_t pinRed, const uint8_t pinGreen, const uint8_t pinBlue, const bool pwmInverted = false)
        : _pinRed{pinRed}, _pinGreen{pinGreen}, _pinBlue{pinBlue}, _pwmInverted{pwmInverted} {
        _transition = Transition::None;
        pinMode(_pinRed, OUTPUT);
        pinMode(_pinGreen, OUTPUT);
        pinMode(_pinBlue, OUTPUT);
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b, Transition transition = Transition::None);

    void setColor(Color c, Transition transition = Transition::None);

    void setTransition(Transition transition);

    void update();

    void on();

    void off();

    // Getters (const‑correct)
    Color getColorToString() const { return _color; }
    Transition getTransition() const { return _transition; }
    Rgb getRgb() const { return _rgb; }
    bool isOn() const { return _isOn; }

private:
    uint8_t applyPwmInversion(uint8_t color) const;

    // Pins are immutable after construction
    const uint8_t _pinRed, _pinGreen, _pinBlue;
    unsigned long lastUpdate = 0;

    Rgb _rgb{};
    Rgb _last_rgb{};

    // Runtime state
    Color _color{Color::Red};
    bool _isOn{true};
    bool _pwmInverted;
    Transition _transition;

    unsigned long _transitionTime = 0;
    Rgb computeTransitionColor();
    uint8_t linearInterpolate(uint8_t a, uint8_t b, float t) const;
};
