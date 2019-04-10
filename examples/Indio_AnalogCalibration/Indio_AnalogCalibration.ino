/*
* Industruino Demo Code - IND.I/O analog I/O calibration
* universal version for 1286 and 32u4 (does not use membrane buttons)
* 
* INSTRUCTIONS:
*
* Connect your INDIO to an external power supply and to your laptop via USB
* Upload this sketch, open the Serial Monitor, and follow the instructions
*
* Calibration data will appear in the Serial Monitor after each step (ADC_voltage, ADC_current, DAC_voltage, DAC_current)
*
* The resulting calibration data have to replace the default values in your Indio library file Indio.cpp (approximately lines 41 to 75)
* There are 4x 4 lines and each block of 4 lines refers to one type of I/O: ADC_voltage, ADC_current, DAC_voltage, DAC_current
* After each step the Serial Monitor shows the calibration data for that step; use these to update the corresponding 4 lines in Indio.cpp
*
* The USB connection may drop at the end of step 4, this is a known issue but does not affect the functionality of this sketch
* Tom Tobback for Industruino - September 2016
*
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <Indio.h>
#include <Wire.h>

#include <UC1701.h>
static UC1701 lcd;

const int inputChannels = 4;      // number of input channels
const int outputChannels = 2;     // number of output channels

int ADC_current_low_raw[inputChannels];     // arrays to hold calibration data per channel for Low and High voltage and current
int ADC_current_low_uA[inputChannels];
int ADC_current_high_raw[inputChannels];
int ADC_current_high_uA[inputChannels];

int ADC_voltage_low_raw[inputChannels];
int ADC_voltage_low_mV[inputChannels];
int ADC_voltage_high_raw[inputChannels];
int ADC_voltage_high_mV[inputChannels];

int DAC_current_low_raw[outputChannels];
int DAC_current_low_uA[outputChannels];
int DAC_current_high_raw[outputChannels];
int DAC_current_high_uA[outputChannels];

int DAC_voltage_low_raw[outputChannels];
int DAC_voltage_low_mV[outputChannels];
int DAC_voltage_high_raw[outputChannels];
int DAC_voltage_high_mV[outputChannels];

byte i = 0;                  // counter

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {

  pinMode(26, OUTPUT); //set backlight pin to output   
  digitalWrite(26, HIGH); // backlight on

  lcd.begin();
  lcd.clear();

  screenWelcome(); //load first menu

  SerialUSB.begin(9600);
  while (!SerialUSB) {};

  serialMenu();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

  int option = SerialUSB.parseInt();
  if (option < 1 || option > 4) return;
  else {
    SerialUSB.println(option);
    switch (option) {
      case 1:
        DAC_voltage();
        break;
      case 2:
        DAC_current();
        break;
      case 3:
        ADC_voltage();
        break;
      case 4:
        ADC_current();
        break;
    }
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void ADC_voltage() {

  SerialUSB.println(F("ADC calibration voltage mode: 0-10V"));
  SerialUSB.println(F("==================================="));
  screenADC_voltage_low();

  Indio.setADCResolution(16);
  for (i = 1; i <= inputChannels; i++) {
    Indio.analogReadMode(i, V10_raw);
  }

  SerialUSB.println();
  SerialUSB.println(F("Connect known LOW voltage to all analog input channels, e.g. 2500mV"));
  SerialUSB.print(F("Enter the voltage (in mV) in the Serial Monitor box above. Received (mV): "));
  int voltage = getSerialInt(0, 10000);
  SerialUSB.println(voltage);

  for (i = 1; i <= inputChannels; i++) {
    ADC_voltage_low_mV[i]  = voltage;
    ADC_voltage_low_raw[i] = Indio.analogRead(i);                                                // read Analog IN at same time
  }

  SerialUSB.println();
  SerialUSB.print(F("Calibration data ADC_voltage_low_mV: "));
  for (i = 1; i < inputChannels; i++) {
    SerialUSB.print(ADC_voltage_low_mV[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(ADC_voltage_low_mV[inputChannels]);

  SerialUSB.print(F("Calibration data ADC_voltage_low_raw: "));
  for (i = 1; i < inputChannels; i++) {
    SerialUSB.print(ADC_voltage_low_raw[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(ADC_voltage_low_raw[inputChannels]);

  screenADC_voltage_high();
  SerialUSB.println();
  SerialUSB.println(F("Connect known HIGH voltage to all analog input channels, e.g. 7500mV"));
  SerialUSB.print(F("Enter the voltage (in mV) in the Serial Monitor box above. Received (mV): "));
  voltage = getSerialInt(0, 10000);
  SerialUSB.println(voltage);

  for (i = 1; i <= inputChannels; i++) {
    ADC_voltage_high_mV[i]  = voltage;
    ADC_voltage_high_raw[i] = Indio.analogRead(i);                                                // read Analog IN at same time
  }

  SerialUSB.println();
  SerialUSB.print(F("Calibration data ADC_voltage_high_mV: "));
  for (i = 1; i < inputChannels; i++) {
    SerialUSB.print(ADC_voltage_high_mV[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(ADC_voltage_high_mV[inputChannels]);

  SerialUSB.print(F("Calibration data ADC_voltage_high_raw: "));
  for (i = 1; i < inputChannels; i++) {
    SerialUSB.print(ADC_voltage_high_raw[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(ADC_voltage_high_raw[inputChannels]);

  screenDone();
  serialMenu();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void ADC_current() {
  SerialUSB.println(F("ADC calibration current mode: 4-20mA"));
  SerialUSB.println(F("===================================="));
  SerialUSB.println();

  Indio.setADCResolution(12);
  for (i = 1; i <= inputChannels; i++) {
    Indio.analogReadMode(i, mA_raw);
  }

  screenADC_current();

  for (i = 1; i <= inputChannels; i++) {                                          // read Analog IN one by one

    SerialUSB.print(F("Connect a known LOW current to input channel "));
    SerialUSB.print(i);
    SerialUSB.println(F(", e.g. 10000uA"));
    SerialUSB.print(F("Enter the voltage (in uA) in the Serial Monitor box above. Received (uA): "));
    ADC_current_low_uA[i] = getSerialInt(0, 20000);
    SerialUSB.print(ADC_current_low_uA[i]);

    ADC_current_low_raw[i] = Indio.analogRead(i);
    SerialUSB.print(F(" Measured (raw): "));
    SerialUSB.println(ADC_current_low_raw[i]);
    //    SerialUSB.println();
  }

  SerialUSB.println();
  SerialUSB.print(F("Calibration data ADC_current_low_uA: "));
  for (i = 1; i < inputChannels; i++) {
    SerialUSB.print(ADC_current_low_uA[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(ADC_current_low_uA[inputChannels]);

  SerialUSB.print(F("Calibration data ADC_current_low_raw: "));
  for (i = 1; i < inputChannels; i++) {
    SerialUSB.print(ADC_current_low_raw[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(ADC_current_low_raw[inputChannels]);
  SerialUSB.println();


  for (i = 1; i <= inputChannels; i++) {                                          // read Analog IN one by one

    SerialUSB.print(F("Connect a known HIGH current to input channel "));
    SerialUSB.print(i);
    SerialUSB.println(F(", e.g. 20000uA"));
    SerialUSB.print(F("Enter the voltage (in uA) in the Serial Monitor box above. Received (uA): "));
    ADC_current_high_uA[i] = getSerialInt(0, 20001);
    SerialUSB.print(ADC_current_high_uA[i]);

    ADC_current_high_raw[i] = Indio.analogRead(i);
    SerialUSB.print(F(" Measured (raw): "));
    SerialUSB.println(ADC_current_high_raw[i]);
    //    SerialUSB.println();
  }


  SerialUSB.println();
  SerialUSB.print(F("Calibration data ADC_current_high_uA: "));
  for (i = 1; i < inputChannels; i++) {
    SerialUSB.print(ADC_current_high_uA[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(ADC_current_high_uA[inputChannels]);

  SerialUSB.print(F("Calibration data ADC_current_high_raw: "));
  for (i = 1; i < inputChannels; i++) {
    SerialUSB.print(ADC_current_high_raw[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(ADC_current_high_raw[inputChannels]);

  ////////////////// WILL HANG HERE  /////////////////////////
  screenDone();      //// NEXT LCD DISPLAY HANGS
  serialMenu();

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

void DAC_voltage() {
  SerialUSB.println(F("DAC calibration voltage mode: 0-10V"));
  SerialUSB.println(F("==================================="));
  SerialUSB.println();

  for (i = 1; i <= outputChannels; i++) {
    Indio.analogWriteMode(i, V10_raw);
    DAC_voltage_low_raw[i] = 1000;
    Indio.analogWrite(i, DAC_voltage_low_raw[i], true);
  }

  for (i = 1; i <= outputChannels; i++) {
    screenDAC_voltage();
    SerialUSB.print(F("Measure the VOLTAGE (in mV) on channel "));
    SerialUSB.println(i);
    SerialUSB.print(F("Enter the voltage (in mV) in the Serial Monitor box above. Received (mV): "));
    DAC_voltage_low_mV[i] = getSerialInt(0, 10000);
    SerialUSB.println(DAC_voltage_low_mV[i]);
  }

  SerialUSB.println();
  SerialUSB.print(F("Calibration data DAC_voltage_low_raw: "));
  for (i = 1; i < outputChannels; i++) {
    SerialUSB.print(DAC_voltage_low_raw[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(DAC_voltage_low_raw[outputChannels]);

  SerialUSB.print(F("Calibration data DAC_voltage_low_mV: "));
  for (i = 1; i < outputChannels; i++) {
    SerialUSB.print(DAC_voltage_low_mV[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(DAC_voltage_low_mV[outputChannels]);
  SerialUSB.println();

  for (i = 1; i <= outputChannels; i++) {
    DAC_voltage_high_raw[i] = 3600;
    Indio.analogWrite(i, DAC_voltage_high_raw[i], true);
  }

  for (i = 1; i <= outputChannels; i++) {
    screenDAC_voltage();
    SerialUSB.print(F("Measure the VOLTAGE (in mV) on channel "));
    SerialUSB.println(i);
    SerialUSB.print(F("Enter the voltage (in mV) in the Serial Monitor box above. Received (mV): "));
    DAC_voltage_high_mV[i] = getSerialInt(0, 15000);
    SerialUSB.println(DAC_voltage_high_mV[i]);
  }

  SerialUSB.println();
  SerialUSB.print(F("Calibration data DAC_voltage_high_raw: "));
  for (i = 1; i < outputChannels; i++) {
    SerialUSB.print(DAC_voltage_high_raw[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(DAC_voltage_high_raw[outputChannels]);

  SerialUSB.print(F("Calibration data DAC_voltage_high_mV: "));
  for (i = 1; i < outputChannels; i++) {
    SerialUSB.print(DAC_voltage_high_mV[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(DAC_voltage_high_mV[outputChannels]);
  SerialUSB.println();

  screenDone();
  serialMenu();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void DAC_current() {
  SerialUSB.println(F("DAC calibration current mode: 4-20mA"));
  SerialUSB.println(F("===================================="));

  for (i = 1; i <= outputChannels; i++) {
    Indio.analogWriteMode(i, mA_raw);
    DAC_current_low_raw[i] = 1000;
    Indio.analogWrite(i, DAC_current_low_raw[i], true);
  }

  for (i = 1; i <= outputChannels; i++) {
    screenDAC_current();
    SerialUSB.print(F("Measure the current (in uA) on channel "));
    SerialUSB.println(i);
    SerialUSB.print(F("Enter the current (in uA) in the Serial Monitor box above. Received (uA): "));
    DAC_current_low_uA[i] = getSerialInt(0, 20000);
    SerialUSB.println(DAC_current_low_uA[i]);
  }

  SerialUSB.println();
  SerialUSB.print(F("Calibration data DAC_current_low_raw: "));
  for (i = 1; i < outputChannels; i++) {
    SerialUSB.print(DAC_current_low_raw[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(DAC_current_low_raw[outputChannels]);

  SerialUSB.print(F("Calibration data DAC_current_low_uA: "));
  for (i = 1; i < outputChannels; i++) {
    SerialUSB.print(DAC_current_low_uA[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(DAC_current_low_uA[outputChannels]);
  SerialUSB.println();

  for (i = 1; i <= outputChannels; i++) {
    DAC_current_high_raw[i] = 3600;
    Indio.analogWrite(i, DAC_current_high_raw[i], true);
  }

  for (i = 1; i <= outputChannels; i++) {
    screenDAC_current();
    SerialUSB.print(F("Measure the CURRENT (in uA) on channel "));
    SerialUSB.println(i);
    SerialUSB.print(F("Enter the current (in uA) in the Serial Monitor box above. Received (uA): "));
    DAC_current_high_uA[i] = getSerialInt(0, 30000);
    SerialUSB.println(DAC_current_high_uA[i]);
  }

  SerialUSB.println();
  SerialUSB.print(F("Calibration data DAC_current_high_raw: "));
  for (i = 1; i < outputChannels; i++) {
    SerialUSB.print(DAC_current_high_raw[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(DAC_current_high_raw[outputChannels]);

  SerialUSB.print(F("Calibration data DAC_current_high_uA: "));
  for (i = 1; i < outputChannels; i++) {
    SerialUSB.print(DAC_current_high_uA[i]);
    SerialUSB.print(F(","));
  }
  SerialUSB.println(DAC_current_high_uA[outputChannels]);
  SerialUSB.println();

  screenDone();
  serialMenu();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void screenWelcome() { //this function draws the first menu - splash screen

  lcd.clear();
  lcd.setCursor(15, 1); //set the cursor to the fifth pixel from the left edge, third row.
  lcd.print(F("Welcome to")); //print text on screen
  lcd.setCursor(15, 2); //set the cursor to the fifth pixel from the left edge, third row.
  lcd.print(F("Industruino!")); //print text on screen
  lcd.setCursor(15, 4); //set the cursor to the fifth pixel from the left edge, third row.
  lcd.print(F("Analog I/O")); //print text on screen
  lcd.setCursor(15, 5); //set the cursor to the fifth pixel from the left edge, third row.
  lcd.print(F("calibration:")); //print text on screen
  lcd.setCursor(15, 6); //set the cursor to the fifth pixel from the left edge, third row.
  lcd.print(F("see Serial Monitor")); //print text on screen
  delay(2000);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void screenDone() {

  lcd.clear();
  lcd.setCursor(15, 1); //set the cursor to the fifth pixel from the left edge, third row.
  lcd.print(F("To continue")); //print text on screen
  lcd.setCursor(15, 2); //set the cursor to the fifth pixel from the left edge, third row.
  lcd.print(F("Analog I/O")); //print text on screen
  lcd.setCursor(15, 3); //set the cursor to the fifth pixel from the left edge, third row.
  lcd.print(F("calibration:")); //print text on screen
  lcd.setCursor(15, 5); //set the cursor to the fifth pixel from the left edge, third row.
  lcd.print(F("see Serial Monitor")); //print text on screen
  delay(2000);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void screenADC_voltage_low() {

  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print(F("ANALOG IN 0-10V"));
  lcd.setCursor(5, 3);
  lcd.print(F("connect LOW voltage"));
  lcd.setCursor(5, 4);
  lcd.print(F("input Serial Monitor"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void screenADC_voltage_high() {

  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print(F("ANALOG IN 0-10V"));
  lcd.setCursor(5, 3);
  lcd.print(F("connect HIGH voltage"));
  lcd.setCursor(5, 4);
  lcd.print(F("input Serial Monitor"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void screenADC_current() {

  //  SerialUSB.println("in screen");    for debugging
  //  SerialUSB.println(freeRam());
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print(F("ANALOG IN 4-20mA"));
  lcd.setCursor(5, 3);
  lcd.print(F("connect current"));
  lcd.setCursor(5, 4);
  lcd.print(F("to channels 1-4 "));
  //  lcd.print(i);
  //  lcd.setCursor(5, 6);
  //  lcd.print(F("input Serial Monitor"));
  //  SerialUSB.println(freeRam());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void screenDAC_voltage() {

  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print(F("ANALOG OUT 0-10V"));
  lcd.setCursor(5, 3);
  lcd.print(F("measure mV on"));
  lcd.setCursor(5, 4);
  lcd.print(F("channel "));
  lcd.print(i);
  lcd.setCursor(5, 6);
  lcd.print(F("input Serial Monitor"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void screenDAC_current() {

  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print(F("ANALOG OUT 4-20mA"));
  lcd.setCursor(5, 3);
  lcd.print(F("measure uA on"));
  lcd.setCursor(5, 4);
  lcd.print(F("channel "));
  lcd.print(i);
  lcd.setCursor(5, 6);
  lcd.print(F("input Serial Monitor"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void serialMenu() {

  SerialUSB.println();
  SerialUSB.println(F("================================================================================="));
  SerialUSB.println();
  SerialUSB.println(F("Welcome to Industruino!"));
  SerialUSB.println(F("Analog I/O calibration for IND.I/O"));
  SerialUSB.println();
  SerialUSB.println(F("Main menu"));
  SerialUSB.println(F("1. DAC: analog OUT 0-10V"));
  SerialUSB.println(F("2. DAC: analog OUT 4-20mA"));
  SerialUSB.println(F("3. ADC: analog IN 0-10V"));
  SerialUSB.println(F("4. ADC: analog IN 4-20mA"));

  SerialUSB.println();
  SerialUSB.println(F("Please enter your choice 1,2,3,4 in the Serial input box above and hit return"));
  //  SerialUSB.println(F("Make sure your Serial Monitor is configured for 'NO LINE ENDING'"));
  SerialUSB.println();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int getSerialInt(int min, int max) {
  int number = -1;
  while (number <= min || number >= max) {
    while (SerialUSB.available() == 0) { }
    number = SerialUSB.parseInt();
  }
  return number;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}



