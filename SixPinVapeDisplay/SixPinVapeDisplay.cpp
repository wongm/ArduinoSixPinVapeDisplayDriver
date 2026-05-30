#include "SixPinVapeDisplay.h"

// ==========================================
// DIRECT SEGMENT MAPPING TABLE
// ==========================================
const  SixPinVapeDisplaySegment  SEGMENT_MAP[20] = {
  // HIGH Pin,  LOW Pin      // Segment ID
  {   -1,         -1   },   // 0: (Unused)
  
  // Digit 1 (Tens)
  {  1,        0   },   // 1  
  {  2,        0   },   // 2  
  {  3,        0   },   // 3  
  {  4,        0   },   // 4  
  {  5,        0   },   // 5  
  {  0,        1   },   // 6 
  {  2,        1   },   // 7
  
  // Digit 2
  {  3,        1   },   // 8
  {  4,        1   },   // 9
  {  5,        1   },   // 10 
  {  0,        2   },   // 11
  {  1,        2   },   // 12 
  {  3,        2   },   // 13 
  {  4,        2   },   // 14 

  // Symbols
  {  5,        2   },   // 15 (Lightning Bolt)
  {  0,        3   },   // 16 (Percentage %)4

  // Progress Bar & Percentage (From your snippet: A2 was LOW)
  {  1,        3   },   // 17 (Waterdrop top)
  {  2,        3   },   // 18 (Waterdrop middle)
  {  4,        3   }    // 19 (Waterdrop bottom)
};

// Standard 7-Segment lookup map for numbers 0-9
// Bit order: 0b0GFEDCBA
const byte  FONT_7SEG[10] = {
  0b00111111, // 0 (A, B, C, D, E, F)
  0b00000110, // 1 (B, C)
  0b01011011, // 2 (A, B, D, E, G)
  0b01001111, // 3 (A, B, C, D, G)
  0b01100110, // 4 (B, C, F, G)
  0b01101101, // 5 (A, C, D, F, G)
  0b01111101, // 6 (A, C, D, E, F, G)
  0b00000111, // 7 (A, B, C)
  0b01111111, // 8 (A, B, C, D, E, F, G)
  0b01101111  // 9 (A, B, C, D, F, G)
};

// Global state tracking which segments should be ON
bool segmentState[20] = {false}; 

// Constructor: Capture the runtime pins into our array
SixPinVapeDisplay::SixPinVapeDisplay(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4, uint8_t p5, uint8_t p6) {
    _pins[0] = p1; // Corresponds to Index 0 in your map
    _pins[1] = p2; // Corresponds to Index 1 in your map
    _pins[2] = p3; // Corresponds to Index 2 in your map
    _pins[3] = p4; // Corresponds to Index 3 in your map
    _pins[4] = p5; // Corresponds to Index 4 in your map
    _pins[5] = p6; // Corresponds to Index 5 in your map
    
    // Initialize default internal states
    _currentNumber     = 0;
    _lightningActive   = false;
    _percentageActive   = false;
    _waterdropLevel = 0;
}

// Setup logic: Sets up the mapped pins as outputs
void SixPinVapeDisplay::begin() {
    // Dynamically set all 6 mapped hardware pins to OUTPUT
    for (uint8_t i = 0; i < 6; i++) {
        pinMode(_pins[i], INPUT);
    }
}

void SixPinVapeDisplay::setNumber(int number) {
    _currentNumber = number;
    
    // Constrain number to safety limits of a 2-digit grid
    if (number < 0)  number = 0;
    if (number > 99) number = 99;

    int tens = number / 10;
    int units = number % 10;

    // Retrieve bit patterns from lookup font array
    byte tensPattern = FONT_7SEG[tens];
    byte unitsPattern = FONT_7SEG[units];

    // Decode and map bits straight to Digit 1 states (IDs 1 through 7)
    for (int i = 0; i < 7; i++) {
        segmentState[1 + i] = (tensPattern >> i) & 1;
    }

    // Decode and map bits straight to Digit 2 states (IDs 8 through 14)
    for (int i = 0; i < 7; i++) {
        segmentState[8 + i] = (unitsPattern >> i) & 1;
    }
}

void SixPinVapeDisplay::showPercentage(bool active) {
    _percentageActive = active;

    segmentState[15] = active;
}

void SixPinVapeDisplay::showLightning(bool active) {
    _lightningActive = active;

    segmentState[16] = active;
}

void SixPinVapeDisplay::setWaterdrop(int level) {
    _waterdropLevel = level;

    segmentState[17] = (level >= 3);
    segmentState[18] = (level >= 2);
    segmentState[19] = (level >= 1);
}

// Low-level drive/refresh logic
void SixPinVapeDisplay::driveDisplay() {
    for (int i = 1; i <= 19; i++) {
        if (segmentState[i]) {
            // Example: How to safely process and look up the pins for Segment 1
            int highIdx = SEGMENT_MAP[i].highPinIdx;
            int lowIdx  = SEGMENT_MAP[i].lowPinIdx;

            if (highIdx == -1 || lowIdx == -1) continue; 
            
            int hPin = _pins[highIdx];
            int lPin  = _pins[lowIdx];

            pinMode(hPin, OUTPUT);
            pinMode(lPin, OUTPUT);

            digitalWrite(hPin, HIGH);
            digitalWrite(lPin, LOW);

            delayMicroseconds(900); 

            digitalWrite(hPin, LOW);
            pinMode(hPin, INPUT);
            pinMode(lPin, INPUT);
        }
    }
}