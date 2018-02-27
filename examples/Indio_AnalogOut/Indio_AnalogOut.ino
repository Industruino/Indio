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


void setup()
{
  SerialUSB.begin(9600); 

  Indio.analogWriteMode(1, V10); // Set Analog-Out CH1 to 10V mode (0-10V).
//Indio.analogWriteMode(1, V10_p); // Set Analog-Out CH1 to % 10V mode ( 0-100% -> 0-10V).
//Indio.analogWriteMode(1, V10_raw); // Set Analog-Out CH1 to 10V mode and take raw DAC value (0-4096 -> 0-10V).

  Indio.analogWriteMode(2, mA); // Set Analog-Out CH2 to mA mode (0-20mA).
//Indio.analogWriteMode(2, mA_p); // Set Analog-Out CH2 to % mA mode (0-100% -> 4-20mA).
//Indio.analogWriteMode(2, mA_raw); // Set Analog-Out CH2 to mA mode and take raw DAC value (0-4096 -> 0-20mA).   
  
 }

void loop()
{
  
  Indio.analogWrite(1, 2.67, true); //Set CH2 to 2.67V ("true" will write value to EEPROM of DAC, restoring it after power cycling).
//Indio.analogWrite(2, 33.5, true);  //Set CH2 to 33.5% (approx 3.685V)
//Indio.analogWrite(2, 1000, true);  //Set CH2 DAC to integer value 1000 (approx 2.685V).

  Indio.analogWrite(2, 10.50, false); //Set CH1 to 10.5mA ("false" will not write value to EEPROM of DAC).
//Indio.analogWrite(1, 75, true); //Set CH1 to 75% (approx 16mA).
//Indio.analogWrite(1, 2048, true); //Set CH1 DAC to integer value 2048 (approx 10.5mA).


do { } while (1);  //Do nothing, program finished. Keep waiting.


}

