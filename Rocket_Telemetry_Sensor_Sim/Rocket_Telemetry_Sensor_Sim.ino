#include <stdlib.h>

float pitot_tube = 0.0;
float altimeter = 0.0;
float accelerometer = 0.0;

void transmit(String message){
  for (int i = 0; i < message.length(); i++)
    Serial.print(message.charAt(i));
}

void setup(){    
  Serial.begin(9600);   
}

void loop(){
  // Buffer to store message
  char pitot_tube_buffer[15];
  char altitude_buffer[15];
  char acceleration_buffer[15];
  char message[100];

  pitot_tube = (float) random(0,100);
  altimeter = (float) random(0,1800);
  accelerometer = (float) random(-16,16);

  // Format message, begins with P and ends with newline
  dtostrf(pitot_tube, 4, 2, pitot_tube_buffer);
  dtostrf(altimeter, 4, 2, altitude_buffer);
  dtostrf(accelerometer, 4, 2, acceleration_buffer);
  snprintf(message, 100, 
    "Pi:%s Al:%s Ac:%s\n", 
    pitot_tube_buffer, altitude_buffer, acceleration_buffer);
  
  // Transmit every second
  transmit(message);
  delay(1000);
}
