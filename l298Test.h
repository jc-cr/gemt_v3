#ifndef motortest
#define motortest

#include <HCSR04.h>

extern void l298Test(void);

GEMTtest l298Display;

// Initialize pins
const unsigned short int  L8IN1 = 5;
const unsigned short int  L8IN2 = 6;
const unsigned short int  L8ENA = 7;
const unsigned short int  L8IN3 = 8;
const unsigned short int  L8IN4 = 9;
const unsigned short int  L8ENB = 10;

// I have no idea what this does
// voltL and volt R are supposed to refer to line 69
digitalWrite(L8IN1, LOW);
digitalWrite(L8IN2, LOW);
digitalWrite(L8IN3, LOW);
digitalWrite(L8IN4, LOW);

analogWrite(L8ENA, 255);
digitalWrite(L8IN1, HIGH);
digitalWrite(L8IN2, LOW);
delay(2000);
voltL();
digitalWrite(L8IN1, LOW);
digitalWrite(L8IN2, HIGH);
delay(2000);
voltL();
digitalWrite(L8IN1, LOW);
digitalWrite(L8IN2, LOW);
  
analogWrite(L8ENB, 255);
digitalWrite(L8IN3, HIGH);
digitalWrite(L8IN4, LOW);
delay(2000);
voltR();
digitalWrite(L8IN3, LOW);
digitalWrite(L8IN4, HIGH);
delay(2000);
voltR();

digitalWrite(L8IN3, LOW);
digitalWrite(L8IN4, LOW);
delay(200);
// End to "I have no idea what this does"

void runl298Test(void)
{
  l298Display.setFirstLine("L298 Info:");
  l298Display.setInfoMsgLine("Bottom HBridge -> Bottom PCB Row"); 
  l298Display.setInfoMsgLine("out1-4 -> Left PCB Rw"); 
  l298Display.setInfoMsgLine("Right -> Analog");

  if(l298Display.showInfoScreen())
  {
    l298Display.setFirstLine("L298 Test:");
    l298Display.showStaticTestScreen(l298Test);
  }
}


// NOTE: Since test is constatly updating, we dont have access to click feature
//        If testing interval too short we cant exit test!
extern void l298Test(void)
{
  if (millis() - previousMillis > testingInterval)
   {
    previousMillis = millis();

    int value_in1 = analogRead(A1); // Voltage readings from left output. Front & backward = in1 & in2
    float voltage_in1 = value_in1 * 5.0/1023;
    int value_in2 = analogRead(A2);
    float voltage_in2 = value_in2 * 5.0/1023;

    int value_in3 = analogRead(A3); // Voltage readings from right output. Front & backward = in3 & in4
    float voltage_in3 = value_in3 * 5.0/1023;
    int value_in4 = analogRead(A4);
    float voltage_in4 = value_in4 * 5.0/1023;

    String top = String("V-Left: ") + String(voltage_in1) + String(" & ") + String(voltage_in2); // 19 char here
    String bottom = String("V-Right: ") + String(voltage_in3) + String(" & ") + String(voltage_in4); // 20 char here
    String msg = top + bottom;

    l298Display.showStaticTestFeedback(msg)
   }
}

#endif