#pragma once


#include "gemt_interface.h"

extern void l298Test(void);


GEMTtest l298Display;

// Initialize pins
#define in1 34
#define in2 36
#define enA 46
#define in3 38
#define in4 40
#define enB 44


int value_in1 = 0;
float voltage_in1;
int value_in2 = 0;
float voltage_in2;
int value_in3 = 0;
float voltage_in3;
int value_in4 = 0;
float voltage_in4;
String l1;
String l2;

void test()
{
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void voltL()
{

  value_in1 = analogRead(A3);
  voltage_in1 = value_in1 * 5.0/1023;
  Serial.print("Voltage OUT1= ");
  Serial.println(voltage_in1);
  value_in2 = analogRead(A4);
  voltage_in2 = value_in2 * 5.0/1023;
  Serial.print("Voltage OUT2= ");
  Serial.println(voltage_in2);

}

void runl298Test(void)
{
  l298Display.setFirstLine("L298 Info:");
  l298Display.setInfoMsgLine("Bottom HBridge ->");
  l298Display.setInfoMsgLine("Bottom PCB Row"); 
  l298Display.setInfoMsgLine("out1-4 ->"); 
  l298Display.setInfoMsgLine("Left PCB Rw"); 
  l298Display.setInfoMsgLine("Right -> Analog");

  if(l298Display.showInfoScreen())
  {
    l298Display.setFirstLine("L298 Test:");
    l298Display.showStaticTestScreen(l298Test);
  }
}

// NOTE: Since test is constatly updating, we dont have access to click feature
//        If testing interval too short we cant exit test!
unsigned long l298nTestingInterval = 1000;
unsigned long l298nPreviousMillis = 0.00;

extern void l298Test(void)
{
  int x=1;
  
  while (x<4)
   {
    switch (x) 
    {
  case 1:
    
    analogWrite(enA, 255);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    value_in1 = analogRead(A3); // Voltage readings from left output. Front & backward = in1 & in2
    voltage_in1 = value_in1 * 5.0/1023;
    value_in2 = analogRead(A4);
    voltage_in2 = value_in2 * 5.0/1023;

    l1 = String("V-Left1: ") + String(voltage_in1, 2);
    l2 = String("V-Left2: ") + String(voltage_in2, 2);
    
    l298Display.setStaticTestFeedbackLine("Forward");
    l298Display.setStaticTestFeedbackLine(l1);
    l298Display.setStaticTestFeedbackLine(l2);
    l298Display.showStaticTestFeedback(2000);

    
    
    
 
    x=x+1;
    break;
  case 2:
    
    analogWrite(enA, 255);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    value_in1 = analogRead(A3); // Voltage readings from left output. Front & backward = in1 & in2
    voltage_in1 = value_in1 * 5.0/1023;
    value_in2 = analogRead(A4);
    voltage_in2 = value_in2 * 5.0/1023;

    l1 = String("V-Left1: ") + String(voltage_in1, 2);
    l2 = String("V-Left2: ") + String(voltage_in2, 2);
    
    l298Display.setStaticTestFeedbackLine("Backwards");
    l298Display.setStaticTestFeedbackLine(l1);
    l298Display.setStaticTestFeedbackLine(l2);
    l298Display.showStaticTestFeedback(2000);
    x=x+1;
    break;
   case 3:
    
    analogWrite(enA, 255);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    value_in1 = analogRead(A3); // Voltage readings from left output. Front & backward = in1 & in2
    voltage_in1 = value_in1 * 5.0/1023;
    value_in2 = analogRead(A4);
    voltage_in2 = value_in2 * 5.0/1023;

    l1 = String("V-Left1: ") + String(voltage_in1, 2);
    l2 = String("V-Left2: ") + String(voltage_in2, 2);
    
    l298Display.setStaticTestFeedbackLine("OFF");
    l298Display.setStaticTestFeedbackLine(l1);
    l298Display.setStaticTestFeedbackLine(l2);
    l298Display.showStaticTestFeedback(2000);

    x=x+1;
    break;
   case 4:
    l298Display.setStaticTestFeedbackLine("Null");
    l298Display.showStaticTestFeedback(2000);
    x=5;
    break;
    }
   }
}
