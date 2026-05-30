#include <SixPinVapeDisplay.h>

// Instantiate the library object
SixPinVapeDisplay display(A5, A4, A3, A2, A1, A0);
//SixPinVapeDisplay display(A0, A1, A2, A3, A4, A5);

void setup() {
    // Run the library's internal pin setup logic
    display.begin();
}

void loop() {

    // Set initial display elements
    display.setNumber(57);
    display.showPercentage(false);
    display.showLightning(true);
    display.setWaterdrop(1);
    
    // Keeps the display refreshed and drawn continuously
    display.driveDisplay();
}
