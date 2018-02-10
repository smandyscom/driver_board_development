#include "convolution.h"


float convolution::	convolutionCalculate(float xi){
	float outputValue = 0;
	this->enqueue(xi);
	for(int i=0;i<WINDOW_LENGTH;i++){
		outputValue += xt[i] * readIndex(i);
	}
	return outputValue;
}

void convolution::	enqueue(float xi){
	ht[writeIndex] = xi;
	writeIndex++;					// increment the writeIndex
	writeIndex %= WINDOW_LENGTH;	// overflow - rewind
}

float convolution::readIndex(int index){
	return ht[(writeIndex + index)%WINDOW_LENGTH];
}


int dataShift(float* array,int length,float incomingValue){
	
	for(int i=length-1;i>0;i--)
		array[i] = array[i-1];
	array[0] = incomingValue;			// post operation , debugged
	return 0;
}
