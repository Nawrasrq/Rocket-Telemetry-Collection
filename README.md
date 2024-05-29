# Battle of the Rockets 2019 Telemetry Collection
Code meant for the Battle of the Rockets 2019 competition. Transmits telemetry gathered from an accelerometer, altimeter and pitot tube using an Xbee Radio. Telemetry is transmitted from the rocket during its flight and graphed live at the ground station.

Electronics
- Arduino UNO
- Adafruit MPL3115A2 Altimeter (I2C)
- Adafruit LIS3DH Accelerometer (I2C)
- Holybro Air Speed Sensor (SPI)
- Two XBee Pro 60mW PCB Antenna - Series 1 (UART)
- Sparkfun XBee Explorer dongle

Updates
- Cleaned up the Rocket_Telemetry_Transmitter.ino (previously RocketSensorRead.ino) 
- Rewrote the Rocket_Telemetry_Reciever in python using matplotlib instead of the processing.ide
- Created a Rocket_Telemetry_Sensor_Sim for testing (Replaces sensor data with randomized values)
- Included the original files used during the flight in the Old Files folder
