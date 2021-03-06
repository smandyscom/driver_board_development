#include <sensingModule.h>
#include <device.h>
#include <dcBrushControl.h>
#include <cwccwControl.h>
#include <convolution.h>
#include <actuator.h>
#include <LiquidCrystal.h>
#include <TimerOne.h>

#include <timer.h>
//------------------------------------
//	Used to measure Stoke time for DC motor
//------------------------------------

#define CHANNEL_1 0
#define CHANNEL_2 1
#define TOTAL_CHANNEL 2

#define FORWARD 1
#define STOP 0
#define BACKWARD -1

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

char* __ch1 = "CH1:";
//declare channel data

extern void timerDeviceEnable(timerDevice* __timerDevice, bool isEnable);

int lcd_key = 0;
int adc_key_in = 0;
int read_LCD_buttons(){               // read the buttons
	adc_key_in = analogRead(0);       // read the value from the sensor 

	// my buttons when read are centered at these valies: 0, 144, 329, 504, 741
	// we add approx 50 to those values and check to see if we are close
	// We make this the 1st option for speed reasons since it will be the most likely result

	if (adc_key_in > 1000) return btnNONE;

	// For V1.1 us this threshold
	if (adc_key_in < 50)   return btnRIGHT;
	if (adc_key_in < 250)  return btnUP;
	if (adc_key_in < 450)  return btnDOWN;
	if (adc_key_in < 650)  return btnLEFT;
	if (adc_key_in < 850)  return btnSELECT;

	// For V1.0 comment the other threshold and use the one below:
	/*
	if (adc_key_in < 50)   return btnRIGHT;
	if (adc_key_in < 195)  return btnUP;
	if (adc_key_in < 380)  return btnDOWN;
	if (adc_key_in < 555)  return btnLEFT;
	if (adc_key_in < 790)  return btnSELECT;
	*/

	return btnNONE;                // when all others fail, return this.
}


LiquidCrystal lcd(8,9,4,5,6,7);		// initial the LCD

struct channelData
{
	timerDevice __timer;
	unsigned int __snapedTimerCount;	//used to memorize timer count
	float __snapedTimerValue;			// timerCount/timerInterval

	int manualTriggerButton;			// get from read_from_LCD

	unsigned char forwordOutput;
	unsigned char backwordOutput;
	
	unsigned char firstLimit;
	unsigned char endLimit;

	char* preFix;	// e.g "CH1:"
	int lineIndex;			// for LCD
	int startCharIndex;		// for LCD
	char*	visualBuffer;		//for LCD , used as temp buffer to handle string

	int currentStep;	// the sequence
};

struct channelData __datas[TOTAL_CHANNEL];


void channelInitialize(struct channelData* __data)
{
	//memset(__data, 0, sizeof(channelData));
	//__data->__timer = new timerDevice;
	timerDeviceInitialize(&(__data->__timer));

	pinMode(__data->forwordOutput,OUTPUT);
	pinMode(__data->backwordOutput, OUTPUT);


	channelOutput(__data, STOP);		//stop in default


	pinMode(__data->firstLimit, INPUT_PULLUP);	//as low active
	pinMode(__data->endLimit, INPUT_PULLUP);

	__data->visualBuffer = (char*)malloc(16);
	//pinMode(__data->manualTriggerButton, INPUT);
}

void channelOutput(struct channelData* __data,char __direction)
{
	// control the Mutually Exclusive output
	// the LOW-ACTIVE mode
	switch (__direction)
	{
	case FORWARD:
		digitalWrite(__data->backwordOutput, HIGH);
		delay(10);	//delay for awhile for debouncing
		digitalWrite(__data->forwordOutput, LOW);
		break;
	case BACKWARD:
		digitalWrite(__data->forwordOutput, HIGH);
		delay(10);	//delay for awhile for debouncing
		digitalWrite(__data->backwordOutput, LOW);
		break;
	case STOP:
		digitalWrite(__data->forwordOutput, HIGH);
		digitalWrite(__data->backwordOutput, HIGH);
		break;
	default:
		break;
	}
}

void channelPrint(struct channelData* __data,const char* __string)
{
	//print the prefix
	// set the cursor
	// print additional string
	/*String __temp;
	__temp.concat(__data->preFix);
	__temp.concat(string);
	lcd.println(__temp);*/
	memset(__data->visualBuffer, 0, 16);
	lcd.setCursor(0, __data->lineIndex);

	//strcat(__data->visualBuffer, __data->preFix);
	//strcat(__data->visualBuffer, ":");
	//strcat(__data->visualBuffer, __string);
	sprintf(__data->visualBuffer, "%s%c%s", __data->preFix, ':', __string);


	/*lcd.print(__data->preFix);
	lcd.print(':');
	lcd.print(__string);*/
	lcd.print(__data->visualBuffer);
}
void channelPrint(struct channelData* __data, float value)
{
	//print the prefix
	// set the cursor
	// print additional string
	char floatBuffer[10];
	memset(floatBuffer, '.', 10);
	memset(__data->visualBuffer, '.', 16);
	lcd.setCursor(0, __data->lineIndex);

	/*strcat(__data->visualBuffer, __data->preFix);
	strcat(__data->visualBuffer, ":");
	strcat(__data->visualBuffer, );*/
	dtostrf(value, 4, 3, floatBuffer);

	sprintf(__data->visualBuffer, "%s%c%s%s", __data->preFix, ':', floatBuffer,"s...");

	//lcd.print(__data->preFix);
	//lcd.print(':');
	//lcd.print(value);

	lcd.print(__data->visualBuffer);

}

void channelRunning(struct channelData* __data)
{
	//-------------------------------
	//	Steps:
	//		1. presume : both motor are extract to ready position
	//		2. wait for used to trigger measuring/backward trigger
	//		3. once channel is triggered , output the forward digital pin , raise motor moving
	//		4. wait first limit sensor sensed
	//		5. once first limit sensor reached , start timer countring
	//		6. once end limit sensor reached , end of timer counting , output the timer total count , start the backward
	//		7. backward until first limit raised
	//		8. backward until first limit fallen , end of procedure

	// the sequence control for singal channel
	switch (__data->currentStep)
	{
	case 0:
		if (read_LCD_buttons()==__data->manualTriggerButton){
			channelOutput(__data, FORWARD);	//trigger to the prepare position
			__data->currentStep += 10;
		}
		else
			// wait triggering
			channelPrint(__data, "PRESS TRIG..");
		break;
	case 10:
		if (digitalRead(__data->firstLimit) == LOW){
			//deviceEnable((deviceBase*)(&(__data->__timer)), false);		// restart the timer counting
			deviceEnable((deviceBase*)(&(__data->__timer)), true);		// restart the timer counting
			//timerDeviceEnable(&__data->__timer, true);	//this line worked , Hsien  , 2015.06.01
			//deviceEnable((deviceBase*)&(__datas[CHANNEL_1].__timer), true);		// restart the timer counting
			//timerDeviceInitialize(&__data->__timer);	
			__data->__timer.count = 0;		//due to reference failure issue , direct override count to reset timer , Hsien , 2015.06.01
			__data->currentStep += 10;
		}
		else
			// wait until first limit reached
			channelPrint(__data, "PREPARING..");
		break;
	case 20:
		if (digitalRead(__data->endLimit) == LOW){
			__data->__snapedTimerCount = __data->__timer.count;	//memorize the timer count
			__data->__snapedTimerValue = (float)(__data->__snapedTimerCount *0.01f);

			
			channelPrint(__data, __data->__snapedTimerValue);
			Serial.println(__data->__snapedTimerValue);
			Serial.println(__data->__timer.count);

			channelOutput(__data, STOP);		// stop motion

			__data->currentStep += 10;
		}
		else
			// wait until end limit reached
			channelPrint(__data, "MEASURING...");
		break;
	case 30:
		if (read_LCD_buttons() == __data->manualTriggerButton){
			channelOutput(__data, BACKWARD);		// start backward motion
			__data->currentStep += 10;
		}
		else
			// wait user confirm
			//channelPrint(__data, "PRESS BACK...");
		break;
	case 40:
		if (digitalRead(__data->firstLimit) == LOW){
			__data->currentStep += 10;
		}
		else
			// wait until first limit reached
			channelPrint(__data, "SEARCHING 1-LIMIT");
		break;
	case 50:
		if (digitalRead(__data->firstLimit) == HIGH){
			delay(1000);	//delay for a while  , then stop
			channelOutput(__data, STOP);		// stop whole motion , measure cycle end
			__data->currentStep = 0;
		}
		else
			// wait until first limit leaved
			channelPrint(__data, "LEAVING 1-LIMIT");
		break;
	case 60:
		// reserve
		break;
	default:
		break;
	}


}


void setup()
{
	struct channelData* __tempReference ;
	//---------------------
	//	CH1 Mapping
	//---------------------
	__tempReference = &(__datas[CHANNEL_1]);
	__tempReference->forwordOutput = A5;
	__tempReference->backwordOutput = A4;
	__tempReference->firstLimit = A3;
	__tempReference->endLimit = A2;
	//---------------------
	//	CH2 Mapping
	//---------------------
	__tempReference = &__datas[CHANNEL_2];
	__tempReference->forwordOutput = 0;
	__tempReference->backwordOutput = 1;
	__tempReference->firstLimit = 2;
	__tempReference->endLimit = 3;

	// had initialize first
	for (int i = 0; i < TOTAL_CHANNEL; i++)
		channelInitialize(&(__datas[i]));

	Serial.begin(115200);

	//---------------------
	//	CH1 LCD Mapping
	//---------------------
	__tempReference = &__datas[CHANNEL_1];
	__tempReference->preFix = "CH1";
	__tempReference->lineIndex = 0;
	__tempReference->manualTriggerButton = btnUP;
	//---------------------
	//	CH2 LCD Mapping
	//---------------------
	__tempReference = &__datas[CHANNEL_2];
	__tempReference->preFix = "CH2";
	__tempReference->lineIndex = 1;
	__tempReference->manualTriggerButton = btnDOWN;



	// timerOne initilaize
	Timer1.initialize(10000);		//10ms a calling
	Timer1.attachInterrupt(timerInterrupt);
	//Timer1.start();
  /* add setup code here */
	lcd.begin(16, 2);

	//channelPrint(__tempReference, 0.5634);
	//lcd.println("CH1:LINE");
	//memset(__datas[CHANNEL_1].visualBuffer, 0, 16);
	//dtostrf(3.1415, 3, 3, __datas[CHANNEL_1].visualBuffer);
	////sprintf(__datas[CHANNEL_1].visualBuffer, "%f", 3.1415f);
	//lcd.print(__datas[CHANNEL_1].visualBuffer);
}

void loop()
{
	//-------------------------------
	//	Steps:
	//		1. presume : both motor are extract to ready position
	//		2. wait for used to trigger measuring/backward trigger
	//		3. once channel is triggered , output the forward digital pin , raise motor moving
	//		4. wait first limit sensor sensed
	//		5. once first limit sensor reached , start timer countring
	//		6. once end limit sensor reached , end of timer counting , output the timer total count , start the backward
	//		7. backward until first limit raised
	//		8. backward until first limit fallen , end of procedure


  /* add main program code here */
	//lcd.setCursor(9, 1);             // move cursor to second line "1" and 9 spaces over
	//lcd.print(millis() / 1000);       // display seconds elapsed since power-up

	//lcd.setCursor(0, 1);             // move to the begining of the second line
	//lcd_key = read_LCD_buttons();   // read the buttons

	//switch (lcd_key){               // depending on which button was pushed, we perform an action

	//case btnRIGHT:{             //  push button "RIGHT" and show the word on the screen
	//	lcd.print("RIGHT ");
	//	break;
	//}
	//case btnLEFT:{
	//	lcd.print("LEFT   "); //  push button "LEFT" and show the word on the screen
	//	break;
	//}
	//case btnUP:{
	//	lcd.print("UP    ");  //  push button "UP" and show the word on the screen
	//	break;
	//}
	//case btnDOWN:{
	//	lcd.print("DOWN  ");  //  push button "DOWN" and show the word on the screen
	//	break;
	//}
	//case btnSELECT:{
	//	lcd.print("SELECT");  //  push button "SELECT" and show the word on the screen
	//	break;
	//}
	//case btnNONE:{
	//	lcd.print("NONE  ");  //  No action  will show "None" on the screen
	//	break;
	//}
	//}
	/*int i = 0;
	if (read_LCD_buttons() == btnUP){
		i = 2;
	}*/


	 //had initialize first
	for (int i = 0; i < TOTAL_CHANNEL; i++)
		channelRunning(&(__datas[i]));

}

void timerInterrupt()
{
	// roll over the timer
	for (int i = 0; i < TOTAL_CHANNEL; i++)
		deviceRunning((deviceBase*)&(__datas[i].__timer));
}

