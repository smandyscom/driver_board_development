
#define ARDUINO_PROJECT 0

#include <eeprom.h>
//#include <timerone.h>
//#include "convolution.h"
//#include "cwccwcontrol.h"
//#include "timer.h"
//#include "actuator.h"
#include"TimerOne-r11\TimerOne.h"
#include<dcBrushControl.h>


#define TIMER_INTERVAL	10000		// unit in micro second
#define UART_BAUT_RATE 9600

#define AXIS_0	0
#define TOTAL_AXIS 1

extern int k29[];

DCBRUSHCONFIGURATION __config = {
	MODE_PWM_DIRECTION,
	0,
	0,
	currentSense,
	k29,
	29,
	/*{128,128,128},
	{128,128},*/
};
dcBrushControl __motor(__config);
void setup() 
{
	

  Serial.begin(UART_BAUT_RATE);
  
  Timer1.initialize(TIMER_INTERVAL); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}
 
void loop()
{
  // Main code loop
  // TODO: Put your regular (non-ISR) logic here
	//while (Serial.available()>0)
	//{
	//	char incoming = Serial.read();
	//	switch (incoming){
	//	case '1':
	//		inputValue[0] = 255;
	//		break;
	//	case '2':
	//		inputValue[0] = 0;
	//		break;
	//	case '3':
	//		inputValue[0] = -255;
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//----------------------------------
	//	Multi-task sequence control
	//----------------------------------
	//Serial.println(analogRead(A0));
	
	//if (digitalRead(2) == LOW){
	//	inputValue[0] = 255;
	//	digitalWrite(13, HIGH);
	//}
	//else{

	//	if (digitalRead(4) == LOW){
	//		inputValue[0] = -255;
	//		digitalWrite(13, HIGH);
	//	}
	//	else{
	//		inputValue[0] = 0;
	//		digitalWrite(13, LOW);
	//	}
	//}
	//Serial.println(currentSense());

	//if (digitalRead(2) == LOW){
	//	actuatorControl[AXIS_0].Command = REQUEST_TO_HOME;
	//	//Serial.println(analogRead(A0));
	//	Serial.println(actuatorCurrentDegree(&actuatorControl[AXIS_0]));
	//}
	//else{

	//	if (digitalRead(4) == LOW){
	//		actuatorControl[AXIS_0].Command = REQUEST_TO_LIMIT;
	//		//Serial.println(analogRead(A0));
	//		Serial.println(actuatorCurrentDegree(&actuatorControl[AXIS_0]));
	//	}
	//	else{
	//		actuatorControl[AXIS_0].Command = REQUEST_STOP;
	//	}
	//}



}
 
/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
	//---------------------------------
	//	MotorInterpolator-Outputcontrol
	//---------------------------------
	//for (int i = 0; i < TOTAL_AXIS; i++)
	//	motors[i].drive(interpolators[i].convolutionCalculate(inputValue[i]));
	//deviceRunning((deviceBase*)&actuatorControl[AXIS_0]);
	//inputValue[0] = (float)__inputValue;		//
	//----------------------------------
	//	Timer
	//----------------------------------

    // Toggle LED
    //Serial.println("abc");
    //digitalWrite( 13, digitalRead( 13 ) ^ 1 );

	//count++;
	//if (count == 100){
	//	//Serial.println(value,2);
	//	digitalWrite(13, !digitalRead(13));
	//	count = 0;
	//}
	__motor.drive();
}

int currentSense()
{
	//linked to filtered sense value
	return analogRead(A0);
}


