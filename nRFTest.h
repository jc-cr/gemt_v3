#pragma once


#include "gemt_interface.h"
#include <RF24.h>

GEMTtest nRFdisplay;

extern void nRFtest(void);

// Driver for testing protocol
void runNRFtest(void)
{
  nRFdisplay.setFirstLine("nRF Test Info: ");
  nRFdisplay.setInfoMsgLine("nRF to test -> SPI1");
  nRFdisplay.setInfoMsgLine("Working nRF -> SPI2");

  // If user chooses to proceed, run test
  // Otherwise we just exit test and return to previous menu
  if (nRFdisplay.showInfoScreen())
  {
    nRFdisplay.setFirstLine("nRF Test: ");
    nRFdisplay.showStaticTestScreen(nRFtest);
  }
}

// This is actual testing function
extern void nRFtest(void)
{
  bool result = false;
  
  //create an RF24 object
  // Testing done on SPI2 port input
  RF24 spi1(42, 43);  // CE, CSN
  RF24 spi2(24, 22);  // CE, CSN

  spi1.begin();
  spi2.begin();
  
  //address through which two modules communicate.
  const byte address[6] = "00002";
  
 
  nRFdisplay.setStaticTestFeedbackLine("SPI1 RX");
  nRFdisplay.setStaticTestFeedbackLine("Test starting!");
  nRFdisplay.showStaticTestFeedback(1000);

  // Test connections, display feedback
  if (spi2.isChipConnected() == 1 && spi1.isChipConnected() == 1) 
  {
    // TEST 1: SPI2 TX
    //----------------------------------------------
    //set the address
    spi1.openReadingPipe(0, address);

    // RX Radio config:
    spi1.flush_tx(); // Clear 3 FIFO buffers
    spi1.setPALevel(0); // Set power amplifier low
    spi1.startListening(); //Set module as receiver
    
    // TX Radio config:
    spi2.setPALevel(0); // Set power amplifier min
    spi2.openWritingPipe(address); //set the address
    spi2.stopListening(); //Set module as transmitter

    // Transmission test. Loop few times because radios are weird
    const char text[] = "Test message";
    char msgBuf[32] = {0};
    for (int i = 0; i < 5; ++i)    
    {
      spi1.read(&msgBuf, sizeof(msgBuf));
      
      delay(10);
      
      if (spi2.write(&text, sizeof(text))) 
      { 
        // We got acknoledgement of message, test done
        result = 1;
        break;
      }
      else
      {
         result = 0;
      }
      
      delay(100);
    }

    // If first test passed, continue to next test
    if (result == 1)
    {
      nRFdisplay.setStaticTestFeedbackLine("SPI1 TX Passed... ");
      nRFdisplay.setStaticTestFeedbackLine("SPI1 TX");
      nRFdisplay.setStaticTestFeedbackLine("Test starting!");
      nRFdisplay.showStaticTestFeedback(0);
      delay(1500);
      
      
      // TEST 2: SPI2 RX
      //----------------------------------------------
    
      // RX radio config:
      spi2.openReadingPipe(0, address); //set the address   
      spi2.flush_tx();  // Clear 3 FIFO buffers  
      spi2.setPALevel(0);  // Set power amplifier low
      spi2.startListening();  //Set module as receiver
    
      // TX Radio config:
      spi1.setPALevel(0);     // Set power amplifier min 
      spi1.openWritingPipe(address); //set the address
      spi1.stopListening(); //Set module as transmitter
  
      // Transmission test. Loop few times because radios are weird
      const char text[] = "Test message";
      char msgBuf[32] = {0};
      for (int i = 0; i < 5; ++i)    
      {
        spi2.read(&msgBuf, sizeof(msgBuf));
        
        delay(10);
        
        if (spi1.write(&text, sizeof(text))) 
        { 
          // We got acknoledgement of message, test done
          result = 1;
          break;
        }
        else
        {
           result = 0;
        }
        
        delay(100);
      }
    }
  }
  
  else if (spi2.isChipConnected() == 1 && spi1.isChipConnected() == 0) 
  {
    nRFdisplay.setStaticTestFeedbackLine("SPI1");
    nRFdisplay.setStaticTestFeedbackLine("not detected!");
    nRFdisplay.showStaticTestFeedback(0);
    delay(1500);
  }
  
  else if ( spi2.isChipConnected() == 0 && spi1.isChipConnected() == 1)
  {
    nRFdisplay.setStaticTestFeedbackLine("SPI2");
    nRFdisplay.setStaticTestFeedbackLine("not detected!");
    nRFdisplay.showStaticTestFeedback(0);
    delay(1500);
    result = 0;
  }
  
  else 
  {
    nRFdisplay.setStaticTestFeedbackLine("SPI1, SPI2");
    nRFdisplay.setStaticTestFeedbackLine("not detected!");
    nRFdisplay.showStaticTestFeedback(0);
    delay(1500);
    result = 0;
  }


  // Return final results
  if (result == true)
  {
    nRFdisplay.setStaticTestFeedbackLine("NRF test passed");
    nRFdisplay.showStaticTestFeedback(1);
    delay(1500);
  }
  else
  {
    nRFdisplay.setStaticTestFeedbackLine("NRF test failed");
    nRFdisplay.showStaticTestFeedback(1);
    delay(1500);
  }

  nRFdisplay.testingComplete = true;
} 