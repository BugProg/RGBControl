#pragma once
#include <Arduino.h>
#include "utils.h"

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

enum class Transition : uint8_t {
    None,
    Fade,
};

enum class Effect : uint8_t {
    None,
    Blink,
    Pulse,
    Rainbow,
    Glitch
};


class RgbLed {
public:
    RgbLed(const uint8_t pinRed, const uint8_t pinGreen, const uint8_t pinBlue, const bool pwmInverted = false)
        : _pinRed{pinRed}, _pinGreen{pinGreen}, _pinBlue{pinBlue}, _pwmInverted{pwmInverted} {
        _selectedTransition = Transition::None;
        _selectedEffect = Effect::None;
        pinMode(_pinRed, OUTPUT);
        pinMode(_pinGreen, OUTPUT);
        pinMode(_pinBlue, OUTPUT);
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b, Transition transition = Transition::None);

    void setColor(Color c, Transition transition = Transition::None);

    void setTransition(Transition transition);
    void setEffect(Effect effect);

    void setLuminosity(float luminosity);

    void setBlinkOnDuration(uint8_t duration);
    void setBlinkOffDuration(uint8_t duration);

    void setPulsePeriod(float pulsePeriod);

    void update();

    void on();

    void off();

    // Getters (const‑correct)
    uint8_t getBlinkOffDuration() const { return _blinkOffTimeMs; }
    Transition getTransition() const { return _selectedTransition; }
    uint8_t getBlinkOnDuration() const { return _blinkOnTimeMs; }
    float getPulsePeriod() const { return _pulsePeriod; }
    float getLuminosity() const { return _luminosity; }
    Color getColorToString() const { return _color; }
    Rgb getRgb() const { return _rgb; }
    bool isOn() const { return _isOn; }

private:
    uint8_t applyPwmInversion(uint8_t color) const;

    // Pins are immutable after construction
    const uint8_t _pinRed, _pinGreen, _pinBlue;
    unsigned long lastUpdate = 0;
    float _luminosity = 1.f;

    // Blink settings
    uint16_t _blinkOnTimeMs = 1000;
    uint16_t _blinkOffTimeMs = 1000;
    bool _blinkState = true;

    // Pulse settings
    float _pulsePeriod = 2000.f;

    Rgb _rgb{};
    Rgb _last_rgb{};
    Rgb _glitch_rgb{};

    // Runtime state
    Color _color{Color::Red};
    bool _isOn{true};
    bool _pwmInverted;
    Transition _selectedTransition;
    Effect _selectedEffect;

    unsigned long _transitionTime = 0;
    unsigned long _effectMemoryTime = 0;

    Rgb computeTransitionColor();
    Rgb computeEffectColor(Rgb rgb);
};
