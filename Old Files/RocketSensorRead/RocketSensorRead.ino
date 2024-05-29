#include <Wire.h>
#include "SPI.h"
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS3DH lis = Adafruit_LIS3DH();
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
int PitotTubeVal = 0;

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero
#define Serial SerialUSB
#endif

// RX: Arduino pin 2, XBee pin DOUT.  TX:  Arduino pin 3, XBee pin DIN
SoftwareSerial XBee(2, 3);
float altOff = 0; // Offset value for altitude

void setup() {
  Wire.begin();        
  Serial.begin(9600); 
  XBee.begin(9600);

  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }

  if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
    return;
  }

    lis.setRange(LIS3DH_RANGE_16_G);   // 2, 4, 8 or 16 G
  //divide data from airspeed by 4 thats why it was giving a bunch of wierd letters
}
void loop() {
  //write sends data to serial port

  //Pitot Tube
  PitotTubeVal = analogRead(A0)/4;
  transmit("airspeed: "+ PitotTubeVal);
  
  //altitude
  float altm = baro.getAltitude();
  transmit(((altm + 60) * 3.37) + " meters");

  //accelerometer
  sensors_event_t event;
  lis.getEvent(&event);
  /*Display the results (acceleration is measured in m/s^2)*/
   //Serial.print(" \tY: ");
   //Serial.write(event.acceleration.y);
   //Serial.println(" m/s^2 ");
   transmit(" \tY: " + event.acceleration.y + " m/s^2");
}

void calibrateAlt()
{
  transmit("$Calibrating altitude sensor...");
  // Collects 100 acceleration readings

  // Finds average acceleration reading
  for(int i = 0; i < 4; i++)
  {
      float percent = 25*i;
      String completion = String(percent, 2) + "%";
      transmit(completion);
//      altOff += baro.getAltitude();
  }

//  altOff = altOff/4;
//  String adjustment = "Altitude adjusted by " + String((altOff*-3.28084),4) + " ft";
//  transmit(adjustment);

}

void transmit(String message)
{
  for (int i = 0; i < message.length(); i++)
  {
    XBee.write(message.charAt(i));
    Serial.print(message);
    delay(50);
  }
  XBee.write('$'); // Starts a new line
}
