#define UNDER_ARDUINO_PROJECT
#include"C:\Users\smandyscom\Documents\Arduino\libraries\mylibs\timer.h"
#include"c:\Users\smandyscom\Documents\Arduino\libraries\TimerOne-r11\TimerOne.h"
#include"c:\Users\smandyscom\Documents\Arduino\libraries\TimerOne-r11\TimerOne.cpp"
#define TIMER_INTERVAL	10000	//10ms	// unit in micro second
#define UART_BAUT_RATE 9600

#define __MAIN_PROGRAM


//------------------------
//	PCB PINS
//------------------------
#define PIN_OPEN 8//12//8		//blue
#define PIN_CLOSE 12//8//12		//white
#define PIN_STOP 2		// interrupt pin
#define PIN_SENSE A0
//------------------------
//	LIMIT SENSOR
//------------------------
#define PIN_STOP_OPEN 4		//yellow
#define PIN_STOP_CLOSE 5		//yellow

#define SENSE_THRED 200

#define COMMAND_OPEN 0
#define COMMAND_CLOSE 1
#define COMMAND_STOP 2

#define DURANCE_STOP	100	 // 100ms
#define DURANCE_OPEN	40000	// 40s
#define DURANCE_CLOSE	40000	// 40s

//#define __MAIN_PROGRAM

timerDevice __timer;
int ticksPerSecond;
int state = 100;

volatile int latch = LOW;

void setup()
{

  /* add setup code here */
	Serial.begin(UART_BAUT_RATE);

	timerDeviceInitialize(&__timer);
	deviceEnable((deviceBase*)&__timer, true);
	__timer.TimeInterval = TIMER_INTERVAL/1000;
	__timer.Goal = DURANCE_OPEN/__timer.TimeInterval;
	ticksPerSecond = 1000/__timer.TimeInterval;

	pinMode(13, OUTPUT);

	pinMode(PIN_STOP_OPEN, INPUT_PULLUP);
	pinMode(PIN_STOP_CLOSE, INPUT_PULLUP);

	//----------------
	//	Default status
	//----------------

	Timer1.initialize(TIMER_INTERVAL); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
	Timer1.attachInterrupt(timerIsr); // attach the service routine here


	// configure pins
	pinMode(PIN_OPEN, OUTPUT);
	pinMode(PIN_CLOSE, OUTPUT);
	digitalWrite(PIN_OPEN, LOW);
	digitalWrite(PIN_CLOSE, LOW);

	pinMode(PIN_STOP,INPUT_PULLUP);
	attachInterrupt(0,__latch,LOW);	//hsien , 2015

	//command(COMMAND_CLOSE);
	//delay(30000);

	// drive to open
	((deviceBase*)&__timer)->Enable((deviceBase*)&__timer, true);		// restart timer 
	state = 0;
}

int count = 0;
String outputString;

void __latch()
{
	latch = HIGH;
}

//void loopVersion1()
//{
//
//  /* add main program code here */
//	
//	// test use
//	//outputString = String(count);
//	//outputString.concat("(OPEN) : ");
//	//outputString.concat(((float)__timer.count/ticksPerSecond));
//	//Serial.println(outputString);
//	//((deviceBase*)&__timer)->Enable((deviceBase*)&__timer, true);
//	//delay(1000);
//
//
//	//digitalWrite(PIN_STOP, LOW);
//	//digitalWrite(PIN_OPEN, HIGH);
//	//Serial.println(analogRead(PIN_SENSE));
//	//delay(1000);
//
//
//	switch (state)
//	{
//	case 0:
//		//--------------
//		// drive to open
//		//--------------
//		command(COMMAND_OPEN);
//		((deviceBase*)&__timer)->Enable((deviceBase*)&__timer, true);		// restart timer 
//		state = 5;
//		break;
//	case 5:
//		if (analogRead(PIN_SENSE) >= SENSE_THRED)
//		{
//			command(COMMAND_STOP);
//
//			outputString = String(count);
//			outputString.concat("(OPEN) : ");
//			outputString.concat(__timer.count);
//			Serial.println(outputString);
//
//			__timer.Goal = DURANCE_STOP/__timer.TimeInterval;
//			((deviceBase*)&__timer)->Enable((deviceBase*)&__timer, true);		// restart timer 
//			state = 10;	// turn to stop
//
//			count += 1;
//		}
//		else if (timerDeviceIsTicked(&__timer))
//		{
//			// fail to test
//			outputString = String(count);
//			outputString.concat("(OPEN FAIL TIME OUT) : ");
//			outputString.concat(__timer.count);
//			Serial.println(outputString);
//
//			state = 500;
//		}
//
//		break;
//	case 10:
//		//--------------
//		//	Output stop pulse for a while
//		//--------------
//		if (timerDeviceIsTicked(&__timer))
//		{
//			command(COMMAND_CLOSE);
//
//			__timer.Goal = DURANCE_CLOSE / __timer.TimeInterval;
//			((deviceBase*)&__timer)->Enable((deviceBase*)&__timer, true);		// restart timer 
//			state = 100;	// turn to close
//		}
//		break;
//	case 100:
//		// drive to close
//		if (analogRead(PIN_SENSE) == SENSE_THRED)
//		{
//			command(COMMAND_STOP);
//
//			outputString = String(count);
//			outputString.concat("(CLOSE) : ");
//			outputString.concat(__timer.count);
//			Serial.println(outputString);
//
//			__timer.Goal = DURANCE_STOP / __timer.TimeInterval;
//			((deviceBase*)&__timer)->Enable((deviceBase*)&__timer, true);		// restart timer 
//			state = 110;	// turn to stop
//
//		}
//		else if (timerDeviceIsTicked(&__timer))
//		{
//			// fail to test
//			outputString = String(count);
//			outputString.concat("(CLOSE FAIL TIME OUT) : ");
//			outputString.concat(__timer.count);
//			Serial.println(outputString);
//
//			state = 500;
//		}
//
//		break;
//	case 110:
//		//--------------
//		//	Output stop pulse for a while
//		//--------------
//		if (timerDeviceIsTicked(&__timer))
//		{
//			command(COMMAND_STOP);
//
//			__timer.Goal = DURANCE_OPEN / __timer.TimeInterval;
//			((deviceBase*)&__timer)->Enable((deviceBase*)&__timer, true);		// restart timer 
//			state = 5;	// turn to open
//		}
//		break;
//
//	case 500:
//		//----------------
//		//	FAIL TO TEST , STOP
//		//----------------
//		command(COMMAND_STOP);
//	default:
//		break;
//	}
//
//}

void loop()
{
	/*Serial.println("CLOSE");
	command(COMMAND_CLOSE);
	digitalWrite(13, HIGH);
	delay(30 * 1000);


	Serial.println("OPEN");
	command(COMMAND_OPEN);
	digitalWrite(13, LOW);
	delay(30 * 1000);*/
	/* add main program code here */

	/*if(latch == HIGH)
	{
		digitalWrite(13,!digitalRead(13));
		Serial.println("CHANGE");
		latch = LOW;
	}*/
#ifdef __MAIN_PROGRAM

	switch (state)
	{
	case 0:
		//--------------
		// drive to open
		//--------------
		//delay(30 * 1000);

		count += 1;

		Serial.println("OPEN COMMAND");

		//digitalWrite(13, HIGH);
		
		command(COMMAND_OPEN);

		latch = LOW;

		__timer.Goal = DURANCE_OPEN / __timer.TimeInterval;
		((deviceBase*)&__timer)->Enable((deviceBase*)&__timer, true);		// restart timer 
		state = 10;
		break;

	case 10:
		if (latch == HIGH/*digitalRead(PIN_STOP_OPEN) == LOW /*|| (analogRead(PIN_SENSE) == SENSE_THRED)*/)
		{
			//command(COMMAND_STOP);
			log("(OPEN)");
			state = 100;
			latch = LOW;

			delay(10000);	//due to duty cycle issue , Hsien , 2015.04.18
		}
		else if (timerDeviceIsTicked(&__timer))
		{
			// fail to test
			log("(OPEN FAIL)");
			state = 500;
		}
		break;

		//-----------------------
		//	Drive to CLOSE
		//-----------------------
	case 100:
		//delay(30 * 1000);

		command(COMMAND_CLOSE);
		Serial.println("CLOSE COMMAND");
		
		//digitalWrite(13, LOW);
		
		latch = LOW;

		__timer.Goal = DURANCE_CLOSE / __timer.TimeInterval;
		((deviceBase*)&__timer)->Enable((deviceBase*)&__timer, true);		// restart timer 
		
		state = 110;	// turn to stop
		break;
	case 110:
		// drive to close
		if (latch == HIGH/*digitalRead(PIN_STOP_CLOSE) == LOW /*|| (analogRead(PIN_SENSE) == SENSE_THRED)*/)
		{
			//command(COMMAND_STOP);
			log("(CLOSE)");

			state = 0;	// turn to stop
			latch = LOW;

			delay(10000);	//due to duty cycle issue , Hsien , 2015.04.18
		}
		else if (timerDeviceIsTicked(&__timer))
		{
			// fail to test
			log("CLOSE FAIL");
			state = 500;
		}

		break;

	case 500:
		//----------------
		//	FAIL TO TEST , STOP
		//----------------
		//command(COMMAND_STOP);
		log("TEST FAILED");
		delay(1000);		//report every 1 seconds
		break;
	default:
		break;
	}
#endif
}



/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
	//---------------------------------
	//	MotorInterpolator-Outputcontrol
	//---------------------------------
	
	//----------------------------------
	//	Timer
	//----------------------------------
	deviceRunning((deviceBase*)&__timer);
}


void command(char __command)
{
	switch (__command)
	{
	case COMMAND_OPEN: 
		//open
		//digitalWrite(PIN_STOP, HIGH);
		digitalWrite(PIN_OPEN, HIGH);
		digitalWrite(PIN_CLOSE, LOW);

		delay(100);

		digitalWrite(PIN_OPEN, LOW);
		digitalWrite(PIN_CLOSE, LOW);

		//delay(100);

		break;
	case COMMAND_CLOSE:
		//close
		//digitalWrite(PIN_STOP, HIGH);
		digitalWrite(PIN_OPEN, LOW);
		digitalWrite(PIN_CLOSE, HIGH);

		delay(100);

		digitalWrite(PIN_OPEN, LOW);
		digitalWrite(PIN_CLOSE, LOW);

		//delay(100);

		break;
	case COMMAND_STOP:
		//stop
		//digitalWrite(PIN_STOP, LOW);
		digitalWrite(PIN_OPEN, LOW);
		digitalWrite(PIN_CLOSE, LOW);
		// stop
		break;
	default:
		break;
	}
}

void log(const char* __string)
{
	outputString = String(count);
	outputString.concat(__string);
	outputString.concat(" : ");
	outputString.concat(__timer.count);
	outputString.concat(" , ");
	outputString.concat(analogRead(PIN_SENSE));
	outputString.concat(" , ");
	outputString.concat(digitalRead(PIN_STOP_OPEN));
	outputString.concat(" , ");
	outputString.concat(digitalRead(PIN_STOP_CLOSE));
	Serial.println(outputString);
}