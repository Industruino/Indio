/* Indio Library, by Rustem Iskuzhin for Industruino - wwww.industruino.com
 * This library enables communication with the Industruino IND.I/O Baseboard.
 * The IND.I/O Baseboard has 8CH of 24V digital I/O (6.5-32V supported),
 * 4CH 0-10V/4-20mA ADC, 2CH 0-10V/4-20mA DAC, RS485 transceiver.
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


/*
Analog Calibration method:
 
 Analog IN, 0-10V: In your sketch set the analogReadMode to V10_raw. Feed a known voltage between 0-10V into on of the 4 input channels. Read the corresponding raw ADC value from the serial terminal and write it down. Do this at two voltage levels, for example 2.5V and 7.5V. Enter the recorded raw ADC value and corresponding voltage (in mV) into the calibration array below. Repeat this for all 4 channels.

 Analog IN, 4-20mA: In your sketch set the analogReadMode to mA_raw. Feed a known current between 0-20mA into on of the 4 input channels. Read the corresponding raw ADC value from the serial terminal and write it down. Do this at two current levels, for example 2.5V and 7.5V. Enter the recorded raw ADC value and corresponding voltage (in uA) into the calibration array below. Repeat this for all 4 channels. As a current source you can for example use a 500mA resistor in series with a 0-20V voltage source and multimeter in series.
 
 Analog OUT, 0-10V: In your sketch set the analogWriteMode to V10_raw. Set the analogWrite value to anything between 0-4096. Read the corresponding output voltage on a multimeter and write it down. Do this at two DAC output values, for example 1000 and 3600. Enter the recorded raw DAC value and corresponding voltage (in mV) into the calibration array below. Repeat this for all 2 channels.

 Analog OUT, 0-10V: In your sketch set the analogWriteMode to mA_raw. Set the analogWrite value to anything between 0-4096. Read the corresponding output current on a multimeter (connect multimeter between output and ground, best with a 100-500Ohm resistor in series) and write it down. Do this at two DAC output values, for example 1000 and 3600. Enter the recorded raw DAC value and corresponding current (in uA) into the calibration array below. Repeat this for all 2 channels.

 */


//Calibration data array for ADC, 0-10V mode

const int ADC_voltage_low_raw[5] = {0,384,384,382,386};//raw ADC value for low reference calibration point. Ignore first 0, subsequent is CH1-CH4 from left to right.
const int ADC_voltage_low_mV[5] = {0,1006,1006,1006,1006}; //corresponding mV for low reference calibration point. Ignore first 0, subsequent is CH1-CH4 from left to right.


const int ADC_voltage_high_raw[5] = {0,2939,2946,2952,2946}; //raw ADC value for high reference calibration point. Ignore first 0, subsequent is CH1-CH4 from left to right.
const int ADC_voltage_high_mV[5] = {0,7985,7985,7985,7985}; //corresponding mV for high reference calibration point. Ignore first 0, subsequent is CH1-CH4 from left to right.


//Calibration data array for ADC, 4-20mA mode

const int ADC_current_low_raw[5] = {0,1863,1863,1863,1863}; //raw ADC value for low reference calibration point. Ignore first 0, subsequent is CH1-CH4 from left to right.
const int ADC_current_low_uA[5] = {0,10000,10000,10000,10000}; //corresponding uA for low reference calibration point. Ignore first 0, subsequent is CH1-CH4 from left to right.

const int ADC_current_high_raw[5] = {0,3692,3692,3692,3692}; //raw ADC value for high reference calibration point. Ignore first 0, subsequent is CH1-CH4 from left to right.
const int ADC_current_high_uA[5] = {0,20000,20000,20000,20000}; //corresponding uA for high reference calibration point. Ignore first 0, subsequent is CH1-CH4 from left to right.



//Calibration data array for DAC, 0-10V mode

const int DAC_voltage_low_raw[3] = {0,1000,1000};//raw DAC value for low reference calibration point. Ignore first 0, subsequent is CH1-CH2 from left to right.
const int DAC_voltage_low_mV[3] = {0,2641,2630}; //corresponding mV for low reference calibration point. Ignore first 0, subsequent is CH1-CH2 from left to right.


const int DAC_voltage_high_raw[3] = {0,2500,2500}; //raw DAC value for high reference calibration point. Ignore first 0, subsequent is CH1-CH2 from left to right.
const int DAC_voltage_high_mV[3] = {0,6823,6805}; //corresponding mV for high reference calibration point. Ignore first 0, subsequent is CH1-CH2 from left to right.


//Calibration data array for DAC, 4-20mA mode

const int DAC_current_low_raw[3] = {0,1000,1000}; //raw DAC value for low reference calibration point. Ignore first 0, subsequent is CH1-CH2 from left to right.
const int DAC_current_low_uA[3] = {0,5162,5162}; //corresponding uA for low reference calibration point. Ignore first 0, subsequent is CH1-CH2 from left to right.

const int DAC_current_high_raw[3] = {0,3600,3600}; //raw DAC value for high reference calibration point. Ignore first 0, subsequent is CH1-CH2 from left to right.
const int DAC_current_high_uA[3] = {0,19530,19530}; //corresponding uA for high reference calibration point. Ignore first 0, subsequent is CH1-CH2 from left to right.



#include "Indio.h"
#include <Wire.h>
#include <Arduino.h>

#define BUFFER_SIZE 128
// I2C address for MCP3422 - base address for MCP3424
#define MCP3422_ADDRESS 0X68

// fields in configuration register
#define MCP342X_GAIN_FIELD 0X03 // PGA field
#define MCP342X_GAIN_X1    0X00 // PGA gain X1
#define MCP342X_GAIN_X2    0X01 // PGA gain X2
#define MCP342X_GAIN_X4    0X02 // PGA gain X4
#define MCP342X_GAIN_X8    0X03 // PGA gain X8

#define MCP342X_RES_FIELD  0X0C // resolution/rate field
#define MCP342X_RES_SHIFT  2    // shift to low bits
#define MCP342X_12_BIT     0X00 // 12-bit 240 SPS
#define MCP342X_14_BIT     0X04 // 14-bit 60 SPS
#define MCP342X_16_BIT     0X08 // 16-bit 15 SPS
#define MCP342X_18_BIT     0X0C // 18-bit 3.75 SPS

#define MCP342X_CONTINUOUS 0X10 // 1 = continuous, 0 = one-shot

#define MCP342X_CHAN_FIELD 0X60 // channel field
#define MCP342X_CHANNEL_1  0X00 // select MUX channel 1
#define MCP342X_CHANNEL_2  0X20 // select MUX channel 2
#define MCP342X_CHANNEL_3  0X40 // select MUX channel 3
#define MCP342X_CHANNEL_4  0X60 // select MUX channel 4

#define MCP342X_START      0X80 // write: start a conversion
#define MCP342X_BUSY       0X80 // read: output not ready

#define MCP4726_CMD_WRITEDAC            (0x40)  // Writes data to the DAC
#define MCP4726_CMD_WRITEDACEEPROM      (0x60)  // Writes data to the DAC and the EEPROM 


    
// read mcp342x data
float IndioClass::analogRead(int pin)
{
  float current, voltage;
  long data;
  
    if (pin != previouspin)
    {
        this->mcp342xWrite(adcConfig[pin]);
        delay(sample_rate);
        previouspin = pin;
    }
  // pointer used to form int32 data
  uint8_t *p = (uint8_t *)&data;
  // timeout - not really needed?
  uint32_t start = millis();
  do {
    // assume 18-bit mode
    Wire.requestFrom(MCP3422_ADDRESS, 4);
    if (Wire.available() != 4) {
      Serial.println("read failed");
    }
    for (int8_t i = 2; i >= 0; i--) {
      p[i] = Wire.read();
    }
    // extend sign bits
    p[3] = p[2] & 0X80 ? 0XFF : 0;
    // read config/status byte
    uint8_t s = Wire.read();
    if ((s & MCP342X_RES_FIELD) != MCP342X_18_BIT) {
      // not 18 bits - shift bytes for 12, 14, or 16 bits
      p[0] = p[1];
      p[1] = p[2];
      p[2] = p[3];
    }
	if (mode_ADC[pin]==1)
	{
	current=map(((data/mvDivisor)+2048), ADC_current_low_raw[pin], ADC_current_high_raw[pin], ADC_current_low_uA[pin], ADC_current_high_uA[pin]);
	return current /= 1000;
	}
	if (mode_ADC[pin]==2)
	{
	voltage=map(((data/mvDivisor)+2048), ADC_voltage_low_raw[pin], ADC_voltage_high_raw[pin], ADC_voltage_low_mV[pin], ADC_voltage_high_mV[pin]);
	return voltage /= 1000;
	}
	if (mode_ADC[pin]==3)
	{
	voltage=map(((data/mvDivisor)+2048), ADC_voltage_low_raw[pin], ADC_voltage_high_raw[pin], ADC_voltage_low_mV[pin], ADC_voltage_high_mV[pin]);
	return voltage /= 1000;
	}
	if (mode_ADC[pin]==4)
	{
	current=map(((data/mvDivisor)+2048), ADC_current_low_raw[pin], ADC_current_high_raw[pin], ADC_current_low_uA[pin], ADC_current_high_uA[pin]);
	return ((((current-4000)/16000))*100);
	}
	if (mode_ADC[pin]==5)
	{
	voltage=map(((data/mvDivisor)+2048), ADC_voltage_low_raw[pin], ADC_voltage_high_raw[pin], ADC_voltage_low_mV[pin], ADC_voltage_high_mV[pin]);
	return (((voltage/10000))*100);
	}
	if (mode_ADC[pin]==6)
	{
	voltage=map(((data/mvDivisor)+2048), ADC_voltage_low_raw[pin], ADC_voltage_high_raw[pin], ADC_voltage_low_mV[pin], ADC_voltage_high_mV[pin]);
	return (((voltage/5000))*100);
	}
	if (mode_ADC[pin]==7)
	{
	current=((data/mvDivisor)+2048);
	return current;
	}
    if (mode_ADC[pin]==8)
    {
    voltage=((data/mvDivisor)+2048);
    return voltage;
    }
      
      
    if ((s & MCP342X_BUSY) == 0);
  } while (millis() - start < 500); //allows rollover of millis()
  Serial.println("read timeout"); 
}
//------------------------------------------------------------------------------
// write mcp342x configuration byte
    int IndioClass::mcp342xWrite(int config)
    {
     Wire.beginTransmission(MCP3422_ADDRESS);
     Wire.write(config); 
     Wire.endTransmission();
    }
  
    //------------------------------------------------------------------------------
    void IndioClass::begin(char addr) 
    { 
     _i2caddr = addr;
      Wire.begin();
      
    }
  
    void IndioClass::setAddress(char subAddress)
    {
      Wire.begin();
      ADDRESS         = 0x4 << 3 | subAddress;
      REGISTER_INPUT  = 0x0;
      REGISTER_OUTPUT = 0x2;
      REGISTER_CONFIG = 0x6;
      REGISTER_MASK = 0x4A;
      REGISTER_LATCH = 0x44;
      REGISTER_INTERRUPT_SOURCE = 0x4C;

    //  outputBuffer    = 0;
    //  inputBuffer     = 0;
    //  modeBuffer      = 0;
    }
	
    void IndioClass::setAddress2(char subAddress)
    {
      Wire.begin();
      ADDRESS         = 0x4 << 3 | subAddress;
      REGISTER_INPUT  = 0x0;
      REGISTER_OUTPUT = 0x2;
      REGISTER_CONFIG = 0x6;

	 // outputBuffer2    = 0;
     // inputBuffer2     = 0;
     // modeBuffer2      = 0;
    }
	void IndioClass::analogWrite(int pin, float value, bool writeEEPROM )
    {
      int output;
        
      if(mode_DAC[pin]==1)
	  {
            output = map((value*1000), DAC_current_low_uA[pin], DAC_current_high_uA[pin], DAC_current_low_raw[pin], DAC_current_high_raw[pin]);
	  }
	  if(mode_DAC[pin]==2)
	  {
          output = map((value*1000), DAC_voltage_low_mV[pin], DAC_voltage_high_mV[pin], DAC_voltage_low_raw[pin], DAC_voltage_high_raw[pin]);
	  }
	  if(mode_DAC[pin]==3)
	  {
          output = map((value*1000), DAC_voltage_low_mV[pin], DAC_voltage_high_mV[pin], DAC_voltage_low_raw[pin], DAC_voltage_high_raw[pin]);
	  }
      if(mode_DAC[pin]==4)
      {
          output = map((4+(value*0.16))*1000, DAC_current_low_uA[pin], DAC_current_high_uA[pin], DAC_current_low_raw[pin], DAC_current_high_raw[pin]);
      }
      if(mode_DAC[pin]==5)
      {
          output = map((value*100), DAC_voltage_low_mV[pin], DAC_voltage_high_mV[pin], DAC_voltage_low_raw[pin], DAC_voltage_high_raw[pin]);
      }
      if(mode_DAC[pin]==6)
      {
          output = map((value*1000), DAC_voltage_low_mV[pin], DAC_voltage_high_mV[pin], DAC_voltage_low_raw[pin], DAC_voltage_high_raw[pin]);
      }
      if(mode_DAC[pin]==7)
	  {
          output = value;
	  }
      if(mode_DAC[pin]==8)
      {
          output = value;
      }
        
     
        if (pin == 1)
        {
            begin(0x60);
        }
        if (pin == 2)
        {
            begin(0x61);
        }
        
     //uncomment the following two lines for i2c fast mode. Currently not supported in this way on D21G.
     // uint8_t twbrback = TWBR; 
     // TWBR = 12; // 400 khz
     
      Wire.beginTransmission(_i2caddr);
    if (writeEEPROM)
     {
     Wire.write(MCP4726_CMD_WRITEDACEEPROM);
     }
    else
     {
     Wire.write(MCP4726_CMD_WRITEDAC);
     }
      Wire.write(output / 16);                   // Upper data bits          (D11.D10.D9.D8.D7.D6.D5.D4)
      Wire.write((output % 16) << 4);            // Lower data bits          (D3.D2.D1.D0.x.x.x.x)
      Wire.endTransmission();
     // TWBR = twbrback;
    }

    void IndioClass::setADCResolution(int res)
	{
	INDIO_ADC_RESOLUTION=res;
    
    }

	void IndioClass::analogReadMode(int pin, int mode)
	{
        int res=(INDIO_ADC_RESOLUTION-12)/2;
        int modeSwitchPin;

        switch (res) {
            case 0:
                sample_rate = 5;
                mvDivisor = 1;
                break;
            case 1:
                sample_rate = 17;
                 mvDivisor = 4;
                break;
            case 2:
                sample_rate = 67;
                 mvDivisor = 16;
                break;
            case 3:
                sample_rate = 267;
                 mvDivisor = 64;
                break;
        }

        switch (pin) {
            case 1:
                modeSwitchPin = 11;
                break;
            case 2:
                modeSwitchPin = 10;
                break;
            case 3:
                modeSwitchPin = 13;
                break;
            case 4:
                modeSwitchPin = 12;
                break;
        }

        previouspin = 0;
        
        this->setAddress2(0x20);
        bitClear(modeBuffer2, 10);
        bitClear(modeBuffer2, 11);
        bitClear(modeBuffer2, 12);
        bitClear(modeBuffer2, 13);
        this->flushMode2();
 
   
	 // setup the mode of a pin from PCA9555 call flushMode() or flush() to send it on PCA9555
      if(mode == 1)
      {

        bitWrite(outputBuffer2, modeSwitchPin, HIGH);
		adcConfig[pin]= MCP342X_START | MCP342X_CONTINUOUS | (pin-1) << 5 | res<< 2;
		mode_ADC[pin]=1;
      }
      else if (mode == 2)
      {
         bitWrite(outputBuffer2, modeSwitchPin, LOW);
         adcConfig[pin] = MCP342X_START | MCP342X_CONTINUOUS | (pin-1) << 5 | res<< 2;
		 mode_ADC[pin]=2;
      }
	  else if (mode == 3)
	  {
	     bitWrite(outputBuffer2, modeSwitchPin, LOW);
         adcConfig[pin] = MCP342X_START | MCP342X_CONTINUOUS | (pin-1) << 5 | res<< 2 | 1;
		 mode_ADC[pin]=3;
	  }
	  else if (mode == 4)
	  {
	     bitWrite(outputBuffer2, modeSwitchPin, HIGH);
         adcConfig[pin]= MCP342X_START | MCP342X_CONTINUOUS | (pin-1) << 5 | res<< 2;
		 mode_ADC[pin]=4;
	  }
	  else if (mode == 5)
      {
         bitWrite(outputBuffer2, modeSwitchPin, LOW);
         adcConfig[pin] = MCP342X_START | MCP342X_CONTINUOUS | (pin-1) << 5 | res<< 2;
		 mode_ADC[pin]=5;
      }
	  else if (mode == 6)
	  {
	     bitWrite(outputBuffer2, modeSwitchPin, LOW);
         adcConfig[pin] = MCP342X_START | MCP342X_CONTINUOUS | (pin-1) << 5 | res<< 2 | 1;
		 mode_ADC[pin]=6;
	  }
	  else if (mode == 7)
	  {
	     bitWrite(outputBuffer2, modeSwitchPin, HIGH);
         adcConfig[pin] = MCP342X_START | MCP342X_CONTINUOUS | (pin-1) << 5 | res<< 2;
		 mode_ADC[pin]=7;
	  }
      else if (mode == 8)
	  {
          bitWrite(outputBuffer2, modeSwitchPin, LOW);
          adcConfig[pin] = MCP342X_START | MCP342X_CONTINUOUS | (pin-1) << 5 | res<< 2;
          mode_ADC[pin]=8;
	  }
	  this->flushOutput2();

	}
	
	void IndioClass::analogWriteMode(int pin, int mode)
	{
        int tempstore = outputBuffer2;
        
        this->setAddress2(0x20);
        bitClear(modeBuffer2, 8);
        bitClear(modeBuffer2, 9);
        this->flushMode2();
        
        outputBuffer2 = tempstore;
        this->flushOutput2();

      if(mode == 1)
      {
          bitSet(outputBuffer2, pin+7);
          mode_DAC[pin]=1;
      }
      else if (mode == 2)
      {
         bitClear(outputBuffer2, pin+7);
		 mode_DAC[pin]=2;
      }
	  else if (mode == 3)
	  {
	     bitClear(outputBuffer2, pin+7);
		 mode_DAC[pin]=3;
	  }
	  else if (mode == 4)
	  {
	     bitSet(outputBuffer2, pin+7);
		 mode_DAC[pin]=4;
	  }
	  else if (mode == 5)
	  {
	     bitClear(outputBuffer2, pin+7);
		 mode_DAC[pin]=5;
	  }
	  else if (mode == 6)
	  {
	     bitClear(outputBuffer2, pin+7);
		 mode_DAC[pin]= 6;
	  }
	  else if (mode == 7)
	  {
        bitSet(outputBuffer2, pin+7);
        mode_DAC[pin]=7;
	  }
      else if (mode == 8)
	  {
          bitClear(outputBuffer2, pin+7);
          mode_DAC[pin]=8;
	  }
	  this->flushOutput2();
	  
      /*if (pin == 1)
	  {
	     begin(0x60);
	  }
	  if (pin == 2)
	  {
	     begin(0x61);
	  }
       */
	}
	
    void IndioClass::digitalMode(int pin, int mode)
    {
      this->setAddress(0x21);
	
	if  (startupState == 1)
	{
      Wire.beginTransmission(0x21);
      Wire.write(0x4A);
      Wire.write(0x00);  //  low byte
      Wire.endTransmission();
      Wire.beginTransmission(0x21);
      Wire.write(0x4B);
      Wire.write(0x00);  //  low byte
      Wire.endTransmission();
      startupState = 0;
      }
      
    
      // setup the mode of a pin from PCA9555 call flushMode() or flush() to send it on PCA9555
      if(mode == 1)
      {
        bitClear(modeBuffer, pin*2-1);
        this->flushMode();
       
        bitSet(maskBuffer, pin*2-2);
        this->flushMask();
    	bitClear(latchBuffer, pin*2-2);
        this->flushLatch();
      }
      if(mode == 0)
      {
        bitSet(modeBuffer, pin*2-2);
        this->flushMode();
    	
    	bitClear(maskBuffer, pin*2-2);
        this->flushMask();
    	bitClear(latchBuffer, pin*2-2);
        this->flushLatch();
      }
      if(mode == 11)
      {
      	bitSet(modeBuffer, pin*2-2);
        this->flushMode();
       
        bitSet(maskBuffer, pin*2-2);
        this->flushMask();
      }
      if(mode == 12)
      {
      	bitSet(modeBuffer, pin*2-2);
        this->flushMode();
       
        bitClear(maskBuffer, pin*2-2);
        this->flushMask();
       
        bitSet(latchBuffer, pin*2-2);
        this->flushLatch();
        
      }
      if(mode == 13)
      {
      	bitSet(modeBuffer, pin*2-2);
        this->flushMode();
       
        bitSet(maskBuffer, pin*2-2);
        this->flushMask();
        
    	bitSet(latchBuffer, pin*2-2);
        this->flushLatch();
      }
        
	  
  
        
    }
    void IndioClass::digitalWrite(int pin, int value)
    {
       
        this->setAddress(0x21);


      bitWrite(outputBuffer, pin*2-1, value);
	  this->flushOutput();
    }

    int IndioClass::digitalRead(int pin)
    {
      
        this->setAddress(0x21);
      // reads a bit from inputBuffer
      // reads the value of a pin from PCA9555 it's not the current State. Call flush() or flushInput() before read
      this->flushInput();
      return (bitRead(inputBuffer,pin*2-2));
	  
    }
    
    int IndioClass::interruptSource()
    {
    
      this->setAddress(0x21);
      // reads a bit from inputBuffer
      this->flushInterruptSource();     
      int n = interruptSourceBuffer;
      n = n << 2;  //offset for zero value - interrupt source register empty
	  n = ((Log2n(n) + 1)/2);  
      return n;
    }
    

    void IndioClass::flushMode()
    {
      // send modeBuffer to PCA9555 so you can config the inputs and outputs
      gpio_dir(modeBuffer);
    }
     
     void IndioClass::flushMask()
    {
      // send modeBuffer to PCA9555 so you can config the inputs and outputs
      gpio_mask(maskBuffer);
    }
    
    void IndioClass::flushLatch()
    {
      // send modeBuffer to PCA9555 so you can config the inputs and outputs
      gpio_latch(latchBuffer);
    }

    void IndioClass::flushOutput()
    {
      // send outputBuffer to PCA9555
      gpio_write(outputBuffer);
    }

    void IndioClass::flushInput()
    {
      // receive inputBuffer from PCA9555
      inputBuffer = gpio_read();
    }
    
    void IndioClass::flushInterruptSource()
    {
      // receive inputBuffer from PCA9555
      interruptSourceBuffer = interruptSource_read();
    }
	
    void IndioClass::flushMode2()
    {
      // send modeBuffer to PCA9555 so you can config the inputs and outputs
      gpio_dir(modeBuffer2);
    }

    void IndioClass::flushOutput2()
    {
      // send outputBuffer to PCA9555
      gpio_write(outputBuffer2);
    }

    void IndioClass::flushInput2()
    {
      // receive inputBuffer from PCA9555
      inputBuffer2 = gpio_read();
    }
    void IndioClass::flush()
    {
      // send modeBuffer to PCA9555
      flushMode();
      // send outputBuffer to PCA9555
      flushOutput();
      // receive inputBuffer from PCA9555
      flushInput();
    }
	
	void IndioClass::flush2()
    {
      // send modeBuffer to PCA9555
      flushMode2();
      // send outputBuffer to PCA9555
      flushOutput2();
      // receive inputBuffer from PCA9555
      flushInput2();
    }
    int IndioClass::gpio_read()
    {
      int data = 0;

      //  Send input register address
      Wire.beginTransmission(ADDRESS);
      Wire.write(REGISTER_INPUT);
      Wire.endTransmission();

      //  Connect to device and request two bytes
      Wire.beginTransmission(ADDRESS);
      Wire.requestFrom(ADDRESS, 2);
      if (Wire.available()) {

        data = Wire.read();
      }    
      if (Wire.available()) {
        data = word(Wire.read(),data);
      }
      Wire.endTransmission();
      return data;
    }
    
    
     int IndioClass::interruptSource_read()
    {
      int data = 0;

      //  Send input register address
      Wire.beginTransmission(ADDRESS);
      Wire.write(REGISTER_INTERRUPT_SOURCE);
      Wire.endTransmission();

      //  Connect to device and request two bytes
      Wire.beginTransmission(ADDRESS);
      Wire.requestFrom(ADDRESS, 2);
      if (Wire.available()) {

        data = Wire.read();
      }    
      if (Wire.available()) {
        data = word(Wire.read(),data);
      }
      Wire.endTransmission();
      return data;
    }

    void IndioClass::gpio_write(int data)
    {
      //  Send output register address
      Wire.beginTransmission(ADDRESS);
      Wire.write(REGISTER_OUTPUT);

      //  Connect to device and send two bytes
      Wire.write(0xff & data);  //  low byte
      Wire.write(data >> 8);    //  high byte

      Wire.endTransmission();
    }

    void IndioClass::gpio_dir(int dir)
    {
      //  Send config register address
      Wire.beginTransmission(ADDRESS);
      Wire.write(REGISTER_CONFIG);

      //  Connect to device and send two bytes
      Wire.write(0xff & dir);  //  low byte
      Wire.write(dir >> 8);    //  high byte

      Wire.endTransmission();
    }
     void IndioClass::gpio_mask(int mask)
    {
      //  Send config register address
      Wire.beginTransmission(ADDRESS);
      Wire.write(REGISTER_MASK);

      //  Connect to device and send two bytes
      Wire.write(0xff & mask);  //  low byte
      Wire.write(mask >> 8);    //  high byte

      Wire.endTransmission();
    }
     void IndioClass::gpio_latch(int latch)
    {
      //  Send config register address
      Wire.beginTransmission(ADDRESS);
      Wire.write(REGISTER_LATCH);

      //  Connect to device and send two bytes
      Wire.write(0xff & latch);  //  low byte
      Wire.write(latch >> 8);    //  high byte

      Wire.endTransmission();
    }
    
    unsigned int IndioClass::Log2n(unsigned int n)
	{
    return (n > 1)? 1 + Log2n(n/2): 0;
	}


IndioClass Indio;
