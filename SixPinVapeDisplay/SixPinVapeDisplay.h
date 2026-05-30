#ifndef SIX_PIN_VAPE_DISPLAY_H
#define SIX_PIN_VAPE_DISPLAY_H

#include <Arduino.h>

struct SixPinVapeDisplaySegment {
    int8_t highPinIdx; // Index (0-5) of the HIGH pin, or -1 if unused
    int8_t lowPinIdx;  // Index (0-5) of the LOW pin, or -1 if unused
};

class SixPinVapeDisplay {
public:
    // Constructor: Pass your Arduino pins here to map them to the display
    SixPinVapeDisplay(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin5, uint8_t pin6);

    // Initialization logic (handles pinModes)
    void begin();

    // Public API methods exposed to callers
    void setNumber(int number);
    void showPercentage(bool active);
    void showLightning(bool active);
    void setWaterdrop(int level);
    
    // Refresh/multiplexing logic called continuously in loop()
    void driveDisplay();

private:
    // Storage for the pin mapping
    uint8_t _pins[6];

    // Internal state variables to track display values
    int  _currentNumber;
    bool  _percentageActive;
    bool _lightningActive;
    int _waterdropLevel;

    bool segmentState[20];
};

#endif