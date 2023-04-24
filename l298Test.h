#pragma once


#include "gemt_interface.h"

extern void l298Test(void);
void VoltL(void);
void VoltR();

GEMTtest l298Display;

// Initialize pins
const unsigned short int  L8IN1 = 34;
const unsigned short int  L8IN2 = 36;
const unsigned short int  L8ENA = 46;
const unsigned short int  L8IN3 = 38;
const unsigned short int  L8IN4 = 40;
const unsigned short int  L8ENB = 44;

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
  int x = 1;
  int value_in1; // Voltage readings from left output. Front & backward = in1 & in2
  float voltage_in1;
  int value_in2;
  float voltage_in2;
  String l1;
  String l2;
  digitalWrite(L8IN1, LOW);
  digitalWrite(L8IN2, LOW);
  digitalWrite(L8IN3, LOW);
  digitalWrite(L8IN4, LOW);
  
  if (x<4)
   {
    switch (x) 
    {
  case 1:
    analogWrite(L8ENA, 255);
    digitalWrite(L8IN1, HIGH);
    digitalWrite(L8IN2, LOW);
    value_in1 = analogRead(A1); // Voltage readings from left output. Front & backward = in1 & in2
    voltage_in1 = value_in1 * 5.0/1023;
    value_in2 = analogRead(A2);
    voltage_in2 = value_in2 * 5.0/1023;

    l1 = String("V-Left1: ") + String(voltage_in1, 2);
    l2 = String("V-Left2: ") + String(voltage_in2, 2);
    
    l298Display.setStaticTestFeedbackLine(l1);
    l298Display.setStaticTestFeedbackLine(l2);
    l298Display.showStaticTestFeedback(2000);

    x=x+1;
    break;
  case 2:
    analogWrite(L8ENA, 255);
    digitalWrite(L8IN1, HIGH);
    digitalWrite(L8IN2, LOW);
    value_in1 = analogRead(A1); // Voltage readings from left output. Front & backward = in1 & in2
    voltage_in1 = value_in1 * 5.0/1023;
    value_in2 = analogRead(A2);
    voltage_in2 = value_in2 * 5.0/1023;

    l1 = String("V-Left1: ") + String(voltage_in1, 2);
    l2 = String("V-Left2: ") + String(voltage_in2, 2);
    
    l298Display.setStaticTestFeedbackLine(l1);
    l298Display.setStaticTestFeedbackLine(l2);
    l298Display.showStaticTestFeedback(2000);

    x=x+1;
    break;
   case 3:
    analogWrite(L8ENA, 255);
    digitalWrite(L8IN1, LOW);
    digitalWrite(L8IN2, HIGH);
    value_in1 = analogRead(A1); // Voltage readings from left output. Front & backward = in1 & in2
    voltage_in1 = value_in1 * 5.0/1023;
    value_in2 = analogRead(A2);
    voltage_in2 = value_in2 * 5.0/1023;
    l1 = String("V-Left1: ") + String(voltage_in1, 2);
    l2 = String("V-Left2: ") + String(voltage_in2, 2);
    
    l298Display.setStaticTestFeedbackLine(l1);
    l298Display.setStaticTestFeedbackLine(l2);
    l298Display.showStaticTestFeedback(2000);

    x=x+1;
    break;
   case 4:
    analogWrite(L8ENA, 255);
    digitalWrite(L8IN1, LOW);
    digitalWrite(L8IN2, HIGH);
    value_in1 = analogRead(A1); // Voltage readings from left output. Front & backward = in1 & in2
    voltage_in1 = value_in1 * 5.0/1023;
    value_in2 = analogRead(A2);
    voltage_in2 = value_in2 * 5.0/1023;
    l1 = String("V-Left1: ") + String(voltage_in1, 2);
    l2 = String("V-Left2: ") + String(voltage_in2, 2);
    
    l298Display.setStaticTestFeedbackLine(l1);
    l298Display.setStaticTestFeedbackLine(l2);
    l298Display.showStaticTestFeedback(2000);

    x=5;
    break;
    }
    

   }
}

void VoltL()
{
  analogWrite(L8ENA, 255);
  digitalWrite(L8IN1, HIGH);
  digitalWrite(L8IN2, LOW);
  delay(500);
  VoltL();
  digitalWrite(L8IN1, LOW);
  digitalWrite(L8IN2, HIGH);
  delay(500);
  VoltL();
  digitalWrite(L8IN1, LOW);
  digitalWrite(L8IN2, LOW);
}

void VoltR()
{
  analogWrite(L8ENB, 255);
  digitalWrite(L8IN3, HIGH);
  digitalWrite(L8IN4, LOW);
  delay(500);
  VoltR();
  digitalWrite(L8IN3, LOW);
  digitalWrite(L8IN4, HIGH);
  delay(500);
  VoltR();
  digitalWrite(L8IN1, LOW);
  digitalWrite(L8IN2, LOW);
}
