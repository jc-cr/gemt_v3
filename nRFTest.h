#include "gemt_interface.h"

String msg = "Connect functional nRF24 module into SPI1 port\
        \nConnect nRF24 module to be tested into SPI2";
String title = "nRF Test";

GEMTtest nRFdisplay(msg);
void nRFtest(void);

void runNRFtest(void)
{
  // User chose to proceed with test: setup screen and run test 
  nRFdisplay.showInfoScreen();

  if(nRFdisplay.userSelectedOk)
  {
    // Setup display
    nRFdisplay.setFirstLine(title);
    // Static bottom ("Done") set in class implementation
    nRFtest();
  }

}

// use nrf.testResults to update available lines with feedback
void nRFtest(void)
{

} 
