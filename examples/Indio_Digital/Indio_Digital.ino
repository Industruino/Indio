/* Indio Library, by Rustem Iskuzhin for Industruino - www.industruino.com
This library enables communication with the Industruino IND.I/O Baseboard.
The IND.I/O Baseboard has 8CH of 24V digital I/O (6.5-32V supported), 4CH 0-10V/4-20mA ADC, 2CH 0-10V/4-20mA DAC, RS485 transceiver.


WARNING!: PLEASE BE VERY CAREFUL WHEN CONNECTING PERIPHERALS TO YOUR IND.I/O AND CONFIGURING THE DIRECTION OF THE I/O.
          THE FAILURE MODE OF A 24V SHORTED CONNECTION IS MUCH MORE SPECTACULAR THAN WHEN WORKING AT 5V. 

*/

#include <Indio.h>
#include <Wire.h>


void setup()
{

// IND.I/O Baseboard has 8 channels of bidirectional 24V I/O (6.5-32V supported). 
// Here we choose the direction of each I/O channel. 
  
   Indio.digitalMode(1,INPUT);  // Set CH1 as an input
   Indio.digitalMode(2,INPUT);  // Set CH2 as an input
   Indio.digitalMode(3,INPUT);  // Set CH3 as an input
  
   Indio.digitalMode(7,OUTPUT);  // Set CH7 as an output
   Indio.digitalMode(8,OUTPUT);  // Set CH8 as an output
   
 }

void loop()
{

  Serial.print(Indio.digitalRead(1),BIN); // Read CH1 one, print state to serial.
  Serial.print(Indio.digitalRead(2),BIN); // Read CH2 one, print state to serial.
  Serial.print(Indio.digitalRead(3),BIN); // Read CH3 one, print state to serial.
  Serial.print("\r\n"); // Print a new line to serial.

  Indio.digitalWrite(7,HIGH); // Set CH7 to high (24V, or whatever your Vin voltage).
  Indio.digitalWrite(8,HIGH); // Set CH8 to high (24V, or whatever your Vin voltage).
  
  delay(2000);

  Serial.print(Indio.digitalRead(1),BIN); // Read CH1 one, print state to serial.
  Serial.print(Indio.digitalRead(2),BIN); // Read CH2 one, print state to serial.
  Serial.print(Indio.digitalRead(3),BIN); // Read CH3 one, print state to serial.
  Serial.print("\r\n");
  
  Indio.digitalWrite(7,LOW); // Set CH7 to low (0V).
  Indio.digitalWrite(8,LOW); // Set CH8 to low (0V)
  
  delay(2000);
  
}

