/*
 * Indio - Interface with i2c PCA9555 MCP3424 MCP4725 Modbus-master/slave
 *
 * Copyright (c) 2014 Rustem Iskuzhin
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
 
#define mA 1
#define V10 2
#define V5 3
#define mA_p 4
#define V10_p 5
#define V5_p 6
#define mA_raw 7
#define V10_raw 8


class IndioClass
{
  private:
    int ADDRESS;
    int REGISTER_INPUT;
    int REGISTER_OUTPUT;
    int REGISTER_CONFIG;
    int outputBuffer;
    int inputBuffer;
    int modeBuffer;
	int outputBuffer2;
    int inputBuffer2;
    int modeBuffer2;
    int previouspin;
    int sample_rate;
    int previous_sample;
    double mvDivisor;

  public:
    
    
    // default adc congifuration register - resolution and gain added in setup()
    int adcConfig[5];
    // divisor to convert ADC reading to milivolts
    
	int mode_ADC[5];
	int mode_DAC[3];
    int _i2caddr;
    int INDIO_ADC_RESOLUTION;

    
 
// read mcp342x data
    float analogRead(int pin);
//------------------------------------------------------------------------------
// write mcp342x configuration byte
    int mcp342xWrite(int config);
      //------------------------------------------------------------------------------
    void begin(char addr); 
      
    void setAddress(char subAddress);
   
    void setAddress2(char subAddress);
   
	void analogWrite(int pin, float value, bool writeEEPROM);
	
    void setADCResolution(int res);
	
	void analogReadMode(int pin, int mode);
		
	void analogWriteMode(int pin, int mode);
	
	void digitalMode(int pin, int mode);
   
    void digitalWrite(int pin, int value);

    int digitalRead(int pin);
    
    void flushMode();

    void flushOutput();
    
    void flushInput();
   
    void flush();
	
	void flushMode2();

    void flushOutput2();
    
    void flushInput2();
   
    void flush2();
    
    int gpio_read();
    
    void gpio_write(int data);
    
    void gpio_dir(int dir);

  
};

extern IndioClass Indio;
