#include "gemt_interface.h"
#include "nRFTest.h"

GEMTmenu MainMenu(2);




void setup(void)
{
  // Add items to your menus here:
  MainMenu.setFirstLine("GEMT Main Menu");
  MainMenu.addItem("nRF Test", dummyTest);
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
