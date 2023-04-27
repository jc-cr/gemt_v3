#pragma once
#include "Arduino.h"
#include "gemt_interface.h"

GEMTtest esrDisplay;

#define FASTADC 1 
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define DISCHARGE_PIN 41
#define ESR_PIN A0
#define PULSE_PIN 39


double ESR_test(void);
unsigned long measureESR();
long readVcc();
void esrTest(void);


/**
 * Main driver for ESR testing protocol.
 */
void runESRtest(void)
{
  esrDisplay.setFirstLine("ESR Test Info: ");
  esrDisplay.setInfoMsgLine("Cathode -> J1");
  esrDisplay.setInfoMsgLine("Anode -> J2");

  if (esrDisplay.showInfoScreen())
  {
    esrDisplay.setFirstLine("ESR Test: ");
    esrDisplay.showStaticTestScreen(esrTest);
  }
}

/**
 * Perform ESR test and display the result on the screen.
 */
void esrTest(void)
{
  esrDisplay.setStaticTestFeedbackLine("Test starting.");
  esrDisplay.setStaticTestFeedbackLine("Sit back, relax,");
  esrDisplay.setStaticTestFeedbackLine("This'll take ~1 min");
  esrDisplay.showStaticTestFeedback(1000);

  pinMode(ESR_PIN, INPUT);
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(DISCHARGE_PIN, OUTPUT);

  double esrVal = 0.00;
  esrVal = ESR_test();
  
  String msg = String("ESR: ") + String(esrVal,2) + String(" mOhms");
  esrDisplay.setStaticTestFeedbackLine(msg);
  esrDisplay.showStaticTestFeedback(10000);

  esrDisplay.testingComplete = true;
}

/**
 * Calculate ESR value by performing multiple measurements.
 * @return ESR value in milliohms.
 */
double ESR_test(void)
{
  unsigned long esrSamples = 0;
  double miliVolt = 0.00;
  double esrVal = 0.00;
  double Vcc = 0.00;
  double totalAvg = 0.00;
  double vRef = 1.1;
  unsigned short int count = 0;
  unsigned short int numReadings = 2;

  Vcc = readVcc();
  analogReference(INTERNAL1V1);
  digitalWrite(PULSE_PIN, HIGH);

  if (FASTADC)
  {
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
  }

  while (count < numReadings)
  {
    esrSamples = measureESR();
    miliVolt = (esrSamples * vRef) / 1024.00; // 0.09 - With Serial
      // Note: Serial Printing seem to cause timing issues, 
    esrVal = 100 / ((Vcc / miliVolt) - 1);
    esrVal = esrVal * 1000.00;
    totalAvg += esrVal;
    count++;
  }
  totalAvg /= numReadings;
  return totalAvg;
}

/**
 * Measure ESR samples.
 * @return Adjusted ESR samples.
 */
unsigned long measureESR()
{
  unsigned long samples = 0;
  unsigned int acumulator = 0;
  int i = 0;

  while (i < 8192)
  {
    digitalWrite(DISCHARGE_PIN, HIGH);
    delayMicroseconds(2000);
    digitalWrite(DISCHARGE_PIN, LOW);

    unsigned int pulseDuration = 5;
    if (samples > 0)
    {
      double avg_miliVolt = ((samples >> 5) * 1.1) / 0.2;
      if (avg_miliVolt >= 15.0)
      {
        pulseDuration = 10;
      }
    }
    digitalWrite(PULSE_PIN, LOW);
    delayMicroseconds(pulseDuration);
    delay(1);

    acumulator = analogRead(ESR_PIN);
    digitalWrite(PULSE_PIN, HIGH);
    delayMicroseconds(500);
    delayMicroseconds(pulseDuration);
    samples += acumulator;
    i++;
    delayMicroseconds(10);
  }
  samples = samples >> 5;
  return samples;
}

/**
 * Read Vcc value.
 * @return Vcc value in millivolts.
 */
long readVcc() {
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
     ADMUX = _BV(MUX5) | _BV(MUX0) ;
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(1);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC));

  uint8_t low  = ADCL;
  uint8_t high = ADCH;

  long result = (high<<8) | low;
  result = 1125300L / result;
  return result;
}