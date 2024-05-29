#include <SPI.h>
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_Sensor.h>
#include <MS5525DSO.h>

// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero
#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

#define ACCELEROMETER_ADDRESS 0x18                        // Accelerometer I2C address (alternative: 0x19)
#define SLP_FL 1018                                       // Average Sea Level pressure (Millibars) in Florida
#define GAS_CONSTANT 287.05                               // Gas constant
#define METERS_TO_FEET 3.28                               // Convert Meters to Feet
#define MILLIBAR_TO_PASCAL 100                            // Convert Millibars to pascals
#define PSI_TO_PASCAL 6894.76                             // Convert PSI to Pascals
#define C_TO_K 273.15                                     // Convert Celsius to Kelvin
#define ERROR -20                                         // Number to return when sensor cannot be read

Adafruit_LIS3DH accelerometer = Adafruit_LIS3DH();        // Initialize Accelerometer
Adafruit_MPL3115A2 barometer = Adafruit_MPL3115A2();      // Initialize Barometer
MS5525DSO pitot_tube = MS5525DSO(pp005GS);                // Initialize pitot tube

float altitude_offset = 0;                                // Offset current altitude by initial reading

// Transmit data from Xbee (Rocket to Ground Station)
void transmit(String message){
  for (int i = 0; i < message.length(); i++)
    Serial.print(message.charAt(i));
}

void setup(){
  Wire.begin();
  Serial.begin(9600); 

  // Check if accelerometer is detected
  if (!accelerometer.begin(ACCELEROMETER_ADDRESS)) {
    Serial.println("Couldnt find sensor:Acc");
    while (1);
  }

  // Check if barometer is detected
  if (!barometer.begin()) {
    Serial.println("Couldnt find sensor:Alt");
    while (1);
  }
  
  // Check if barometer is detected
  if (!pitot_tube.begin()) {
    Serial.println("Couldnt find sensor:Pitot");
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
  // Buffer to store message
  char pitot_tube_buffer[15];
  char altitude_buffer[15];
  char acceleration_buffer[15];
  char message[100];

  double pressure;
  double temp;
  float airspeed;
  float altitude;
  sensors_event_t event;
  float accel;

  // Get airspeed: m/s
  // pressure and temp are read in PSI and Celsius and must be converted to Pascals and Kelvin
  if (pitot_tube.readPressureAndTemperature(&pressure, &temp))
    airspeed = sqrt((2 * (pressure * PSI_TO_PASCAL) * GAS_CONSTANT * (temp * C_TO_K))/(SLP_FL * 100));
  else
    airspeed = ERROR;
  
  // Get altitude: ft
  altitude = barometer.getAltitude();
  if(altitude)
    altitude = (altitude - altitude_offset) * METERS_TO_FEET;
  else 
    altitude = ERROR;
  
  // Get acceleration: m/s^2
  if(accelerometer.getEvent(&event))
    accel = event.acceleration.y;
  else
    accel = ERROR;

  // Format message, begins with P and ends with newline
  dtostrf(airspeed, 4, 2, pitot_tube_buffer);
  dtostrf(altitude, 4, 2, altitude_buffer);
  dtostrf(accel, 4, 2, acceleration_buffer);
  snprintf(message, 100, 
    "Pi:%s Al:%s Ac:%s\n", 
    pitot_tube_buffer, altitude_buffer, acceleration_buffer);
  
  // Transmit every second
  transmit(message);
  delay(1000);
}