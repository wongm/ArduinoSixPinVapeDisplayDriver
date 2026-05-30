# ArduinoSixPinVapeDisplayDriver
Arduino code to drive a six pin LED display found in disposable vapes. 

## Hardware

An example of these displays is this. I bent up a 6 pin piece of 2.54mm pin header to line up with the 2mm spacing of the PCB.

<img width="2048" height="1536" alt="486662626_10161677579572585_1073964685747046295_n" src="https://github.com/user-attachments/assets/f283f031-de0f-47ae-bfad-08f95ce72550" />

## Code

Two Arduino sketches:

- vape_screen_with_methods uses inline code to drive the display
- vape_display_using_library is an improved version that uses the "SixPinVapeDisplay" library in this repository to do the same job.

Methods in the library are:

- `setNumber(int number)` to display a two digit number
- `void showPercentage(bool active)` controls the "percentage" icon
- `void showLightning(bool active)` controls the "charging" icon
- `void setWaterdrop(int level)` controls the three level "waterdrop" icon
- `driveDisplay()` updates the screen

