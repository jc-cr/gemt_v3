#ifndef esrtest
#define esrtest

//#include "gemt_interface.h"

GEMTtest esrDisplay;

const unsigned short int  DISCHARGE_PIN       = 41;
const unsigned short int  ESR_PIN             = A0;
const unsigned short int  PULSE_PIN           = 39;

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
    esrDisplay.setFirstLine("ESR Test: ");
    esrDisplay.showStaticTestScreen(esrTest);
  }
}

unsigned long esrTestingInterval = 4000.00; // Ping every 4 seconds
unsigned long esrPreviousMillis = 0.00;

extern void esrTest(void)
{
  #define FASTADC 1
  // defines for setting and clearing register bits
  #ifndef cbi
  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
  #endif
  #ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
  #endif
  double        vRef        = 1.069;//voltage on the Aref pin 
  double        current     = 0.088564;
  unsigned long esrSamples;
  double        miliVolt;
  double        esrVal;
  double        Vcc;
  
  pinMode(ESR_PIN, INPUT);//reading miliVolt
  pinMode(PULSE_PIN, OUTPUT);
  pinMode(DISCHARGE_PIN, OUTPUT);
  if (millis() - esrPreviousMillis > esrTestingInterval)
  {
    esrPreviousMillis = millis();
    Vcc = readVcc(); //sets Vcc to well defined and measured arduino power rail voltage
    analogReference(INTERNAL1V1);//setting vRef to internal reference 1.1V
    digitalWrite(PULSE_PIN,HIGH);//low enables T1
    digitalWrite(PULSE_PIN,HIGH);//low disables T2
    if (FASTADC) 
    {
      sbi(ADCSRA,ADPS2);
      cbi(ADCSRA,ADPS1);
      sbi(ADCSRA,ADPS0);
    }
  
    esrSamples = measureESR();//this function takes a while,)
    miliVolt = (esrSamples * vRef) / 65.535;//calculating voltage on AIN0 pin
    esrVal = 100 / ((Vcc/miliVolt)-1); //esr value in ohms
    esrVal = esrVal * 1000; //esrval in mOhms
    
    String msg = String("ESR: ") + String(esrVal,2) + String(" mOhms");
    Serial.println(esrVal);
    esrDisplay.setStaticTestFeedbackLine(msg);
    esrDisplay.showStaticTestFeedback();
  }
}

unsigned long measureESR(void)
{
  unsigned long samples     = 0;
  unsigned int  acumulator  = 0;
  int           i           = 0;
  //oversampling 4096 times (for 16 bit is 4^(desiredResolution - ADCresolution))
  while(i < 4096) {
    digitalWrite(DISCHARGE_PIN,HIGH);//discharge caps
    delayMicroseconds(600);
    digitalWrite(DISCHARGE_PIN,LOW); //disable discharging
    digitalWrite(PULSE_PIN,LOW);//making a miliVolt pulse of 50mA
    delayMicroseconds(5);//on the scope it looks that after enabling the pulse a litle delay is
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
long readVcc(void) {
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

#endif
