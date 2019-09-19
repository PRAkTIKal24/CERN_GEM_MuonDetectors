//Initialising all variables
#include <SevSeg.h>
SevSeg sevseg1;
SevSeg sevseg2;
SevSeg sevseg3;

int row=0;
int y=1;
int v=0;
float f=0;
float counter1=0;
float counter2=0;
float p=0;
float i=0;
int x=1;
float lcd=0;
int disp=0;
int vt=0;
int vp=0;
float t=0;
float ap=0;

float avg1=0;
float avgp1=0;
float avgt1=0;

float avg2=0;
float avgp2=0;
float avgt2=0;

float Disp_ap=0;
float Disp_avgp=0;
float Disp_avgt=0;

float Data_ap=0;
float Data_avgp=0;
float Data_avgt=0;

int hours=0;
int minutes=0;
float analogp=0;
float analogt=0;

void setup() 
{
  byte numDigits = 4; 
  
                     //12,  9, 8, 6
  byte digitPins1[] = {13, 10, 9, 7};
  byte segmentPins1[] = {12, 8, 5, 3, 2, 11, 6, 4};
                       //11, 7, 4, 2, 1, 10, 5, 3
  
                     //12,  9,  8,  6
  byte digitPins2[] = {23, 29, 31, 32};
  byte segmentPins2[] = {25, 33, 28, 24, 22, 27, 30, 26};
                       //11,  7,  4,  2,  1, 10,  5,  3
                       
  byte digitPins3[] = {38, 44, 46, 49}; //Display Pins: 12, 9, 8, 6 (Digit1, Digit2, Digit3, Digit4)
  byte segmentPins3[] = {40, 48, 45, 41, 39, 42, 47, 43}; //Display Pins: 11, 7, 4, 2, 1, 10, 5, 3
  
  sevseg1.begin(COMMON_ANODE, numDigits, digitPins1, segmentPins1);
  sevseg2.begin(COMMON_ANODE, numDigits, digitPins2, segmentPins2);
  sevseg3.begin(COMMON_ANODE, numDigits, digitPins3, segmentPins3);
  
  analogReference(DEFAULT);
  Serial.begin(128000);
   
  Serial.println("LABEL,Time,Seconds,Pressure (mBar),Temperature (C),Atm Pressure (mBar), , ,DATE"); 
} // end of void setup()

void loop() 
{
  // put the main code here, to run repeatedly:
  static unsigned long timer1 = millis();
  static unsigned long timer2 = millis();
  static int deciSeconds = 0;

  //analogReadResolution(10);
 
  //v = analogRead(A0);           //Reading the voltage signals through ADCs
  //vt = analogRead(A2);
  //vp = analogRead(A1);

  counter1 = counter1 + 1;
  counter2 = counter2 + 1;

  avg1 = avg1 + analogRead(A0);               //Calculating the average values of temp, atm pressure and internal pressure
  avgp1 = avgp1 + analogRead(A1);
  avgt1 = avgt1 + analogRead(A2);

  avg2 = avg2 + analogRead(A0);               //Calculating the average values of temp, atm pressure and internal pressure
  avgp2 = avgp2 + analogRead(A1);
  avgt2 = avgt2 + analogRead(A2);


  // It is necessary to refresh the display repeatedly to illuminate the LED continuously // this step does not update the displayed values
  sevseg1.refreshDisplay();          
  sevseg2.refreshDisplay();
  sevseg3.refreshDisplay();
  
  if(millis()>=timer2) // start of display loop
  { 

    Disp_ap = avg2/counter2;
    Disp_avgp = avgp2/counter2;
    Disp_avgt = avgt2/counter2;
        
    p = 100*0.68014*(Disp_ap*5/1024)-26.677;          //Pressure Transducer - One; Here ((avg/counter) * 0.0049) gives a voltage (in VOLTS)
    t = (Data_avgt*0.488758)-(333.52);           //Using the calibration given by the manufacturer
    ap = ((Data_avgp/1023)+0.095)/(0.0009);      //Using the calibration given by the manufacturer
    
    avg2 = 0;
    avgt2 = 0;
    avgp2 = 0;
    counter2 = 0;
    
    sevseg1.setNumber(t, 1);                          //Library to use the digital display.
    sevseg2.setNumber(p, 1);
    sevseg3.setNumber(ap, 0);

    sevseg1.refreshDisplay();          
    sevseg2.refreshDisplay();
    sevseg3.refreshDisplay();

    timer2 += 2000;                                   // This value can be altered to change the display update rate (in ms)

  }// end of display loop

  if(millis()>=timer1) // start of "writing to excel" loop
  {   

    Data_ap = avg1/counter1;
    Data_avgp = avgp1/counter1;
    Data_avgt = avgt1/counter1;
        
    p = 100*0.68014*(Disp_ap*5/1024)-26.677;         //Pressure Transducer - One; Here ((avg/counter) * 0.0049) gives a voltage (in VOLTS)
    t = (Data_avgt*0.488758)-(333.52);           //Using the calibration given by the manufacturer
    ap = ((Data_avgp/1023)+0.095)/(0.0009);      //Using the calibration given by the manufacturer

    
        Serial.print("DATA,TIME");                      //Printing the columns in the excel file 
        Serial.print(",");
        Serial.print(y+timer1/1000);                    // y+timer1/1000 = time from the start of the acquisition (s)
        Serial.print(",");
        Serial.print(p);
        Serial.print(",");
        Serial.print(t);
        Serial.print(",");
        Serial.print(ap);
        Serial.println(",");
    
        // Reset counter and average values to 0
        counter1 = 0;
        avg1 = 0;
        avgt1 = 0;
        avgp1 = 0;
        y = y+1;
        
        if ((y%60) == 0)
        {
          minutes=minutes+1;
          if ((y%3600)==0)//Changing the minutest scale after an hour
          {           
            minutes=0;
          }
        }
        
        if ((y%3600) ==0)
        {
          hours=hours+1;
        }
        
        timer1 += 60000;                //1000 initial              // This value can be altered to change the log rate (in ms)
         
   } // end of "writing to excel" loop

  row++;                              //Increasing the row in serial communication

} // end of void loop
