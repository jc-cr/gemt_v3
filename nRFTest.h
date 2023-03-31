#ifndef nrftest
#define nrftest

#include "gemt_interface.h"
#include <RF24.h>

GEMTtest nRFdisplay;

extern void nRFtest(void);

extern void debug(void){Serial.println("Ran test");};

void runNRFtest(void)
{
  nRFdisplay.setInfoTitle("nRF Connections");
  nRFdisplay.setInfoMsgLine("nRF to test in SPI1 port");
  nRFdisplay.setInfoMsgLine("Working nRF in SPI2 port");

  // If user chooses to proceed, run test
  // Otherwise we just exit test and return to previous menu
  if (nRFdisplay.showInfoScreen())
  {
    nRFdisplay.setFirstLine("nRF Test");
    nRFdisplay.showStaticTestScreen(debug);
  }
  
}

// This is actual testing function
//  The way this should work is that function just updates,
//  GEMTtest.testFeedbakc,  message with condition/result of test
extern void nRFtest(void)
{
  bool result;
  
  //create an RF24 object
  // Testing done on SPI2 port input
  RF24 spi1(22, 23);  // CE, CSN
  RF24 spi2(24, 25);  // CE, CSN

  spi1.begin();
  spi2.begin();
  
  //address through which two modules communicate.
  const byte address[6] = "00002";
  
 
  nRFdisplay.testFeedback("nRF24 Test started...");

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
      nRFdisplay.testFeedback("SPI2 TX Passed... ");
      nRFdisplay.testFeedback("Starting RX test!");
      
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
    nRFdisplay.testFeedback("SPI1 not detected!");
    result = 0;
  }
  
  else if ( spi2.isChipConnected() == 0 && spi1.isChipConnected() == 1)
  {
    nRFdisplay.testFeedback("SPI2 not detected!");
    result = 0;
  }
  
  else 
  {
    nRFdisplay.testFeedback("SPI1, SPI2 not detected!");
    result = 0;
  }


  // Return final results
  if (result == 1)
  {
    nRFdisplay.testFeedback("NRF TEST PASSED");
  }
  else
  {
    nRFdisplay.testFeedback("NRF TEST FAILED");
  }
} 

#endif
