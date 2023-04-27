#pragma once
#include "Arduino.h"

#include "gemt_interface.h"

GEMTtest esrDisplay;

const unsigned short int  DISCHARGE_PIN       = 41;
#define ESR_PIN A0
#define PULSE_PIN 39

bool firstLoop = true;

#define FASTADC 1
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

extern void esrTest(void);
unsigned long measureESR(void);
long readVcc(void);


// Driver for testing protocol
void runESRtest(void)
{
  esrDisplay.setFirstLine("ESR Test Info: ");
  esrDisplay.setInfoMsgLine("Cathode -> Blue");
  esrDisplay.setInfoMsgLine("Anode -> GND");

  // If user chooses to proceed, run test
  // Otherwise we just exit test and return to previous menu
  if (esrDisplay.showInfoScreen())
  {
    firstLoop = true;
    esrDisplay.setFirstLine("ESR Test: ");
    esrDisplay.showStaticTestScreen(esrTest);
  }
}


extern void esrTest(void)
{
     unsigned long esrSamples;
    double        miliVolt = 0.00;
    double        esrVal = 0.00;
    double        Vcc = 0.00;
    double        totalAvg = 0.00;
    double        vRef = 1.069; //voltage on the Aref pin 
    int           count = 0;


  if(firstLoop == true)
  {
    esrDisplay.setStaticTestFeedbackLine("Test starting,");
    esrDisplay.setStaticTestFeedbackLine("Please wait...");
    esrDisplay.showStaticTestFeedback(1500);
    firstLoop = false;
  }
  

  Vcc = readVcc(); //sets Vcc to well defined and measured arduino power rail voltage
  //analogReference(INTERNAL1V1);//setting vRef to internal reference 1.1V
 
  digitalWrite(PULSE_PIN,HIGH);//low enables T1
  digitalWrite(PULSE_PIN,HIGH);//low disables T2

  esrDisplay.setStaticTestFeedbackLine("Test in progress!");
  esrDisplay.showStaticTestFeedback(1000);
  
// Set the ADC prescaler value
// Uncomment one of the following lines to set the desired ADC prescaler value

  //sbi(ADCSRA, ADPS2); cbi(ADCSRA, ADPS1); cbi(ADCSRA, ADPS0); // Prescaler 4
  //sbi(ADCSRA, ADPS2); cbi(ADCSRA, ADPS1); sbi(ADCSRA, ADPS0); // Prescaler 8
  //sbi(ADCSRA, ADPS2); sbi(ADCSRA, ADPS1); cbi(ADCSRA, ADPS0); // Prescaler 16
  //cbi(ADCSRA, ADPS2); sbi(ADCSRA, ADPS1); sbi(ADCSRA, ADPS0); // Prescaler 32
  //sbi(ADCSRA, ADPS2); sbi(ADCSRA, ADPS1); sbi(ADCSRA, ADPS0); // Prescaler 64
  cbi(ADCSRA, ADPS2); cbi(ADCSRA, ADPS1); sbi(ADCSRA, ADPS0); // Prescaler 128


 esrSamples = measureESR();//this function takes a while,)
    miliVolt = (esrSamples * Vcc) / 1023.0; //calculating voltage on AIN0 pin using AVcc as reference

    double currentSource = 50.0; // Current source value in mA
    esrVal = (miliVolt * 1000) / currentSource; //esr value in mOhms

    String msg = String("ESR: ") + String(esrVal, 2) + String(" mOhms");
    esrDisplay.setStaticTestFeedbackLine(msg);
    esrDisplay.showStaticTestFeedback(5000);
    esrDisplay.testingComplete = true;
}


unsigned long measureESR(void)
{
    unsigned long samples = 0;
    unsigned int acumulator = 0;
    int i = 0;
    //oversampling 4096 times (for 16 bit is 4^(desiredResolution - ADCresolution))
    while(i < 4096) {
        digitalWrite(DISCHARGE_PIN,HIGH);//discharge caps
        digitalWrite(DISCHARGE_PIN,LOW); //disable discharging
        delayMicroseconds(1000); // Add a delay to allow residual voltage to dissipate
        digitalWrite(PULSE_PIN,LOW);//making a miliVolt pulse of 50mA
        delayMicroseconds(100); // Try different values between 50 and 100
        acumulator = analogRead(ESR_PIN);
        delayMicroseconds(200); // Increase delay to allow the signal to stabilize

        //recomended so the oscillations fade away
        acumulator = analogRead(ESR_PIN);//reading value on AIN0
        digitalWrite(PULSE_PIN,HIGH);//stopping pulse
        samples += acumulator;//acumulating the readings
        i++;
    }
    samples = samples >> 6;//decimating value
    return samples;
}


//function designed to find the true Vcc power rail voltage, used for ESR calculations
long readVcc(void) 
{
    // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
     ADMUX = _BV(MUX5) | _BV(MUX0) ;
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

