/*
  Direct-Mapped Charlieplex/Matrix Driver with Screen Controls
*/

struct Segment {
  int highPin; // The pin that must be HIGH
  int lowPin;  // The pin that must be LOW
};

// ==========================================
// PHYSICAL PIN CONFIGURATION
// (I'm using the Arduino analog pins A0-A5 in reverse order here)
// ==========================================
const int pA6 = A0;
const int pA5 = A1;
const int pA4 = A2;
const int pA3 = A3;
const int pA2 = A4;
const int pA1 = A5;

// ==========================================
// DIRECT SEGMENT MAPPING TABLE
// ==========================================
const Segment SEGMENT_MAP[20] = {
  // HIGH Pin,  LOW Pin      // Segment ID
  {   -1,         -1   },   // 0: (Unused)
  
  // Digit 1 (Tens)
  {  pA2,        pA1   },   // 1  
  {  pA3,        pA1   },   // 2  
  {  pA4,        pA1   },   // 3  
  {  pA5,        pA1   },   // 4  
  {  pA6,        pA1   },   // 5  
  {  pA1,        pA2   },   // 6 
  {  pA3,        pA2   },   // 7
  
  // Digit 2
  {  pA4,        pA2   },   // 8
  {  pA5,        pA2   },   // 9
  {  pA6,        pA2   },   // 10 
  {  pA1,        pA3   },   // 11
  {  pA2,        pA3   },   // 12 
  {  pA4,        pA3   },   // 13 
  {  pA5,        pA3   },   // 14 

  // Symbols
  {  pA6,        pA3   },   // 15 (Lightning Bolt)
  {  pA1,        pA4   },   // 16 (Percentage %)4

  // Progress Bar & Percentage (From your snippet: A2 was LOW)
  {  pA2,        pA4   },   // 17 (Waterdrop top)
  {  pA3,        pA4   },   // 18 (Waterdrop middle)
  {  pA5,        pA4   }    // 19 (Waterdrop bottom)
};

// Standard 7-Segment lookup map for numbers 0-9
// Bit order: 0b0GFEDCBA
const byte FONT_7SEG[10] = {
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

void setup() {
  allPinsOff();
}

void loop() {
  // ==========================================
  // EXAMPLE CONTROL DEMO
  // ==========================================
  
  setNumber(42);         // Display the number 42
  setPercentage(true);   // Show the percentage sign
  setLightning(false);   // Turn off the lightning bolt
  setWaterdrop(2);       // Light up exactly 2 bars of the waterdrop

  // Must call this continuously in your loop to keep the display illuminated
  driveDisplay();
}

// ==========================================
// HIGH-LEVEL INTERFACE METHODS
// ==========================================

// Enable or disable the Percentage icon
void setPercentage(bool enable) {
  segmentState[15] = enable;
}

// Enable or disable the Lightning Bolt icon
void setLightning(bool enable) {
  segmentState[16] = enable;
}

// Set waterdrop level (0 = completely off, 1 = low, 2 = mid, 3 = full)
void setWaterdrop(int level) {
  segmentState[17] = (level >= 3);
  segmentState[18] = (level >= 2);
  segmentState[19] = (level >= 1);
}

// Takes any integer from 0 to 99 and splits it across both digits
void setNumber(int num) {
  // Constrain number to safety limits of a 2-digit grid
  if (num < 0)  num = 0;
  if (num > 99) num = 99;

  int tens = num / 10;
  int units = num % 10;

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

// ==========================================
// CORE LOW-LEVEL DRIVER ENGINE
// ==========================================

void allPinsOff() {
  pinMode(pA1, INPUT);
  pinMode(pA2, INPUT);
  pinMode(pA3, INPUT);
  pinMode(pA4, INPUT);
  pinMode(pA5, INPUT);
  pinMode(pA6, INPUT);
}

void driveDisplay() {
  for (int i = 1; i <= 19; i++) {
    if (segmentState[i]) {
      int hPin = SEGMENT_MAP[i].highPin;
      int lPin = SEGMENT_MAP[i].lowPin;
      
      if (hPin == -1 || lPin == -1) continue; 

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
