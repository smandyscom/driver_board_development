#include <EEPROM.h>
#include <TimerOne.h>
#define ARDUINO_PROJECT 0
#include "convolution.h"
#include "cwccwControl.h"
#include "timer.h"
#include "actuator.h"


#define TIMER_INTERVAL	10000		// unit in micro second
#define UART_BAUT_RATE 9600

#define AXIS_0	0
#define TOTAL_AXIS 1

extern float filter[];

convolution interpolators[TOTAL_AXIS];
cwccwControl motors[TOTAL_AXIS];
timerDevice __t;
actuator actuatorControl[TOTAL_AXIS];
BUFFER_TYPE inputValue[TOTAL_AXIS];

int __inputValue;

int count = 0;
float value = 0;

bool normalClose(){ return true; }

void setup() 
{
  // Initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards
	interpolators[AXIS_0].xt = filter;
	interpolators[AXIS_0].length = WINDOW_LENGTH;
	timerDeviceInitialize(&__t);
	//interpolators[AXIS_1].xt = filter;
	//interpolators[AXIS_1].length = WINDOW_LENGTH;
	actuatorInitialize(&actuatorControl[AXIS_0]);
	actuatorSetDirectionAndSpeed(&actuatorControl[AXIS_0], 1,255);
	(actuatorControl[AXIS_0]).Thresholds[THRESHOLD_ALARM] = 1023;		// never occurs
	(actuatorControl[AXIS_0]).Thresholds[THRESHOLD_HOME] = 512;		// never occurs
	(actuatorControl[AXIS_0]).Thresholds[THRESHOLD_LIMIT] = 512;		// never occurs


	actuatorControl[AXIS_0].OutputPort = &__inputValue;
	actuatorControl[AXIS_0].CurrentSense = currentSense;			// link function
	deviceEnable((deviceBase*)&actuatorControl[AXIS_0],true);

  pinMode(13, OUTPUT);  

  pinMode(2, INPUT_PULLUP);	//test use button
  pinMode(4, INPUT_PULLUP);	//test use button

  motors[AXIS_0].initialize(5, 6, A0, 1023, normalClose);
  //motors[AXIS_1].initialize(10, 11, A1, 1023, normalClose);

  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);

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

	if (digitalRead(2) == LOW){
		actuatorControl[AXIS_0].Command = REQUEST_TO_HOME;
		//Serial.println(analogRead(A0));
		Serial.println(actuatorCurrentDegree(&actuatorControl[AXIS_0]));
	}
	else{

		if (digitalRead(4) == LOW){
			actuatorControl[AXIS_0].Command = REQUEST_TO_LIMIT;
			//Serial.println(analogRead(A0));
			Serial.println(actuatorCurrentDegree(&actuatorControl[AXIS_0]));
		}
		else{
			actuatorControl[AXIS_0].Command = REQUEST_STOP;
		}
	}



}
 
/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
	//---------------------------------
	//	MotorInterpolator-Outputcontrol
	//---------------------------------
	for (int i = 0; i < TOTAL_AXIS; i++)
		motors[i].drive(interpolators[i].convolutionCalculate(inputValue[i]));
	deviceRunning((deviceBase*)&actuatorControl[AXIS_0]);
	inputValue[0] = (float)__inputValue;		//
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

}

unsigned int currentSense()
{
	//linked to filtered sense value
	//Serial.println(analogRead(A0));
	//Serial.println(analogRead(A0));
	return analogRead(A0);
}


