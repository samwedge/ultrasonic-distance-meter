/*
 Modified by Sam Wedge February 2013 samwedge.co.uk
 Adapted from original code (SevSeg V2.1) by Dean Reading, 2012 (deanreading@hotmail.com)

 Modification from the original code is as follows:
  - A reduction from 4 digits to 3
  - No decimal point is used in this code
  - Requires 10 pins to be used instead of 12
  - Numerical range is from -99 to 999

 */

#include "SevSegThree.h"

SevSegThree::SevSegThree()
{
  //Initial values
  number=888;

}

//Begin
/*******************************************************************************************/
//Set pin modes and turns all displays off
void SevSegThree::Begin(boolean mode_in,byte D1, byte D2, byte D3, byte S1, byte S2, byte S3, byte S4, byte S5, byte S6, byte S7){

  //Assign input values to variables
  //Mode sets what the digit pins must be set at for it to be turned on.  0 for common cathode, 1 for common anode
  mode=mode_in;
  if (mode==1){
    DigitOn=HIGH;
    DigitOff=LOW;
    SegOn=LOW;
    SegOff=HIGH;
  }
  else {
    DigitOn=LOW;
    DigitOff=HIGH;
    SegOn=HIGH;
    SegOff=LOW;
  }

  DigitPins[0]=D1;
  DigitPins[1]=D2;
  DigitPins[2]=D3;
  SegmentPins[0]=S1;
  SegmentPins[1]=S2;
  SegmentPins[2]=S3;
  SegmentPins[3]=S4;
  SegmentPins[4]=S5;
  SegmentPins[5]=S6;
  SegmentPins[6]=S7;

  //Set Pin Modes as outputs
  for (byte digit=0;digit<3;digit++) {
    pinMode(DigitPins[digit], OUTPUT);
  }
  for (byte seg=0;seg<7;seg++) {
    pinMode(SegmentPins[seg], OUTPUT);
  }

  //Turn Everything Off
  //Set all digit pins off.  Low for common anode, high for common cathode
  for (byte digit=0;digit<3;digit++) {
    digitalWrite(DigitPins[digit], DigitOff);
  }
  //Set all segment pins off.  High for common anode, low for common cathode
  for (byte seg=0;seg<7;seg++) {
    digitalWrite(SegmentPins[seg], SegOff);
  }
}

//Refresh Display
/*******************************************************************************************/
//Cycles through each segment and turns on the correct digits for each.
//Leaves everything off
void SevSegThree::PrintOutput(){
  for (byte seg=0;seg<7;seg++) {
    //Turn the relevant segment on
    digitalWrite(SegmentPins[seg],SegOn);

    //For each digit, turn relevant digits on
    for (byte digit=0;digit<3;digit++){
      if (lights[digit][seg]==1) {
        digitalWrite(DigitPins[digit],DigitOn);
      }
      //delay(200); //Uncomment this to see it in slow motion
    }
    //Turn all digits off
    for (byte digit=0;digit<3;digit++){
      digitalWrite(DigitPins[digit],DigitOff);
    }

    //Turn the relevant segment off
    digitalWrite(SegmentPins[seg],SegOff);
  }
}

//New Number
/*******************************************************************************************/
//Function to update the number displayed
void SevSegThree::NewNum(int number_in)
{
  //Feed the inputs into the library's variables
  number=number_in;
  FindNums();
  CreateArray();
}

//Find Digits (Nums)
/*******************************************************************************************/
//Function to find the three individual digits to be displayed from the variable 'number'
void SevSegThree::FindNums() {
  //If the number received is negative, set the flag and make the number positive
  if (number<0) {
    negative=1;
    number=number*-1;
  }
  else {
    negative=0;
  }

  //If the number is out of range, just display '---'
  if ((negative==0 && number>999) || (negative==1 && number>99)) {
    nums[0]=21;
    nums[1]=21;
    nums[2]=21;
  }
  else{
    //Find the three digits
    int total=number;
    if (negative==0) {
      nums[0]=number/100;
      total=total-nums[0]*100;
    }
    else {
      nums[0]=21;
    }
    nums[1]=total/10;
    total=total-nums[1]*10;
    nums[2]=total;

    //If there are zeros, set them to 20 which means a blank
    //However, don't cut out significant zeros
    if (negative==0) {
      if (nums[0]==0){
        nums[0]=20;
        if (nums[1]==0) {
          nums[1]=20;
          if (nums[2]==0) {
            nums[2]=20;
          }
        }
      }
    }
    else {
      if (nums[1]==0) {
        nums[1]=20;
        if (nums[2]==0) {
          nums[2]=20;//FIXME: Might need to remove this line
        }
      }
    }
  }
}

//Create Array
/*******************************************************************************************/
//From the numbers found, says which LEDs need to be turned on
void SevSegThree::CreateArray() {
  for (byte digit=0;digit<3;digit++) {
    switch (nums[digit]){
    case 0: // 0
      lights[digit][0]=1;
      lights[digit][1]=1;
      lights[digit][2]=1;
      lights[digit][3]=1;
      lights[digit][4]=1;
      lights[digit][5]=1;
      lights[digit][6]=0;
      break;
    case 1: // 1
      lights[digit][0]=0;
      lights[digit][1]=1;
      lights[digit][2]=1;
      lights[digit][3]=0;
      lights[digit][4]=0;
      lights[digit][5]=0;
      lights[digit][6]=0;
      break;
    case 2: // 2
      lights[digit][0]=1;
      lights[digit][1]=1;
      lights[digit][2]=0;
      lights[digit][3]=1;
      lights[digit][4]=1;
      lights[digit][5]=0;
      lights[digit][6]=1;
      break;
    case 3: // 3
      lights[digit][0]=1;
      lights[digit][1]=1;
      lights[digit][2]=1;
      lights[digit][3]=1;
      lights[digit][4]=0;
      lights[digit][5]=0;
      lights[digit][6]=1;
      break;
    case 4: // 4
      lights[digit][0]=0;
      lights[digit][1]=1;
      lights[digit][2]=1;
      lights[digit][3]=0;
      lights[digit][4]=0;
      lights[digit][5]=1;
      lights[digit][6]=1;
      break;
    case 5: // 5
      lights[digit][0]=1;
      lights[digit][1]=0;
      lights[digit][2]=1;
      lights[digit][3]=1;
      lights[digit][4]=0;
      lights[digit][5]=1;
      lights[digit][6]=1;
      break;
    case 6: // 6
      lights[digit][0]=1;
      lights[digit][1]=0;
      lights[digit][2]=1;
      lights[digit][3]=1;
      lights[digit][4]=1;
      lights[digit][5]=1;
      lights[digit][6]=1;
      break;
    case 7: // 7
      lights[digit][0]=1;
      lights[digit][1]=1;
      lights[digit][2]=1;
      lights[digit][3]=0;
      lights[digit][4]=0;
      lights[digit][5]=0;
      lights[digit][6]=0;
      break;
    case 8: // 8
      lights[digit][0]=1;
      lights[digit][1]=1;
      lights[digit][2]=1;
      lights[digit][3]=1;
      lights[digit][4]=1;
      lights[digit][5]=1;
      lights[digit][6]=1;
      break;
    case 9: // 9
      lights[digit][0]=1;
      lights[digit][1]=1;
      lights[digit][2]=1;
      lights[digit][3]=1;
      lights[digit][4]=0;
      lights[digit][5]=1;
      lights[digit][6]=1;
      break;
    case 20:// BLANK
      lights[digit][0]=0;
      lights[digit][1]=0;
      lights[digit][2]=0;
      lights[digit][3]=0;
      lights[digit][4]=0;
      lights[digit][5]=0;
      lights[digit][6]=0;
      break;
    case 21: // -
      lights[digit][0]=0;
      lights[digit][1]=0;
      lights[digit][2]=0;
      lights[digit][3]=0;
      lights[digit][4]=0;
      lights[digit][5]=0;
      lights[digit][6]=1;
      break;
    default:
      lights[digit][0]=0;
      lights[digit][1]=0;
      lights[digit][2]=1;
      lights[digit][3]=1;
      lights[digit][4]=1;
      lights[digit][5]=0;
      lights[digit][6]=1;
      break;
    }
  }
}
