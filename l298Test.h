#ifndef motortest
#define motortest

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
/*
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
*/
// End to "I have no idea what this does"

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
unsigned long l298nTestingInterval = 3000;
unsigned long l298nPreviousMillis = 0.00;

extern void l298Test(void)
{
  if (millis() - l298nPreviousMillis > l298nTestingInterval)
   {
    l298nPreviousMillis = millis();

    int value_in1 = analogRead(A1); // Voltage readings from left output. Front & backward = in1 & in2
    float voltage_in1 = value_in1 * 5.0/1023;
    int value_in2 = analogRead(A2);
    float voltage_in2 = value_in2 * 5.0/1023;

    int value_in3 = analogRead(A3); // Voltage readings from right output. Front & backward = in3 & in4
    float voltage_in3 = value_in3 * 5.0/1023;
    int value_in4 = analogRead(A4);
    float voltage_in4 = value_in4 * 5.0/1023;

    String l1 = String("V-Left1: ") + String(voltage_in1, 2);
    String l2 = String("V-Left2: ") + String(voltage_in2, 2);
    
    String r1 = String("V-Right1: ") + String(voltage_in3, 2); // 20 char here
    String r2 = String("V-Right1: ") + String(voltage_in4, 2); // 20 char here

    l298Display.setStaticTestFeedbackLine(l1);
    l298Display.setStaticTestFeedbackLine(l2);
    l298Display.setStaticTestFeedbackLine(r1);
    l298Display.setStaticTestFeedbackLine(r2);
    l298Display.showStaticTestFeedback();
   }
}

#endif