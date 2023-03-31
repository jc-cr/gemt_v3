#include "gemt_interface.h"
#include "nRFTest.h"

GEMTmenu MainMenu(5);
GEMTmenu ServoMenu(3);

void dummyTest(void)
{
  Serial.println("Entered test");
}

// TODO: Implement these 2 functions better
void goToServoMenu(void)
{
  updateMenu(ServoMenu);
}
void goToMainMenu(void)
{
  updateMenu(MainMenu);
}


void setup(void)
{
  // Add items to your menus here:
  MainMenu.setFirstLine("GEMT Main Menu:");
  MainMenu.addItem("ESR Test", dummyTest);
  MainMenu.addItem("9G Servo Test", goToServoMenu);
  MainMenu.addItem("nRF Test", dummyTest);
  MainMenu.addItem("L298N Test", dummyTest);
  MainMenu.addItem("Ultrasonic Test", dummyTest);
  
  ServoMenu.setFirstLine("9G Servo Submenu");
  ServoMenu.addItem("Manual Servo Test", dummyTest);
  ServoMenu.addItem("Auto Servo Test", dummyTest);
  ServoMenu.addItem("Back", goToMainMenu);


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