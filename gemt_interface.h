#ifndef gemt_interface_h
#define gemt_interface_h

#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EncoderButton.h>
#include <Fonts/Org_01.h>
#include "HardwareSerial.h"

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

  Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, screenReset);

  EncoderButton eb1(pinA, pinB, pinSW);

  // Used to set selection action
  typedef void (*func)(void);
}

// Handle 'click' events
void onEb1Clicked(EncoderButton& eb);

// Handle the 'encoder' event
void onEb1Encoder(EncoderButton& eb); 


//  Desc: Provides core helper functions to children
class GEMTbase
{
  public:
    // Destructor, don't really need but good practice 
    virtual ~GEMTbase(){} 

    // Set first line of any screen    
    void setFirstLine(String& title);
  
  protected:
    // Protected default constructor since we dont create an instance of base class
    GEMTbase(){} 
    
    // Set to true when display booted up from a menu object
    bool _hasBeenBootedUp = false; 
    
    // Function to quickly call actions required to prep screen for printing
    void displayPrep(void);

    // Create function for clarity in resetting clicked
    void resetClicked(void);

   

  private:
  
};

//  Desc: Class for setting up GEMT menus
class GEMTmenu : public GEMTbase
{
  public:
    // explicit ensures that the constructor can only be used if has input params and prevents potential unintended automatic compiler conversions
    explicit GEMTmenu(int numberOfItems) : _items(numberOfItems) {}; // the : is for initilizer list
    
    // Defines boot up actions: Check hardware, set interupt handlers, show logo
    // only need to do once on main menu case
    void bootUp(void);

    // Sets Current Menu pointer to class executed.
    // only need to execute once for main menu implementation
    void run(void);

    // Set line items of menu. Must be at least instatiated value
    void addItem(String itemName, func selectionFunction);

  private:
    const int _items;
    String _itemID[6]; // Maxing out items to 10 for now
   // Figure out how to do allocations at compile time wit _items size
};

// Desc: Class to setup all screens and functions related to testing. 
// When test terminate the return to previous Current Menu pointer
class GEMTtest : public GEMTbase
{
  public:
    explicit GEMTtest(String& infoText) : _infoMsg(infoText) {};

    // Each test case object will have it's own info screen to display
    void showInfoScreen(void);

    bool userSelectedOk = false;
    
  protected:

  private:
    const String _infoMsg;
};

#endif 