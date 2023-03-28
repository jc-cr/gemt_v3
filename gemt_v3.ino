#include "gemt_interface.h"
#include "nRFTest.h"

GEMTmenu MainMenu(2);

void dummyTest (void){};

void setup(void)
{
  // Add items to your menus here:
  MainMenu.setFirstLine("GEMT Main Menu");
  MainMenu.addItem("nRF Test", runNRFtest);
  MainMenu.addItem("9G Servo Test", dummyTest);

  //...
  MainMenu.bootUp();     // TODO: @jc figure out how to get Current Menu ptr to work again
  //Serial.begin(115200); // DEBUGGING
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