#pragma once

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EncoderButton.h>

namespace
{
  // Note: If using jumper wires, make sure pins are well spaced out.
  // Rotary encoder is super noisy and registers false clicks among other issues.
  enum encoderSWPins
  {
    pinA = 19, // CLK
    pinB = 15, // DT
    pinSW = 26 // SW
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

  #define displayRowLimit 8
  #define displayColLimit 21
  #define maxItems 6 // 6 ensures we have a free line at the top and bottom of the screen
}

typedef void (*funcPtr)(void);

/**
 * @class GEMTbase
 * @brief Provides core helper functions to children.
 */
class GEMTbase
{
public:
  // Destructor, don't really need but good practice
  virtual ~GEMTbase(){};

  /**
   * @brief Function to quickly call actions required to prep the screen for printing.
   */
  void displayPrep(void);

  /**
   * @brief Create a function for clarity in resetting clicked.
   */
  void resetClicked(void);

  /**
   * @brief Used to reset index and members after the test.
   */
  virtual void resetMembers(void);

  /**
   * @brief Displays the first line.
   * @param title The string for the first line.
   */
  void setFirstLine(String title);

  /**
   * @brief Used to set the current screen's turn boundaries.
   * @param lower The lower boundary.
   * @param upper The upper boundary.
   */
  void setTurnBounds(int lower, int upper);

protected:
  // Protected default constructor since we don't create an instance of the base class
  GEMTbase() {}

  // Protected value for what the first line of the screen should say for any screen
  String _firstLine;

  // Index for populating menu items
  unsigned short int _currIndex = 0;
};

/**
 * @class GEMTmenu
 * @brief Class for setting up GEMT menus.
 */
class GEMTmenu : public GEMTbase
{
public:
  /**
   * @brief Explicit constructor, ensures that the constructor can only be used if it has input params and prevents potential unintended automatic compiler conversions.
   * @param numberOfItems The number of items in the menu.
   */
  explicit GEMTmenu(int numberOfItems) : numberOfMenuItems(numberOfItems) {}; // the : is for initializer list

  /**
   * @brief Only needs to be executed once. Defines boot-up actions: check hardware, set interrupt handlers, show logo.
   */
  void bootUp(void);

  /**
   * @brief Set line items of the menu. Must be at least the length of the instantiated value.
   * @param itemName The name of the menu item.
   * @param selectionFunction The function to be executed when the item is selected.
   */
  void addItem(String itemName, funcPtr selectionFunction);

  /**
   * @brief Runs the menu.
   */
  void run(void);

  const unsigned short int numberOfMenuItems;

private:
  funcPtr _selectionActions[maxItems];
  String _itemIds[maxItems];
};

/**
 * @class GEMTtest
 * @brief Class to set up all screens and functions related to testing. 
 * When tests terminate, they return to the previous Current Menu pointer.
 */
class GEMTtest : public GEMTbase
{
public:
  /**
   * @brief Explicit constructor.
   */
  explicit GEMTtest(){};

  /**
   * @brief Sets info messages line by line, with a cap of 6 messages.
   * @param msg The message to be displayed.
   */
  void setInfoMsgLine(String msg);

  /**
   * @brief Test if the user wishes to proceed or not.
   * True = Proceed, False = Return to previous menu.
   * Sets an info screen with selection options.
   * @return Returns true if the user wishes to proceed, false otherwise.
   */
  bool showInfoScreen(void);

  /**
   * @brief Displays a screen with test title, updateable test feedback line, and a "Done" selection.
   * @param moduleTest The function to be executed for the test.
   */
  void showStaticTestScreen(funcPtr moduleTest);

  /**
   * @brief Sets up to 6 short messages.
   * @param msg The message to be displayed.
   */
  void setStaticTestFeedbackLine(String msg);

  /**
   * @brief Will display static test feedback messages after set.
   * @param displayDuration The duration of the display in milliseconds.
   */
  void showStaticTestFeedback(int displayDuration);

  /**
   * @brief Displays a screen with test title, user-updatable numbers line, "Run" selection, and "Done" selection.
   * @param writeFunction The function to be executed for writing values.
   * @param unitID The unit identifier string.
   * @param lowerBound The lower boundary of the value.
   * @param upperBound The upper boundary of the value.
   */
  void showInteractiveTestScreen(funcPtr writeFunction, String unitID, int lowerBound, int upperBound);

  /**
   * @brief Updates the interactive value.
   */
  void updateIntereactiveValue(void);

  int interactiveValue[1] = {0};

  bool testingComplete = false;

private:
  /**
   * @brief Reset index and feedback display.
   */
  virtual void _resetMembers(void);

  // Initialize these two arrays with empty strings for printing
  String _testFeedbackMsgs[maxItems] = {"", "", "", "", "", ""};
  String _infoMsgs[maxItems] = {"", "", "", "", "", ""};
};

/**
 * @brief Sets the current menu pointer and drives menu functions.
 * @param StartingMenu The starting menu to be displayed.
 */
void startGEMT(GEMTmenu& StartingMenu);

/**
 * @brief Used to update the Current menu tracker.
 * @param NextMenu The next menu to be displayed.
 */
extern void updateMenu(GEMTmenu& NextMenu);

// Encoder handler functions
extern void onEb1Encoder(EncoderButton& eb);
extern void onEb1Clicked(EncoderButton& eb);
