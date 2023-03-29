#include "gemt_interface.h"
#include "nRFTest.h"

GEMTmenu MainMenu(2);



// DEBUG
void dummyTest (void)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.println("HI BBY");
  display.display();
  delay(2000);
};

void setup(void)
{
  // Add items to your menus here:
  MainMenu.setFirstLine("GEMT Main Menu");
  MainMenu.addItem("nRF Test", *dummyTest);
  MainMenu.addItem("9G Servo Test", dummyTest);

  //...
  MainMenu.bootUp();
  Serial.begin(115200); // DEBUGGING
}

int main(void) 
{
  init(); // Must intialize the arduino firmware
  setup();

  while(true)
  {
    startGEMT(MainMenu);
    delay(3);
  }
  
  return 0;
}