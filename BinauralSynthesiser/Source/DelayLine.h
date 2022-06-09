#pragma once

#define BUFFER_LENGTH 30000
class DelayLineClass
{
private:
    //const int bufferLength = 30000;
    float circBuffer[BUFFER_LENGTH][2];
    float readIndex[2];
    int writeIndex[2] = { 0, 0 };
    float delayLength[2] = { 20, 20 };
    void writeToBuffer(float sample, int channel);
    float readFromBuffer(int channel);
    void clearDelayLine();
public:
    DelayLineClass();
    void setDelayLength(float delay, int channel);
    float delayProcess(float sample, int channel);
};