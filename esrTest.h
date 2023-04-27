#pragma once
#include "Arduino.h"
#include "gemt_interface.h"

GEMTtest esrDisplay;

#define FASTADC 1 
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//define the input and output pin we will use for ESR
#define DISCHARGE_PIN 41
#define ESR_PIN A0
#define PULSE_PIN 39

unsigned long measureESR();
double ESR_test(void);
long readVcc();
void esrTest(void);


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
    esrDisplay.setFirstLine("ESR Test: ");
    //esrTest();
    esrDisplay.showStaticTestScreen(esrTest);
  }
}

void esrTest(void)
{
  esrDisplay.setStaticTestFeedbackLine("Test starting.");
  esrDisplay.setStaticTestFeedbackLine("Sit back, relax,");
  esrDisplay.setStaticTestFeedbackLine("This'll take a while.");
  esrDisplay.showStaticTestFeedback(1000);

  pinMode(ESR_PIN, INPUT);//reading miliVolt
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(DISCHARGE_PIN, OUTPUT);

  
  double esrVal = 0.00;
  esrVal = ESR_test();

  Serial.println(esrVal, 2);
  
  String msg = String("ESR: ") + String(esrVal,2) + String(" mOhms");

  esrDisplay.setStaticTestFeedbackLine(msg);
  esrDisplay.showStaticTestFeedback(6000);

  esrDisplay.testingComplete = true;
}


double ESR_test(void)
{
  unsigned long esrSamples = 0;
  double miliVolt = 0.00;
  double esrVal = 0.00;
  double Vcc = 0.00;
  double totalAvg = 0.00;
  double vRef = 1.1; //voltage on the Aref pin
  unsigned short int count = 0;
  unsigned short int numReadings = 2; // Change this value to increase/decrease the number of readings

  Serial.println("ESR Meter");
  Serial.println("Setting up");

  Vcc = readVcc(); //sets Vcc to well defined and measured arduino power rail voltage
  analogReference(INTERNAL1V1); // Setting vRef to internal reference 1.1V
  digitalWrite(PULSE_PIN, HIGH); //low enables T1

  digitalWrite(PULSE_PIN, HIGH); //low disables T2
  Serial.println("Please wait...\n");

  if (FASTADC)
  {
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
  }

  while (count < numReadings) // Increased number of readings to 10
  {
    esrSamples = measureESR(); // This function takes a while
    miliVolt = (esrSamples * vRef) / 0.09; //0.45 // 0.1 //calculating voltage on AIN0 pin
    Serial.print("miliVolt: ");
    Serial.println(miliVolt, 2); // Debug: print miliVolt value
    esrVal = 100 / ((Vcc / miliVolt) - 1); // ESR value in ohms
    Serial.print("esrVal: ");
    Serial.println(esrVal, 2); // Debug: print esrVal value
    esrVal = esrVal * 1000.00; // ESR value in mOhm1
    totalAvg += esrVal;
    count++;
    //delay(10); // Add delay between measurements
  }
  totalAvg /= numReadings;
  return totalAvg; // Accounting for internal resistance
}


unsigned long measureESR()
{
  unsigned long samples = 0;
  unsigned int acumulator = 0;
  int i = 0;

  while (i < 8192) // Number of samples: 8192
  {
    digitalWrite(DISCHARGE_PIN, HIGH); //discharge caps
    delayMicroseconds(2000); // Discharge time: 2 ms
    digitalWrite(DISCHARGE_PIN, LOW);  //disable discharging
    
    unsigned int pulseDuration = 5; // Initial pulse duration: 5 microseconds
    if (samples > 0)
    {
      double avg_miliVolt = ((samples >> 5) * 1.1) / 0.2;
      if (avg_miliVolt >= 15.0)
      {
        pulseDuration = 10; // Increase pulse duration for higher resistances
      }
    }
    digitalWrite(PULSE_PIN, LOW); //making a miliVolt pulse of 100mA (Changed from 50mA)
    delayMicroseconds(pulseDuration); // Pulse duration based on average measured voltage
    delay(1); // Add a small delay before reading the ADC value
    
    acumulator = analogRead(ESR_PIN); //reading value on AIN0
    digitalWrite(PULSE_PIN, HIGH); //stopping pulse
    delayMicroseconds(500); // Add a small delay before the next sample
    delayMicroseconds(pulseDuration);
    samples += acumulator; //accumulating the readings
    i++;
    delayMicroseconds(10);
  }
  samples = samples >> 5; // Adjust right shift value due to sample count change
   // Debug information
  Serial.print("Raw samples: ");
   Serial.println(samples);
  return samples;
}

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference

  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
     ADMUX = _BV(MUX5) | _BV(MUX0) ;
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(1); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}