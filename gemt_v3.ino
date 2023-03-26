#include "gemt_interface.h"
#include "nRFTest.h"

GEMTmenu MainMenu(1);

void setup(void)
{
  delay(500);

  // Add items to your menus here:
  // Brings up error 
  MainMenu.addItem("nRF Test", runNRFtest);

  //...

  MainMenu.bootUp();
  //Serial.begin(115200); // DEBUGGING
}

int main(void) 
{
  init(); // Must intialize the arduino firmware
  setup();

  while(true)
  {
    MainMenu.run();
    delay(3);
  }
  
  return 0;
}