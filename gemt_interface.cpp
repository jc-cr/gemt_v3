#include "gemt_interface.h"
#include <Wire.h>
#include "HardwareSerial.h"
#include <Adafruit_GFX.h>
#include <Fonts/Org_01.h>
#include <Adafruit_SSD1306.h>
#include <EncoderButton.h>
#include "Arduino.h"
#include "Arduino.h"

//========================================================================
// Workspace Definitions
//========================================================================

// Updated on encoder "click" case, must reset after use 
namespace // Anon namespace, better than static
{
  bool clicked = false;
}

unsigned short int ebState = 0; // Current state (Position) of the encoder 
unsigned short int currentScrollLimit = 0; // Updated within displayMenu function

enum encoderSWPins 
{
  pinA = 19 , // CLK
  pinB = 2, // DT
  pinSW = 8 // SW
};

enum oledDisplayPins
{
  screenWidth = 128,
  screenHeight = 64,
  screenAddress = 0x3C, // i2c Address
  screenReset = -1      // -1 since sharing Arduino reset pin
};

//====
//========================================================================
// Initializers
//========================================================================

// Display init
Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, screenReset);

// EncoderSW init
EncoderButton eb1(pinA, pinB, pinSW);


//========================================================================
// Encoder Handlers
//========================================================================
  
// On click, the global selection variable gets updated with
// value of where it was selcted
void onEb1Clicked(EncoderButton& eb)
{
  // Set selection value to current state
  clicked = true;
  
  // DEBUG - Delete in actual proram as Serial printing slows down interrupts
  //Serial.println("CLICKED!");
}

// A function to handle the 'encoder' event
void onEb1Encoder(EncoderButton& eb) 
{
  // Filter latge spikes from noise
  if(eb.increment() > 4)
  {
    eb.resetPosition(eb.position()); // Reset back to startin pos
  }

  // Reset if encoder goes past active Menu limit
  if (abs(eb.position()) >= currentScrollLimit)
  {
    eb.resetPosition(0);
  }

  ebState = abs(eb.position());
  //Serial.println(ebState);
}
 
//========================================================================
// Helper functions
//========================================================================

// Function to quicly call actions required to prep screen for printing
void displayPrep(void)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
}

// Create function for clarity
void resetClicked(void)
{
  clicked = 0;
}

// Class impelemntaiton for gemt interface
// create a kewords text file to have kewords highlighted in code
// Note: https://docs.arduino.cc/learn/contributions/arduino-creating-library-guide

GEMTmenu::GEMTmenu(void)
{

}
