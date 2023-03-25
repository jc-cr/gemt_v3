#ifndef gemt_interface_h
#define gemt_interface_h

#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EncoderButton.h>
#include <Fonts/Org_01.h>
#include "HardwareSerial.h"

//========================================================================
// Workspace Definitions
//========================================================================
namespace
{
  volatile uint8_t ebState = 0; // Current state (Position) of the encoder. Max by uint8 is 255
  volatile bool clicked = false; // Updated on encoder "click" case, must reset after use 
  uint8_t clickedItemNumber = 0;

  // Note: If using jumper wires make sure pins are well spaced out.
  // rotary encoder is super noisy and registers false clicks among other issues
  enum encoderSWPins 
  {
    pinA = 19 , // CLK
    pinB = 2, // DT
    pinSW = 38 // SW
  };

  enum oledDisplayPins
  {
    screenWidth = 128,
    screenHeight = 64,
    screenAddress = 0x3C, // i2c Address
    screenReset = -1      // -1 since sharing Arduino reset pin
  };

  typedef void (*func)(void);
  //========================================================================
  // Hardware Intilzers
  //========================================================================
  // Display init
  Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, screenReset);

  // EncoderSW init
  EncoderButton eb1(pinA, pinB, pinSW);
}

//========================================================================
// Encoder Handlers (Interrupt functions)
//========================================================================  
// On click, the global selection variable gets updated with
// value of where it was selcted
void onEb1Clicked(EncoderButton& eb);

// A function to handle the 'encoder' event
void onEb1Encoder(EncoderButton& eb); 

//========================================================================
// GEMT Class Definitions 
//========================================================================

/* 
  Base Class:
    - Not constructed just used to create shared implementation features for subclasses
*/

class GEMTdisplay
{
  public:
    virtual ~GEMTdisplay(){} // Destructor, don't really need but good practice 
  
  protected:
    GEMTdisplay(){} // Protected default constructor since we dont create an instance of base class
    bool _hasBeenBootedUp = false; // Set to true when display booted up from a menu object

    // Function to quickly call actions required to prep screen for printing
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

  private:
    

};

// Possible implementation
/*
  Menu class for GEMT display. Has functions for:
    - Add entries + associated click action functions
    - Auto entry numbering
    - 
*/
class GEMTmenu : public GEMTdisplay
{
  public:
    explicit GEMTmenu(int numberOfItems)
    : _items(numberOfItems) // explicit ensures that the constructor can only be used if has input params and prevents potential unintended automatic compiler conversions
    {

    } 

    // Defines boot up actions: Check hardware, set interupt handlers, show logo
    // only need to do once on main menu case
    void bootUp(void);
    void addItem(String itemName, func selectionFunction)
    {
      
    }

  private:
    const int _items;
    String _itemID[10]; // Maxing out items to 10 for now
   // Figure out how to do allocations at compile time wit _items size
   
    
};

class GEMTtestScreen : public GEMTdisplay
{
  public:

    explicit GEMTtestScreen(String infoText) : _infoMsg(infoText) {};

    // Each test case will have it's own info screen to display

    bool showInfoScreen(void);

  protected:

  private:
    const String _infoMsg;
};


/*

*/

#endif 