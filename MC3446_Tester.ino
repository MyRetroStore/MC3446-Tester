/*
  MC3446 Quadruple Bus Transceiver

  Arduino code to test MC3446 bus transceiver chips which are used on the Commodore PET
  I build this circuit on my Mega but if you change the Arduino pin numbering there is no reason why it should not work on the Uno. 
  
  26 August
  Version 1.0
  myretrostore.co.uk
  Compile with Arduido IDE 1.8.19

  Pin Mappings:
  MC3446          Arduino    |     MC3446           Arduino
  Pin 1 (1R)     D22         |    Pin 16 (VCC)
  Pin 2 (1B)     D23         |    Pin 15 (4R)      D11
  Pin 3 (1D)     D30         |    Pin 14 (4B)      D10
  Pin 4 (1,2,3S) D25         |    Pin 13 (4D)      D9
  Pin 5 (2D)     D26         |    Pin 12 (4S)      D8
  Pin 6 (2B)     D27         |    Pin 11 (3D)      D7
  Pin 7 (2R)     D28         |    Pin 10 (3B)      D6
  Pin 8 (GND)                |    Pin 9  (3R)      D5

  Datasheet: https://www.alldatasheet.com/datasheet-pdf/pdf/127175/TI/MC3446.html

  Dependencies: DIO2 
  https://github.com/Locoduino/DIO2
  
*/

#include <DIO2.h>

#define VERSION "Version 1.0"

// Pin definitions
#define Pin_1R 22
#define Pin_1B 23
#define Pin_1D 30
#define Pin_123S 25
#define Pin_2D 26
#define Pin_2B 27
#define Pin_2R 28
#define Pin_3R 5
#define Pin_3B 6
#define Pin_3D 7
#define Pin_4S 8
#define Pin_4D 9
#define Pin_4B 10
#define Pin_4R 11

byte InputD[] = {Pin_1D, Pin_2D, Pin_3D, Pin_4D };
byte OutputR[] = {Pin_1R, Pin_2R, Pin_3R, Pin_4R };
byte IO_B[] = {Pin_1B, Pin_2B, Pin_3B, Pin_4B};
int readPin;

//---------------------------------------------------
// Set B pins as output
//---------------------------------------------------
void B_Output()
{
  for (int i = 0; i < sizeof(IO_B); i++) {
    pinMode2(IO_B[i], OUTPUT);
  }
}

//---------------------------------------------------
// Set B pins as input
//---------------------------------------------------
void B_Input()
{
  for (int i = 0; i < sizeof(IO_B); i++) {
    pinMode2(IO_B[i], INPUT_PULLUP);
  }
}

//---------------------------------------------------
// Initialise pins as innput/ouput
//---------------------------------------------------
void InitialisePins()
{
  //Sets to default transmit S & D low so all outputs are low
  pinMode2(Pin_123S, OUTPUT); //Set pin a output
  pinMode2(Pin_4S, OUTPUT);   //Set pin a output

  digitalWrite2(Pin_123S, LOW);
  digitalWrite2(Pin_4S, LOW);

  //Set 1D, 2D, 3D, 4D as outputs
  for (int i = 0; i < sizeof(InputD); i++) {
    pinMode2(InputD[i], OUTPUT);
    digitalWrite2(InputD[i], LOW);
  }

  //Set 1R, 2R, 3R, 4R as input
  for (int i = 0; i < sizeof(OutputR); i++) {
    pinMode2(OutputR[i], INPUT_PULLUP);
  }
}

//---------------------------------------------------
// There was a error. Print info and pin number
//---------------------------------------------------
void Error(byte pin_number, boolean pin_result, byte output_type) {

  //output_type=0 for B
  //output_type=1 for R

  String Type;

  if (output_type == 0) {
    Type = "B";
  } else {
    Type = "R";
  }

  Serial.print("\nError: Output ");
  Serial.print(pin_number+1);
  Serial.print(Type);
  Serial.print(" reading ");

  if (pin_result) {
    Serial.print("HIGH");  
  } else {
    Serial.print("LOW");   
  }

  while (1);
}

//---------------------------------------------------
// Test transmit
//---------------------------------------------------
void testTransmit()
{
  Serial.println("Testing transmitting");

  //With defaults to transmit and inputs LOW, check that outputs are LOW
  // Test that outputs are LOW
  Serial.println("Testing R is LOW when S&D set LOW");
  for (byte i = 0; i < sizeof(OutputR); i++) {
    readPin = digitalRead2(OutputR[i]);

    if (readPin == HIGH) { 
      Error(i, HIGH, 1);
    }
  }

  Serial.println("Testing B is LOW when S&D set LOW");
  for (int i = 0; i < sizeof(IO_B); i++) {
    readPin = digitalRead2(IO_B[i]);

    if (readPin == HIGH) {
      Error(i, HIGH, 0);
    }
  }

  // Set D HIGH
  for (int i = 0; i < sizeof(InputD); i++) {
    digitalWrite2(InputD[i], HIGH);
  }

  B_Input();

  //B and R should now be HIGH
  Serial.println("Testing R is HIGH when D set HIGH");
  for (int i = 0; i < sizeof(OutputR); i++) {
    readPin = digitalRead2(OutputR[i]);

    if (readPin == LOW) { 
      Error(i, LOW, 1);
    }
  }

  Serial.println("Testing B is HIGH when D set HIGH");
  for (int i = 0; i < sizeof(IO_B); i++) {
    readPin = digitalRead2(IO_B[i]);

    if (readPin == LOW) { 
      Error(i, LOW, 0);
    }
  }

  Serial.println("Transmit test completed");
}

//---------------------------------------------------
// Test receive
//---------------------------------------------------
void testReceive()
{
  Serial.println("\nTesting receiving");

  B_Output();

  //Set S HIGH
  digitalWrite2(Pin_123S, HIGH);
  digitalWrite2(Pin_4S, HIGH);

  //Set B HIGH
  for (int i = 0; i < sizeof(IO_B); i++) {
    digitalWrite2(IO_B[i], HIGH);
  }

  //Check that R is HIGH
  Serial.println("Testing R is HIGH when S&B set HIGH");
  for (int i = 0; i < sizeof(OutputR); i++) {
    readPin = digitalRead2(OutputR[i]);

    if (readPin == LOW) {
      Error(i, LOW, 1);
    }
  }

  //Set B LOW
  for (int i = 0; i < sizeof(IO_B); i++) {
    digitalWrite2(IO_B[i], LOW);
  }

  //Check that R is LOW
  Serial.println("Checking that R is LOW when B set LOW");
  for (int i = 0; i < sizeof(OutputR); i++) {
    readPin = digitalRead2(OutputR[i]);

    if (readPin == HIGH) { //H
      Error(i, HIGH, 1);
    }
  }
  Serial.println("Receiving tests complete");
}


//---------------------------------------------------
// Setup
//---------------------------------------------------
void setup() {

  Serial.begin(115200);
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB port only

  Serial.println("\n===========================");
  Serial.println("      MC3446 Chip Tester");
  Serial.println("      myretrostore.co.uk");
  Serial.print("          "); Serial.println(VERSION);
  Serial.println("===========================");

  InitialisePins();
}

//---------------------------------------------------
// Main program
//---------------------------------------------------
void loop() {

  for (byte pass = 1; pass < 11; pass++)
  {
    Serial.print("\nPass: ");
    Serial.println(pass);
    testTransmit();
    testReceive();
  }

  Serial.println("\nAll tests passed!");
  while (1);
}
