#ifndef RUNNING_AVERAGE_HPP
#define RUNNING_AVERAGE_HPP

#include <Arduino.h>
//#include <Serial.h>

template <class arrayClass, uint32_t bufferSize>
class Running_Average
{
  private:
	   arrayClass buffer[bufferSize];

    public:
	     arrayClass sum = 0;
	     arrayClass average = 0;
	     arrayClass Average(void); // Member functions declaration
	     void AddSample(arrayClass);
	     Running_Average(void);
};

template <class arrayClass, uint32_t bufferSize>
Running_Average<arrayClass, bufferSize>::Running_Average(void)
{
  //empty for now, nothing is needed if we use templates
  //instead of vectors
}

template <class arrayClass, uint32_t bufferSize>
void Running_Average<arrayClass, bufferSize>::AddSample(arrayClass val)  //Add new values to buffer
{
	for (uint32_t i = bufferSize-1; i>0; --i)
	{
		buffer[i] = buffer[i - 1];
	}
	buffer[0] = val;
}

template <class arrayClass, uint32_t bufferSize>
arrayClass Running_Average<arrayClass, bufferSize>::Average(void) //Calculate Average of current values in buffer
{
	uint32_t i = 0;
	for (i = 0; i<bufferSize; i++)
	{
		//Serial.println(buffer[i]);
		sum += buffer[i];
	}
	average = sum / bufferSize;
	sum = 0;
	return average;
}

#endif
