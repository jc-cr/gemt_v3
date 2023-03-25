#include "gemt_interface.h"
#include "nRFTest.h"

GEMTmenu MainMenu(1);

void setup(void)
{
  delay(500);

  MainMenu.addItem("nRF Test", nRFtest);




  MainMenu.bootUp();
  //Serial.begin(115200); // DEBUGGING
}

int main(void) 
{
  // Must intialize the arduino firmware
  init();
  setup();

  while(true)
  {
    //MainMenu.run();
    delay(3);
  }
  
  return 0;
}