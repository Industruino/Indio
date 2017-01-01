/* Indio Library, by Rustem Iskuzhin for Industruino - wwww.industruino.com
This library enables communication with the Industruino IND.I/O Baseboard.
The IND.I/O Baseboard has 8CH of 24V digital I/O (6.5-32V supported), 4CH 0-10V/4-20mA ADC, 2CH 0-10V/4-20mA DAC, RS485 transceiver.


WARNING!: PLEASE BE VERY CAREFUL WHEN CONNECTING PERIPHERALS TO YOUR IND.I/O AND CONFIGURING THE DIRECTION OF THE I/O.
          THE FAILURE MODE OF A 24V SHORTED CONNECTION IS MUCH MORE SPECTACULAR THAN WHEN WORKING AT 5V. 


Please find the calibration data array inside the Indio.cpp library file, together with an explanation on how to perform the calibration. 
Library is preloaded with calibration data but characterisctics are board specific thus reading with standard cal. data might be off.
*/

#include <Indio.h>
#include <Wire.h>


float sensorVal1, sensorVal2, sensorVal3, sensorVal4; //variables to hold your sensor data

void setup()
{
  
  Serial.begin(9600); 
   while (!Serial)
   {
     // wait for serial port to connect. Needed for Leonardo only
   }

  Indio.setADCResolution(16); // Set the ADC resolution. Choices are 12bit@240SPS, 14bit@60SPS, 16bit@15SPS and 18bit@3.75SPS.

  Indio.analogReadMode(1, V10); // Set Analog-In CH1 to 10V mode (0-10V).
  Indio.analogReadMode(2, V10_p); // Set Analog-In CH2 to % 10V mode (0-10V -> 0-100%).
  Indio.analogReadMode(3, mA); // Set Analog-In CH3 to mA mode (0-20mA).
  Indio.analogReadMode(4, mA_p); // Set Analog-In CH4 to % mA mode (4-20mA -> 0-100%)
//  Indio.analogReadMode(4, V5); // Set Analog-In CH4 to 5V mode (2x gain enabled on ADC).
//  Indio.analogReadMode(4, V5_p); // Set Analog-In CH4 to 5V mode (0-5V -> 0-100%).
//  Indio.analogReadMode(4, V10_raw); // Set Analog-In CH4 to 10V mode and read raw ADC value (0-10V -> 0-4096).
//  Indio.analogReadMode(4, mA_raw); // Set Analog-In CH4 to mA mode and read raw ADC value (0-20mA -> 0-4096).
  
 }

void loop()
{

  sensorVal1=Indio.analogRead(1); //Read Analog-In CH1 (output depending on selected mode)
  Serial.print("CH1: "); //Print "CH" for human readability
  Serial.print(sensorVal1, 3); //Print data
  Serial.print("  "); //Add some "  " space
  

  sensorVal2=Indio.analogRead(2); //Read Analog-In CH1 (output depending on selected mode)
  Serial.print("CH2: "); //Print "CH" for human readability
  Serial.print(sensorVal2, 3); //Print data
  Serial.print("  "); //Add some "  " space


  sensorVal3=Indio.analogRead(3); //Read Analog-In CH1 (output depending on selected mode)
  Serial.print("CH3: "); //Print "CH" for human readability
  Serial.print(sensorVal3, 3); //Print data
  Serial.print("  "); //Add some "  " space
 

  sensorVal4=Indio.analogRead(4); //Read Analog-In CH1 (output depending on selected mode)
  Serial.print("CH4: "); //Print "CH" for human readability
  Serial.print(sensorVal4, 3); //Print data
  Serial.print("  "); //Add some "  " space
 
  Serial.print("\r\n"); // Print a new line to serial.

}

