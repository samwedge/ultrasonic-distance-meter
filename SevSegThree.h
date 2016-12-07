//Modified by Sam Wedge 2013 samwedge.co.uk
//Adapted from original code by Dean Reading, 2012 deanreading@hotmail.com
//See .cpp file for info

#ifndef SevSegThree_h
#define SevSegThree_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class SevSegThree
{

public:
  SevSegThree();

  //Public Functions
  void PrintOutput();
  void NewNum(int number_in); 
  void Begin(boolean mode_in,byte C1, byte C2, byte C3, byte UC1, byte UC2, byte UC3, byte UC4, byte UC5, byte UC6, byte UC7);

  //Public Variables

private:
  //Private Functions
  void CreateArray();
  void FindNums();

  //Private Variables
  boolean mode,DigitOn,DigitOff,SegOn,SegOff;
  byte DigitPins[3];
  byte SegmentPins[7];
  boolean lights[3][7];
  byte nums[3];
  int number;
  boolean negative;

};

#endif
