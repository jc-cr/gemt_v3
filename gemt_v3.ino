#include "gemt_interface.h"
#include "nRFTest.h"

GEMTmenu MainMenu(1);

void setup(void)
{
  // Add items to your menus here:
  MainMenu.setFirstLine("GEMT Main Menu");
  MainMenu.addItem("nRF Test", runNRFtest);

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
    //Serial.println(CurrentMenuPtr->DEBUG());
    MainMenu.run();
    delay(3);
  }
  
  return 0;
}