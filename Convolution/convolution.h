#ifndef __CONVOLUTION
#define __CONVOLUTION

#define WINDOW_LENGTH 50

class convolution
{

public:
	
	float xt[WINDOW_LENGTH];		// the x-tao data
	
	float convolutionCalculate(float xi);	// queue in input , and export calculated value
	
	convolution(){
		this->writeIndex=0;
		for(int i=0;i<WINDOW_LENGTH;i++){
			ht[i] = 0;
		}
	}
	
//protected:
	
	int writeIndex;
	//int output;						// the y(t)
	
	float ht[WINDOW_LENGTH];		// the h(t-tao) data 
	//---------------------------
	//	Fixed length ring buffer control
	//---------------------------		
	void enqueue(float xi);			// queue xi into ring buffer , wourld override oldest data
	float readIndex(int index);		// read specific index of ring buffer
};

#endif
