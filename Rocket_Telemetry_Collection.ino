#include <SPI.h>
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>

// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero
#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

#define XBEE_DOUT 2                                       // Xbee DOUT pin
#define XBEE_DIN 3                                        // XBee DIN pin 
#define PITOT_TUBE_PIN A0                                 // Pitot tube pin
#define ACCELEROMETER_ADDRESS 0x18                        // Accelerometer I2C address (alternative: 0x19)
#define SLP_FL 1018                                       // Average Sea Level pressure in Florida
#define METERS_TO_FEET 3.28                               // Conversion from meters to feet

Adafruit_LIS3DH accelerometer = Adafruit_LIS3DH();        // Initialize Accelerometer
Adafruit_MPL3115A2 barometer = Adafruit_MPL3115A2();      // Initialize Barometer
SoftwareSerial XBee(XBEE_DOUT, XBEE_DIN);                 // Initalize Xbee radio

float altitude_offset = 0;                                // Offset current altitude by initial reading

// Transmit data from Xbee (Rocket to Ground Station)
void transmit(String message){
  for (int i = 0; i < message.length(); i++){
    XBee.write(message.charAt(i));
    Serial.print(message);
    delay(50);
  }

  // Starts a new line
  XBee.write('$'); 
}

void setup(){
  Wire.begin();        
  Serial.begin(9600); 
  XBee.begin(9600);

  // Check if accelerometer is detected
  if (!accelerometer.begin(ACCELEROMETER_ADDRESS)) {
    Serial.println("Couldnt start");
    while (1);
  }

  // Check if barometer is detected
  if (!barometer.begin()) {
    Serial.println("Couldnt find sensor");
    while (1);
  }

  // Set acclerometer range to 16G (Can be set to 2, 4, 8 or 16Gs)
  accelerometer.setRange(LIS3DH_RANGE_16_G);

  // Set barometer mode
  barometer.setMode(MPL3115A2_ALTIMETER);
  
  // Set current sea level pressure and altitude offset 
  barometer.setSeaPressure(SLP_FL);
  altitude_offset = barometer.getAltitude();

}

void loop(){
  // Transmit Pitot Tube data Units: m/s
  float pitot_tube_value = analogRead(PITOT_TUBE_PIN)/4;
  transmit("Pitot:"+ String(pitot_tube_value));
  
  // Transmit Altitude data Units: ft
  float altitude = barometer.getAltitude();
  transmit("Alt:" + String((altitude - altitude_offset) * METERS_TO_FEET));

  // Transmit Accelerometer data Units: m/s^2
  sensors_event_t event;
  accelerometer.getEvent(&event);
  transmit("Acc:" + String(event.acceleration.y));
}