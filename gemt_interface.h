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

  // Display init
  Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, screenReset);

  // EncoderSW init
  EncoderButton eb1(pinA, pinB, pinSW);

  typedef void (*func)(void);
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
class GEMTbase
{
  public:
    virtual ~GEMTbase(){} // Destructor, don't really need but good practice 
  
  protected:
    GEMTbase(){} // Protected default constructor since we dont create an instance of base class
    
    bool _hasBeenBootedUp = false; // Set to true when display booted up from a menu object
    
    void firstLine(void);

    // Function to quickly call actions required to prep screen for printing
    void displayPrep(void);

    // Create function for clarity
    void resetClicked(void);

  private:

};

class GEMTmenu : public GEMTbase
{
  public:
    // explicit ensures that the constructor can only be used if has input params and prevents potential unintended automatic compiler conversions
    // the : is for initilizer list
    explicit GEMTmenu(int numberOfItems) : _items(numberOfItems) {}; 
    
    void run(void);

    // Defines boot up actions: Check hardware, set interupt handlers, show logo
    // only need to do once on main menu case
    void bootUp(void);

    void addItem(String itemName, func selectionFunction);

  private:
    const int _items;
    String _itemID[10]; // Maxing out items to 10 for now
   // Figure out how to do allocations at compile time wit _items size
};

class GEMTtest : public GEMTbase
{
  public:

    explicit GEMTtest(String infoText) : _infoMsg(infoText) {};

    // Each test case will have it's own info screen to display

    void showInfoScreen(void);

    void staticTitle(String title);

    bool userSelectedOk = false;
    
  protected:

  private:
    const String _infoMsg;
};

#endif 