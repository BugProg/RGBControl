#pragma once
#include <Arduino.h>
#include "utils.h"

/**
 * @enum Color
 * @brief Predefined color options for the RGB LED.
 */
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

/**
 * @enum Transition
 * @brief Describes the transition behavior when changing colors.
 */
enum class Transition : uint8_t {
    None,
    Fade,
};

/**
 * @enum Effect
 * @brief Describes continuous lighting effects.
 */
enum class Effect : uint8_t {
    Solid,
    Blink,
    Pulse,
    Rainbow,
    Glitch
};

/**
 * @class RgbLed
 * @brief Class for controlling an RGB LED with transitions, effects, and brightness.
 */
class RgbLed {
public:
    /**
     * @brief Constructor.
     * @param pinRed Arduino pin connected to red LED
     * @param pinGreen Arduino pin connected to green LED
     * @param pinBlue Arduino pin connected to blue LED
     * @param pwmInverted Whether the LED uses inverted PWM logic
    */
    RgbLed(const uint8_t pinRed, const uint8_t pinGreen, const uint8_t pinBlue, const bool pwmInverted = false)
        : _pinRed{pinRed}, _pinGreen{pinGreen}, _pinBlue{pinBlue}, _pwmInverted{pwmInverted} {
        _selectedTransition = Transition::None;
        _selectedEffect = Effect::Solid;
        pinMode(_pinRed, OUTPUT);
        pinMode(_pinGreen, OUTPUT);
        pinMode(_pinBlue, OUTPUT);
    }

    /**
     * @brief Sets a custom RGB color.
     * @param r Red (0-255)
     * @param g Green (0-255)
     * @param b Blue (0-255)
     * @param transition Transition effect to use
     */
    void setColor(uint8_t r, uint8_t g, uint8_t b, Transition transition = Transition::None);

    /**
     * @brief Sets a predefined color.
     * @param c Color enum value
     * @param transition Transition effect to use
     */
    void setColor(Color c, Transition transition = Transition::None);

    /**
     * @brief Sets the color transition effect.
     * @param transition Transition type
     */
    void setTransition(Transition transition);

    /**
     * @brief Sets the current lighting effect.
     * @param effect Effect type
     */
    void setEffect(Effect effect);

    /**
     * @brief Sets LED brightness.
     * @param luminosity Float between 0.0 (off) and 1.0 (full brightness)
     */
    void setLuminosity(float luminosity);

    /**
     * @brief Sets blink on duration.
     * @param duration Duration in milliseconds
     */
    void setBlinkOnDuration(uint8_t duration);

    /**
     * @brief Sets blink off duration.
     * @param duration Duration in milliseconds
     */
    void setBlinkOffDuration(uint8_t duration);

    /**
     * @brief Sets the period for the pulse effect.
     * @param pulsePeriod Duration in milliseconds for a full pulse cycle
     */
    void setPulsePeriod(float pulsePeriod);

    /**
     * @brief Enables gamma correction.
     */
    void enableGammaCorrection();

    /**
     * @brief Disables gamma correction.
     */
    void disableGammaCorrection();

    // Getters (const‑correct)
    uint8_t getBlinkOffDuration() const { return _blinkOffTimeMs; }
    Transition getTransition() const { return _selectedTransition; }
    uint8_t getBlinkOnDuration() const { return _blinkOnTimeMs; }
    float getPulsePeriod() const { return _pulsePeriod; }
    float getLuminosity() const { return _luminosity; }
    Color getColorToString() const { return _color; }
    Rgb getRgb() const { return _selectedRgb; }
    bool isOn() const { return _isOn; }
    bool getGammaEnabled() const { return _gammaEnabled; }

    /**
     * @brief Updates the LED state; call this in the loop().
     */
    void update();

    void on();

    void off();

private:
    // --- Hardware configuration ---
    const uint8_t _pinRed;
    const uint8_t _pinGreen;
    const uint8_t _pinBlue;
    bool _pwmInverted;

    // --- Global settings ---
    float _luminosity = 1.f;
    bool _gammaEnabled = false;
    bool _isOn = true;

    // --- Selected modes ---
    Color _color{Color::Red};
    Transition _selectedTransition{Transition::None};
    Effect _selectedEffect{Effect::Solid};

    // --- Color state ---
    Rgb _selectedRgb{};
    Rgb _last_rgb{};
    Rgb _glitch_rgb{};

    // --- Effect parameters ---
    uint16_t _blinkOnTimeMs = 1000;
    uint16_t _blinkOffTimeMs = 1000;
    float _pulsePeriod = 2000.f;

    // --- Runtime state / timing ---
    bool _blinkState = true;
    unsigned long lastUpdate = 0;
    unsigned long _transitionTime = 0;
    unsigned long _effectMemoryTime = 0;

    // --- Internal utility methods ---
    uint8_t applyPwmInversion(uint8_t color) const;
    Rgb computeTransitionColor();
    Rgb computeEffectColor(Rgb rgb);
};
