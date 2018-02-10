#include<stdio.h>
#include<math.h>
#include"convolution.h"
extern int dataShift(float* array,int length,float incomingValue);


int main(){
	int a=0;
	float output = 0;
	float array[5] = {1,2,3,4,5};
	FILE* file = fopen("test.txt","w+");
	
	convolution cv;
	/*
	for(int j=0;j<30;j++){
			output = 0;
			dataShift(array,5,0);
			for(int i=0;i<5;i++){
			output += array[i];
			}
			fprintf(file,"%d,%f\n",j,output);
	}
	*/
	//fclose(file);
	
	/*
	for(int i=0;i<20;i++){
		cv.enqueue(i);
	}
	
	for(int i=0;i<WINDOW_LENGTH;i++){
		printf("%f\n",cv.readIndex(i) );
	}
	*/
	
	//------------------------------------
	//	Triangular filter profile , overall area should equal to 1
	//------------------------------------
	float currentHeight = 0;
	float slope = 4 / pow((double)WINDOW_LENGTH,2);		// slope = 4/L^2 , L the filter length
	for(int i=0;i<WINDOW_LENGTH;i++){
		//cv.xt[i] = float(1/(float)WINDOW_LENGTH);
		if(i < (WINDOW_LENGTH/2)){
			// before peak
			cv.xt[i] = (currentHeight += slope);
		}
		else{
			//after peak
			cv.xt[i] = (currentHeight -= slope);
		}	
	}
	
	//------------------------------------
	//	Rectangular input pulse train
	//------------------------------------
	int duringTime = 50;
	int maxVelocity = 255;
	for(int i=0;i<100;i++){
		if(i<duringTime){
					fprintf(file,"%d,%f\n",i,cv.convolutionCalculate(maxVelocity));
		}
		else{
					fprintf(file,"%d,%f\n",i,cv.convolutionCalculate(0));
		}
		// done , the S-curve acc/dec generated
	}
	fclose(file);
	
	return 0;
}
