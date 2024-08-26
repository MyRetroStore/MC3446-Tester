# MC3446 Chip Tester
Arduino based tester to check MC3446 chips used in many old retro computers like the Commodore PET.

## Introduction
Modern chip testers like the TL866 are unable test MC3446 bus transceivers.  If you are troubleshooting a Commodore PET and you don't have a known good computer to test the chip in, then makes it difficult to troubleshoot.


I built a shield for the Arduino Mega 2560 which tests all the logic levels on the MC3446 (tranmitting and receiving) and provides feedback via the serial monitor.



![Shield](https://github.com/MyRetroStore/MC3446-Tester/blob/main/mc3446-tester.jpg?raw=true)


## Usage

Wire up the Arduino to the MC3446 pins as per the following mapping:

| MC3446  | Arduino Mega | MC3446  | Arduino Mega |
| ---- | ---- | ---- | ---- | 
| Pin 1 (1R) | D22 | Pin 16 (VCC) | 5V |
| Pin 2 (1B) | D23 | Pin 15 (4R) | D11|
| Pin 3 (1D)| D30  | Pin 14 (4B) | D10|
| Pin 4 (1,2,3S)| D25  |    Pin 13 (4D) |      D9|
| Pin 5 (2D)    | D26  |    Pin 12 (4S) |     D8|
| Pin 6 (2B)    | D27  |    Pin 11 (3D) |     D7|
| Pin 7 (2R)    | D28  |    Pin 10 (3B) |     D6|
| Pin 8 (GND)   | GND     |    Pin 9  (3R) |     D5|

This Arduino sketch was built and tested using Arduino IDE 1.8.19 but should work with later versions.

You can also use a Arduino UNO but will have to change the pin mappings. 

Upload the code and set the serial monitor to 115200. 

Remove power and insert the chip to be tested. 

Apply power and open the serial monitor.

Receiving and tranmitting tests are done 10 times. 



![Serial Output](https://github.com/MyRetroStore/MC3446-Tester/blob/main/serial-output.png?raw=true)


If there is a error the program will display the information and halt. 

![Error](https://github.com/MyRetroStore/MC3446-Tester/blob/main/serial-error.png?raw=true)

## License
CC BY-NC-SA 4.0


[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/H2H8RDX9W)


