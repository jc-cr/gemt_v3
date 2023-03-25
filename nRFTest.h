#include "gemt_interface.h"

String msg = "Connect functional nRF24 module into SPI1 port\
        \nConnect nRF24 module to be tested into SPI2";

GEMTtestScreen nRFdisplay(msg);

void nRFtest(void)
{
  // User chose not to proceed
  if(!nRFdisplay.showInfoScreen()){break};

  // User chose to proceed with test
  else
  {
    nRFdisplay.staticHeader("");
    nRFdisplay.
    
  }
};

