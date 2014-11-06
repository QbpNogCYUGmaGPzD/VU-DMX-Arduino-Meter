/* Simple VU meter with DMX LED par
 /
 / dennisneumann@galmoer.nl
 /
 /
 /
 */

#include <DmxMaster.h>

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

int intenPin = 1;
float inten;

void setup() 
{
  Serial.begin(9600);
  for (int i=0;i<512;i++) {
    DmxMaster.write(i,0);
  }
}

void loop()
{
  unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(3);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   float volts = (peakToPeak * 3.3) / 1024;  // convert to volts
 
inten = mapfloat(volts, 0.0, 3.3, 0.0, 255.0);

//Serial.println(volts);
//Serial.println(inten);

  if (inten < 20) { //Laagste stand
    DmxMaster.write(26,inten);
    DmxMaster.write(20,0);
    DmxMaster.write(14,0);
    DmxMaster.write(8,0);
    DmxMaster.write(7,0);
    DmxMaster.write(1,0);
  } 
  else if (inten < 40) {
    DmxMaster.write(26,inten);
    DmxMaster.write(20,inten);
    DmxMaster.write(14,inten/3);
    DmxMaster.write(8,0);
    DmxMaster.write(7,0);
    DmxMaster.write(1,0);
  } 
  else if (inten < 80) {
    DmxMaster.write(26,inten);
    DmxMaster.write(20,inten);
    DmxMaster.write(14,inten);
    DmxMaster.write(8,inten/3);
    DmxMaster.write(7,inten/2);
    DmxMaster.write(1,0);
  } 
  else if (inten < 120) {
    DmxMaster.write(26,inten);
    DmxMaster.write(20,inten);
    DmxMaster.write(14,inten);
    DmxMaster.write(8,inten/3);
    DmxMaster.write(7,inten/2);
    DmxMaster.write(1,inten/2);
  } 
  else if (inten < 160) { //hoogste stand
    DmxMaster.write(26,inten);
    DmxMaster.write(20,inten);
    DmxMaster.write(14,inten);
    DmxMaster.write(8,inten);
    DmxMaster.write(7,inten);
    DmxMaster.write(1,inten);
  }
}

//The Arduino Map function but for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


