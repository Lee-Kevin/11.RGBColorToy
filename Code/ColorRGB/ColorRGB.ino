/*
 * ColorRGB.ino
 * This is a demo to show you the color RGB
 *
 * Copyright (c) 2016 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Jiankai.li
 * Create Time: Feb 2016
 * 
 * The MIT License (MIT)
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

#include <ChainableLED.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <I2C_LCD.h>

#define Red       0
#define Green     1
#define Blue      2

#define NUM_LEDS  3

#define LedPin    4
#define LedNumbers 20
#define AverageTimes 2
#define LCDUpdateTime 800  // unit ms

enum Status 
{
    Standby = 0,
    Update  = 1,
    Other   = 2,
};
typedef enum Status SystemStatus;
SystemStatus WorkingStatus;

ChainableLED leds(2, 3, NUM_LEDS);//defines the pin used on arduino.

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LedNumbers, LedPin, NEO_GRB + NEO_KHZ800);

I2C_LCD LCD;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.
unsigned long StartTime = 0;
uint16_t RGBColor[3];

void setup()
{
    Serial.begin(9600);
    Serial.flush();
    strip.begin();
    strip.show();
    Wire.begin();
    LCD.CleanAll(WHITE);    //Clean the screen with black or white.
    delay(1000);            //Delay for 1s.
    
    //8*16 font size, auto new line, black character on white back ground.
    LCD.FontModeConf(Font_8x16_1, FM_ANL_AAA, BLACK_BAC); 
    
    LCD.CharGotoXY(0,0);

    //Print string on I2C_LCD.
    
    LCD.println("RGB Color Model");

    LCD.FontModeConf(Font_6x12, FM_ANL_AAA, BLACK_BAC); 
    LCD.CharGotoXY(0,20);
    LCD.print("Red  :   ");
    LCD.CharGotoXY(0,35);
    LCD.print("Green:   ");
    LCD.CharGotoXY(0,50);
    LCD.print("Blue :   ");
    
    WorkingStatus = Standby;
    StartTime = millis();

}

void loop()
{
//  uint8_t track_data[5];
    switch (WorkingStatus) {
    case Standby:
        // for(int i=0;i<AverageTimes;i++) {
            // RGBColor[Red]   += analogRead(A0)>>2;
            // RGBColor[Green] += analogRead(A1)>>2;
            // RGBColor[Blue]  += analogRead(A2)>>2;
        // }
        // RGBColor[Red]   /= AverageTimes;
        // RGBColor[Green] /= AverageTimes;
        // RGBColor[Blue]  /= AverageTimes;
        RGBColor[Red]   = analogRead(A0)>>2;
        RGBColor[Green] = analogRead(A1)>>2;
        RGBColor[Blue]  = analogRead(A2)>>2;
        
        leds.setColorRGB(Red, RGBColor[Red], 0, 0);  
        leds.setColorRGB(Green, 0,RGBColor[Green], 0);  
        leds.setColorRGB(Blue, 0,0,RGBColor[Blue]); 
        for (int i=0;i<LedNumbers;i++) {
            strip.setPixelColor(i,RGBColor[Red],RGBColor[Green],RGBColor[Blue]);
            strip.show();
            delay(1);
        }
        break;
    case Update:
        LCDDisplay();
        WorkingStatus = Standby;
        StartTime = millis();
        break;
    default:
        break;
    }
    if((millis()-StartTime) > LCDUpdateTime) {
        
        WorkingStatus = Update;
    }

    // Serial.println(RGBColor[Red]);
    // Serial.println(RGBColor[Green]);
    // Serial.println(RGBColor[Blue]);
}

void LCDDisplay(void) 
{

    LCD.CharGotoXY(54,20);
    if(RGBColor[Red]<10) {
        LCD.print("00");
        LCD.println(RGBColor[Red],DEC);
    } else if(RGBColor[Red]<100) {
        LCD.print("0");
        LCD.println(RGBColor[Red],DEC);
    } else {
        LCD.println(RGBColor[Red],DEC);
    }

    LCD.CharGotoXY(54,35);
    if(RGBColor[Green]<10) {
        LCD.print("00");
        LCD.println(RGBColor[Green],DEC);
    } else if(RGBColor[Green]<100) {
        LCD.print("0");
        LCD.println(RGBColor[Green],DEC);
    } else {
        LCD.println(RGBColor[Green],DEC);
    }

    LCD.CharGotoXY(54,50);
    if(RGBColor[Blue]<10) {
        LCD.print("00");
        LCD.println(RGBColor[Blue],DEC);
    } else if(RGBColor[Blue]<100) {
        LCD.print("0");
        LCD.println(RGBColor[Blue],DEC);
    } else {
        LCD.println(RGBColor[Blue],DEC);
    }
}


