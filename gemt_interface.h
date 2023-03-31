#ifndef gemt_interface_h
#define gemt_interface_h

#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EncoderButton.h>
#include <Fonts/Org_01.h>
//#include "HardwareSerial.h" //  Wht we have this? -jc

namespace
{
  enum encoderSWPins 
  {
    // Note: If using jumper wires make sure pins are well spaced out.
    // rotary encoder is super noisy and registers false clicks among other issues
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
}

typedef void (*funcPtr)(void);


//  Desc: Provides core helper functions to children
class GEMTbase
{
  public:
    // Destructor, don't really need but good practice 
    virtual ~GEMTbase(){} 

    // Helper: Function to quickly call actions required to prep screen for printing
    void displayPrep(void);

    // Helper: Create function for clarity in resetting clicked
    void resetClicked(void);

    // Helper: displays first line
    void setFirstLine(String title);

  protected:
    // Protected default constructor since we dont create an instance of base class
    GEMTbase(){} 
    
    // Displays first line
    void showFirstLine(void);

    // Protected value for what first line of screen should say
    String _firstLine; 
};

//  Desc: Class for setting up GEMT menus
class GEMTmenu : public GEMTbase
{
  public:
    // explicit ensures that the constructor can only be used if has input params and prevents potential unintended automatic compiler conversions
    explicit GEMTmenu(int numberOfItems) : numberOfMenuItems(numberOfItems) {}; // the : is for initilizer list

    // Only need to execute once.
    // Defines boot up actions: Check hardware, set interupt handlers, show logo
    void bootUp(void);

    // Set line items of menu. Must be at least len of  instatiated value
    void addItem(String itemName, funcPtr selectionFunction);

    void run(void);

    const unsigned short int numberOfMenuItems;

  private:
    funcPtr _selectionActions[6]; // Max 6 for now
    String _itemIds[6];
    unsigned short int _currIndex = 0;
};

// Desc: Class to setup all screens and functions related to testing. 
// When test terminate the return to previous Current Menu pointer
class GEMTtest : public GEMTbase
{
  public:
    explicit GEMTtest() {};

    void setInfoTitle(String title);

    // Desc: Sets info msgs line by line, with a cap of 6 msgs
    // TODO: Add length handling
    void setInfoMsgLine(String msg);

    // Desc:  Test if user wishes to proceed or not.
    //        Sets a info screen with selection options
    bool showInfoScreen(void);
    
    // Desc: Displays a screen with test title, updateble test feedback, and a "Done" selection
    void showStaticTestScreen(funcPtr moduleTest);

    // Desc: Displays a screen with test title, user updateble numbers, and a "Done" selection
    void showInteractiveTestScreen(funcPtr moduleTest);

    // Desc: Manually updateable function to display a message in center of screen
    void testFeedback(String msg);
    
  private:

};

// Sets current menu pointer and drives menu functions
void startGEMT(GEMTmenu& StartingMenu);

// Desc: Used to update Current menu tracker
// Extern since being used across source files
extern void updateMenu(GEMTmenu& NextMenu);

// Encoder handler functions
void onEb1Encoder(EncoderButton& eb);
void onEb1Clicked(EncoderButton& eb);


#endif 