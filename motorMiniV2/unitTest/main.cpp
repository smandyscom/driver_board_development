#include<convolution.h>
#include<dcBrushControl.h>
extern int k29[];

int dummyFunction()
{
	return 0;
}

int main()
{
	convolution __convolution(29,k29);

	DCBRUSHCONFIGURATION __config = {
		MODE_PWM_DIRECTION,
		0,
		0,
		dummyFunction,
		k29,
		29,
		/*{128,128,128},
		{128,128},*/
	};



	dcBrushControl __motor(__config);


	__motor.pulseValue = 255;

	for (int i = 0; i < 100;i++)
	{
		if (i == 29)
			__motor.pulseValue = 0;

		__motor.drive();
		
	}

	return 0;
}