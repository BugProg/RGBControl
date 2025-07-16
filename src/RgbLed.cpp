//
// Created by adrien on 7/15/25.
//

#include "RgbLed.h"

void RgbLed::setColor(const uint8_t r, const uint8_t g, const uint8_t b, const Transition transition) {
    _color = Color::Custom;
    setTransition(transition);
    _last_rgb = _rgb;
    _rgb.r = r;
    _rgb.g = g;
    _rgb.b = b;

    update();
}

void RgbLed::setColor(const Color c, const Transition transition) {
    switch (c) {
        case Color::Black:
            setColor(0, 0, 0, transition);
            break;
        case Color::White:
            setColor(255, 255, 255, transition);
            break;
        case Color::Red:
            setColor(255, 0, 0, transition);
            break;
        case Color::Green:
            setColor(0, 255, 0, transition);
            break;
        case Color::Blue:
            setColor(0, 0, 255, transition);
            break;
        case Color::Yellow:
            setColor(255, 255, 0, transition);
            break;
        case Color::Cyan:
            setColor(0, 255, 255, transition);
            break;
        case Color::Magenta:
            setColor(255, 0, 255, transition);
            break;
        case Color::Orange:
            setColor(255, 165, 0, transition);
            break;
        case Color::Purple:
            setColor(128, 0, 128, transition);
            break;
        case Color::Pink:
            setColor(255, 105, 180, transition);
            break;
        default: break;
    }
}

void RgbLed::update() {
    if (millis() - lastUpdate < 25) return;
    lastUpdate = millis();

    Rgb rgb = computeTransitionColor();

    rgb = computeEffectColor(rgb);

    auto scale = [&](const uint8_t value) -> uint8_t {
        return static_cast<uint8_t>(std::lround(static_cast<float>(value) * static_cast<float>(_isOn) * _luminosity));
    };

    analogWrite(_pinRed, applyPwmInversion(scale(rgb.r)));
    analogWrite(_pinGreen, applyPwmInversion(scale(rgb.g)));
    analogWrite(_pinBlue, applyPwmInversion(scale(rgb.b)));
}

Rgb RgbLed::computeTransitionColor() {
    if (_selectedTransition == Transition::None)
        return _rgb;

    const unsigned long elapsed = millis() - _transitionTime;
    constexpr float duration = 500.0f;
    const float t = min(static_cast<float>(elapsed) / duration, 1.0f);

    Rgb rgb{};

    switch (_selectedTransition) {
        case Transition::Fade:
            rgb.r = linearInterpolate(_last_rgb.r, _rgb.r, t);
            rgb.g = linearInterpolate(_last_rgb.g, _rgb.g, t);
            rgb.b = linearInterpolate(_last_rgb.b, _rgb.b, t);
            if (t >= 1.0f) {
                _selectedTransition = Transition::None;
            }
            break;
        case Transition::None:
        default:
            rgb = _rgb;
            break;
    }

    return rgb;
}

Rgb RgbLed::computeEffectColor(Rgb rgb) {
    switch (_selectedEffect) {
        case Effect::Blink: {
            const unsigned long duration = _blinkState ? _blinkOnTimeMs : _blinkOffTimeMs;

            if (millis() - _effectMemoryTime < duration) {
                if (!_blinkState)
                    rgb.r = rgb.g = rgb.b = 0;
                break;
            }
            _blinkState = !_blinkState;
            _effectMemoryTime = millis();
        }
        break;
        case Effect::Pulse: {
            const unsigned long now = millis();
            const float phase = static_cast<float>(now % static_cast<unsigned long>(_pulsePeriod)) / _pulsePeriod;
            const float brightness = (sin(phase * 2.0f * static_cast<float>(M_PI)) + 1.0f) / 2.0f;

            Serial.println(sin(millis() * PI));
            rgb.r *= brightness;
            rgb.g *= brightness;
            rgb.b *= brightness;
        }
        break;
        case Effect::Glitch: {
            if (millis() - _effectMemoryTime > 100) {
                _effectMemoryTime = millis();

                if (random(0, 100) < 70) {
                    // 70% chance: show random glitch
                    rgb.r = random(256);
                    rgb.g = random(256);
                    rgb.b = random(256);
                } else {
                    // 30% chance: show target color
                    rgb = _rgb;
                }
                _glitch_rgb = rgb;
            } else {
                rgb = _glitch_rgb;
            }

        }
        break;
        default:
            break;
    }

    return rgb;
}

void RgbLed::setTransition(const Transition transition) {
    _selectedTransition = transition;
    _transitionTime = millis();
}

void RgbLed::setEffect(const Effect effect) {
    _selectedEffect = effect;
}

void RgbLed::setLuminosity(float luminosity) {
    luminosity = constrain(luminosity, 0.0f, 1.0f);
    _luminosity = luminosity;
}

void RgbLed::setBlinkOnDuration(const uint8_t duration) {
    _blinkOnTimeMs = duration;
}

void RgbLed::setBlinkOffDuration(const uint8_t duration) {
    _blinkOffTimeMs = duration;
}

void RgbLed::setPulsePeriod(const float pulsePeriod) {
    _pulsePeriod = pulsePeriod;
}

void RgbLed::off() {
    _isOn = false;
}

void RgbLed::on() {
    _isOn = true;
}

uint8_t RgbLed::applyPwmInversion(uint8_t color) const {
    return _pwmInverted ? 255 - color : color;
}

uint8_t RgbLed::linearInterpolate(const uint8_t a, const uint8_t b, float t) const {
    t = constrain(t, 0.0f, 1.0f);
    return static_cast<uint8_t>(static_cast<float>(a) + static_cast<float>(b - a) * t);
}
