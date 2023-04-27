#include "gemt_interface.h"
#include "nRFTest.h"
#include "9gServoTest.h"
#include "ultrasonicTest.h"
#include "esrTest.h"
#include "l298Test.h"

GEMTmenu MainMenu(5);
GEMTmenu ServoMenu(3);

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
  MainMenu.setFirstLine("GEMT Main Menu: ");
  MainMenu.addItem("ESR Test", runESRtest);
  MainMenu.addItem("9G Servo Test", goToServoMenu);
  MainMenu.addItem("nRF Test", runNRFtest);
  MainMenu.addItem("L298N Test", runl298Test);
  MainMenu.addItem("Ultrasonic Test", runUltrasonicTest);
  
  ServoMenu.setFirstLine("9G Servo Submenu: ");
  ServoMenu.addItem("Manual Servo Test", runServoManualTest);
  ServoMenu.addItem("Auto Servo Test", runServoAutoTest);
  ServoMenu.addItem("Back", goToMainMenu);

  MainMenu.bootUp();
}

int main(void) 
{
  init(); // Must intialize the arduino firmware
  setup();

  // This the loop arduino fucntion
  while(true)
  {
    startGEMT(MainMenu);
    delay(3);
  }

  return 0;
}