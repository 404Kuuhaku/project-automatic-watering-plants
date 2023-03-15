#include <Arduino.h>

// เวลา
int seconds = 0;
int minutes = 0;
int hours = 0;
int days = 1;
bool timerRun = LOW;
unsigned long currentMillis;
unsigned long previousMillis = 0;
const long interval = 1000;

// all about mode
// ปุ่มกดให้ modeOn
int modeBotton = 26;
int modeBottonState = 0;
int LastmodeBottonState = 0;
int select_Mode = 0;

// โหมดที่เท่าไร
int mode = 0;

// mode0 ตั้งเวลานาฬิกา
bool timeOn = LOW;
int V_mode0 = 0;

// mode1 รดน้ำต้นไม้แบบตั้งเวลา
// ลูกศรปรับค่าในmode1
bool modeOn1 = LOW;
int V_mode1 = 0;

int seconds_1setup = 0;
int minutes_1setup = 0;
int hours_1setup = 7;

int seconds_2setup = 0;
int minutes_2setup = 0;
int hours_2setup = 12;

int seconds_3setup = 0;
int minutes_3setup = 0;
int hours_3setup = 17;

int seconds_time_setup = 0;
int minutes_time_setup = 10;
int hours_time_setup = 0;

int pump_mode1 = 0;
int fog_mode1 = 0;

// mode2
bool modeOn2 = LOW;
int V_mode2 = 0;
int SoilHumidity_1setup = 55;
int AirHumidity_1setup = 55;
int pump_mode2 = 0;
int fog_mode2 = 0;

// mode3
bool modeOn3 = LOW;
int V_mode3 = 0;
int pump_mode3 = 0;
int fog_mode3 = 0;

// mode4
bool modeOn4 = LOW;
int Humidity_Mode = 0;

// mode 5
bool modeOn5 = LOW;

// relay
int Relay1 = 31;
int Relay2 = 33;
bool Relay1_State = HIGH;
bool Relay2_State = HIGH;

// SoilSensor
int SoilSensorValue;
int SoilHumidity;
unsigned long previousMillis_SoilSensor = 0;
const long interval_SoilSensor = 500;
int SoilSensorPin = A8;

// DHT22
#include "DHT.h"
#define DHTPIN 22
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
unsigned long previousMillis_dht22 = 0;
const long interval_dht22 = 500;
float h;
int no_decimal_h;

float t;
int no_decimal_t;

float f;
int no_decimal_f;

float hif;
int no_decimal_hif;

float hic;
int no_decimal_hic;


// จอ
#include <LiquidCrystal.h>

const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

int read_LCD_buttons()
{
    adc_key_in = analogRead(0);
    if (adc_key_in > 1500)
        return btnNONE;
    if (adc_key_in < 50)
        return btnRIGHT;
    if (adc_key_in < 195)
        return btnUP;
    if (adc_key_in < 380)
        return btnDOWN;
    if (adc_key_in < 500)
        return btnLEFT;
    if (adc_key_in < 800)
        return btnSELECT;
    return btnNONE;
}

byte customChar1[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};



void setup()
{
    Serial.begin(9600);
    dht.begin();

    pinMode(modeBotton, INPUT);
    
    pinMode(Relay1, OUTPUT);
    pinMode(Relay2, OUTPUT);
    
    lcd.begin(16, 2);
    lcd.createChar(1, customChar1);
}

void loop()
{

    timeSystem();
    Mode_Botton_System();
    Time_LCD_Botton();
    Mode1_LCD_Botton();
    Mode2_LCD_Botton();
    Mode3_LCD_Botton();
    Mode4_LCD_Botton();
    Mode5_LCD_Botton();

    dht22();
    SoilSensor();

    relayControl();
}

void timeSystem()
{
    if (timerRun == HIGH)
    {
        currentMillis = millis();
        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;

            seconds++;

            if (seconds == 60)
            {
                minutes++;
                seconds = 0;
            }
            if (minutes == 60)
            {
                hours++;
                minutes = 0;
            }
            if (hours == 24)
            {
                days++;
                hours = 0;
            }
            if (days == 7)
            {
                days = 1;
            }
        }
    }
}

void Mode_Botton_System()
{

    modeBottonState = digitalRead(modeBotton);
    if (modeBottonState != LastmodeBottonState && modeBottonState == HIGH)
    {
        lcd.clear();
        mode = (mode + 1) % 6;
    }
    LastmodeBottonState = modeBottonState;

    if (mode == 0)
    {
        timeOn = HIGH;
        modeOn1 = LOW;
        modeOn2 = LOW;
        modeOn3 = LOW;
        modeOn4 = LOW;
        modeOn5 = LOW;
    }
    else if (mode == 1)
    {
        timeOn = LOW;
        modeOn1 = HIGH;
        modeOn2 = LOW;
        modeOn3 = LOW;
        modeOn4 = LOW;
        modeOn5 = LOW;
    }
    else if (mode == 2)
    {
        timeOn = LOW;
        modeOn1 = LOW;
        modeOn2 = HIGH;
        modeOn3 = LOW;
        modeOn4 = LOW;
        modeOn5 = LOW;
    }
    else if (mode == 3)
    {
        timeOn = LOW;
        modeOn1 = LOW;
        modeOn2 = LOW;
        modeOn3 = HIGH;
        modeOn4 = LOW;
        modeOn5 = LOW;
    }
    else if(mode == 4)
    {
        timeOn = LOW;
        modeOn1 = LOW;
        modeOn2 = LOW;
        modeOn3 = LOW;
        modeOn4 = HIGH;
        modeOn5 = LOW;
    }
    else
    {
        timeOn = LOW;
        modeOn1 = LOW;
        modeOn2 = LOW;
        modeOn3 = LOW;
        modeOn4 = LOW;
        modeOn5 = HIGH;
    }

    
    
}

void Time_LCD_Botton()
{

    if (timeOn == HIGH)
    {
        lcd_key = read_LCD_buttons();
        switch (lcd_key)
        {
        case btnRIGHT:
        {
            V_mode0 = (V_mode0 + 1) % 4;

            

            delay(300);
            break;
        }

        case btnLEFT:
        {
            V_mode0--;
            if (V_mode0 < 0)
            {
                V_mode0 = 3;
            }

            

            delay(300);
            break;
        }

        case btnUP:
        {
            if (V_mode0 == 0)
            {
                hours = (hours + 1) % 24;
            }

            else if (V_mode0 == 1)
            {
                minutes = (minutes + 1) % 60;
            }

            else if (V_mode0 == 2)
            {
                seconds = (seconds + 1) % 60;
            }

            else
            {
                days++;
                if (days > 7)
                {
                    days = 1;
                }
            }
            delay(300);

            break;
        }

        case btnDOWN:
        {
            if (V_mode0 == 0)
            {
                hours--;
                if (hours < 0)
                {
                    hours = 23;
                }
            }

            else if (V_mode0 == 1)
            {
                minutes--;
                if (minutes < 0)
                {
                    minutes = 59;
                }
            }

            else if (V_mode0 == 2)
            {
                seconds--;
                if (seconds < 0)
                {
                    seconds = 59;
                }
            }

            else
            {
                days--;
                if (days < 1)
                {
                    days = 7;
                }
            }
            delay(300);

            break;
        }

        case btnSELECT:
        {
            timerRun = !timerRun;
            delay(300);
            break;
        }
        }

        lcd.setCursor(0, 0);
        if (hours < 10)
        {
            lcd.print("0");
        }
        lcd.print(hours);

        lcd.print(":");

        if (minutes < 10)
        {
            lcd.print("0");
        }
        lcd.print(minutes);

        lcd.print(":");

        if (seconds < 10)
        {
            lcd.print("0");
        }
        lcd.print(seconds);

        lcd.print("  ");
        if (days == 1)
        {
            lcd.print("Mon");
        }
        else if (days == 2)
        {
            lcd.print("Tue");
        }
        else if (days == 3)
        {
            lcd.print("Wen");
        }
        else if (days == 4)
        {
            lcd.print("Thu");
        }
        else if (days == 5)
        {
            lcd.print("Fri");
        }
        else if (days == 6)
        {
            lcd.print("Sat");
        }
        else
        {
            lcd.print("Sun");
        }


        if (V_mode0 == 0)
            {
                lcd.setCursor(1, 1);
                lcd.print("^");

                lcd.setCursor(4, 1);
                lcd.print(" ");

                lcd.setCursor(7, 1);
                lcd.print(" ");

                lcd.setCursor(12, 1);
                lcd.print(" ");
            }
            else if (V_mode0 == 1)
            {
                lcd.setCursor(1, 1);
                lcd.print(" ");

                lcd.setCursor(4, 1);
                lcd.print("^");

                lcd.setCursor(7, 1);
                lcd.print(" ");

                lcd.setCursor(12, 1);
                lcd.print(" ");
            }
            else if (V_mode0 == 2)
            {
                lcd.setCursor(1, 1);
                lcd.print(" ");

                lcd.setCursor(4, 1);
                lcd.print(" ");

                lcd.setCursor(7, 1);
                lcd.print("^");

                lcd.setCursor(12, 1);
                lcd.print(" ");
            }
            else
            {
                lcd.setCursor(1, 1);
                lcd.print(" ");

                lcd.setCursor(4, 1);
                lcd.print(" ");

                lcd.setCursor(7, 1);
                lcd.print(" ");

                lcd.setCursor(12, 1);
                lcd.print("^");
            }

    }
}

void Mode1_LCD_Botton()
{
    if (modeOn1 == HIGH)
    {
        lcd_key = read_LCD_buttons();
        switch (lcd_key)
        {
        case btnRIGHT:
        {
            V_mode1 = (V_mode1 + 1) % 14;

            

            delay(300);
            break;
        }

        case btnLEFT:
        {
            V_mode1--;
            if (V_mode1 < 0)
            {
                V_mode1 = 13;
            }

            

            delay(300);
            break;
        }

        case btnUP:
        {
            if (V_mode1 == 0)
            {
                hours_1setup = (hours_1setup + 1) % 24;
            }

            else if (V_mode1 == 1)
            {
                minutes_1setup = (minutes_1setup + 1) % 60;
            }

            else if (V_mode1 == 2)
            {
                seconds_1setup = (seconds_1setup + 1) % 60;
            }

            else if (V_mode1 == 3)
            {
                hours_2setup = (hours_2setup + 1) % 24;
            }

            else if (V_mode1 == 4)
            {
                minutes_2setup = (minutes_2setup + 1) % 60;
            }

            else if (V_mode1 == 5)
            {
                seconds_2setup = (seconds_2setup + 1) % 60;
            }

            else if (V_mode1 == 6)
            {
                hours_3setup = (hours_3setup + 1) % 24;
            }

            else if (V_mode1 == 7)
            {
                minutes_3setup = (minutes_3setup + 1) % 60;
            }

            else if (V_mode1 == 8)
            {
                seconds_3setup = (seconds_3setup + 1) % 60;
            }

            else if (V_mode1 == 9)
            {
                hours_time_setup = (hours_time_setup + 1) % 3;
            }

            else if (V_mode1 == 10)
            {
                minutes_time_setup = (minutes_time_setup + 1) % 60;
            }

            else if (V_mode1 == 11)
            {
                seconds_time_setup = (seconds_time_setup + 1) % 60;
            }

            else if (V_mode1 == 12)
            {
                pump_mode1 = (pump_mode1 +1)%2;
            }

            else if (V_mode1 == 13)
            {
                fog_mode1 = (fog_mode1 +1)%2;
            }

            else
            {
            }
            delay(300);
            break;
        }

        case btnDOWN:
        {

            if (V_mode1 == 0)
            {
                hours_1setup--;
                if (hours_1setup < 0)
                {
                    hours_1setup = 23;
                }
            }

            else if (V_mode1 == 1)
            {
                minutes_1setup--;
                if (minutes_1setup < 0)
                {
                    minutes_1setup = 59;
                }
            }

            else if (V_mode1 == 2)
            {
                seconds_1setup--;
                if (seconds_1setup < 0)
                {
                    seconds_1setup = 59;
                }
            }

            else if (V_mode1 == 3)
            {
                hours_2setup--;
                if (hours_2setup < 0)
                {
                    hours_2setup = 23;
                }
            }

            else if (V_mode1 == 4)
            {
                minutes_2setup--;
                if (minutes_2setup < 0)
                {
                    minutes_2setup = 59;
                }
            }

            else if (V_mode1 == 5)
            {
                seconds_2setup--;
                if (seconds_2setup < 0)
                {
                    seconds_2setup = 59;
                }
            }

            else if (V_mode1 == 6)
            {
                hours_3setup--;
                if (hours_3setup < 0)
                {
                    hours_3setup = 23;
                }
            }

            else if (V_mode1 == 7)
            {
                minutes_3setup--;
                if (minutes_3setup < 0)
                {
                    minutes_3setup = 59;
                }
            }

            else if (V_mode1 == 8)
            {
                seconds_3setup--;
                if (seconds_3setup < 0)
                {
                    seconds_3setup = 59;
                }
            }

            else if (V_mode1 == 9)
            {
                hours_time_setup--;
                if (hours_time_setup < 0)
                {
                    hours_time_setup = 2;
                }
            }

            else if (V_mode1 == 10)
            {
                minutes_time_setup--;
                if (minutes_time_setup < 0)
                {
                    minutes_time_setup = 59;
                }
            }

            else if (V_mode1 == 11)
            {
                seconds_time_setup--;
                if (seconds_time_setup < 0)
                {
                    seconds_time_setup = 59;
                }
            }

            else if (V_mode1 == 12)
            {
                pump_mode1 --;
                if (pump_mode1 < 0)
                {
                    pump_mode1 = 1;
                }
                
            }

            else if (V_mode1 == 13)
            {
                
                fog_mode1 --;
                if (fog_mode1 < 0)
                {
                    fog_mode1 = 1;
                }
            }

            else
            {
            }
            delay(300);
            break;
        }

        case btnSELECT:
        {
            select_Mode = 1;
            mode = 5;
            delay(300);
            break;
        }
        }


        lcd.setCursor(0, 1);
        lcd.print("Mode1");

        if (V_mode1 == 0 || V_mode1 == 3 || V_mode1 == 6 || V_mode1 == 9)
            {
                lcd.setCursor(9, 1);
                lcd.print("^");

                lcd.setCursor(12, 1);
                lcd.print(" ");

                lcd.setCursor(15, 1);
                lcd.print(" ");

                lcd.setCursor(6, 1);
                lcd.print(" ");

                lcd.setCursor(14, 1);
                lcd.print(" ");
            }
            else if (V_mode1 == 1 || V_mode1 == 4 || V_mode1 == 7 || V_mode1 == 10)
            {
                lcd.setCursor(9, 1);
                lcd.print(" ");

                lcd.setCursor(12, 1);
                lcd.print("^");

                lcd.setCursor(15, 1);
                lcd.print(" ");

                lcd.setCursor(6, 1);
                lcd.print(" ");

                lcd.setCursor(14, 1);
                lcd.print(" ");
            }
            else if (V_mode1 == 2 || V_mode1 == 5 || V_mode1 == 8 || V_mode1 == 11)
            {
                lcd.setCursor(9, 1);
                lcd.print(" ");

                lcd.setCursor(12, 1);
                lcd.print(" ");

                lcd.setCursor(15, 1);
                lcd.print("^");

                lcd.setCursor(6, 1);
                lcd.print(" ");

                lcd.setCursor(14, 1);
                lcd.print(" ");
            }
            else if (V_mode1 == 12)
            {
                lcd.setCursor(9, 1);
                lcd.print(" ");

                lcd.setCursor(12, 1);
                lcd.print(" ");

                lcd.setCursor(15, 1);
                lcd.print(" ");

                lcd.setCursor(6, 1);
                lcd.print("^");

                lcd.setCursor(14, 1);
                lcd.print(" ");
            }
            else if (V_mode1 == 13)
            {
                lcd.setCursor(9, 1);
                lcd.print(" ");

                lcd.setCursor(12, 1);
                lcd.print(" ");

                lcd.setCursor(15, 1);
                lcd.print(" ");

                lcd.setCursor(6, 1);
                lcd.print(" ");

                lcd.setCursor(14, 1);
                lcd.print("^");
            }



        if (V_mode1 <= 2)
        {
            lcd.setCursor(0, 0);
            lcd.print("1stTime ");
            if (hours_1setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(hours_1setup);

            lcd.print(":");

            if (minutes_1setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(minutes_1setup);

            lcd.print(":");

            if (seconds_1setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(seconds_1setup);
        }

        if (V_mode1 > 2 && V_mode1 <= 5)
        {
            lcd.setCursor(0, 0);
            lcd.print("2ndTime ");
            if (hours_2setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(hours_2setup);

            lcd.print(":");

            if (minutes_2setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(minutes_2setup);

            lcd.print(":");

            if (seconds_2setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(seconds_2setup);
        }

        if (V_mode1 > 5 && V_mode1 <= 8)
        {
            lcd.setCursor(0, 0);
            lcd.print("3rdTime ");
            if (hours_3setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(hours_3setup);

            lcd.print(":");

            if (minutes_3setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(minutes_3setup);

            lcd.print(":");

            if (seconds_3setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(seconds_3setup);
        }

        if (V_mode1 > 8 && V_mode1 <= 11)
        {
            lcd.setCursor(0, 0);
            lcd.print("TimeSet ");
            if (hours_time_setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(hours_time_setup);

            lcd.print(":");

            if (minutes_time_setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(minutes_time_setup);

            lcd.print(":");

            if (seconds_time_setup < 10)
            {
                lcd.print("0");
            }
            lcd.print(seconds_time_setup);
        }

        if (V_mode1 > 11 && V_mode1 <=13)
        {
            lcd.setCursor(0, 0);
            lcd.print("Pump:");
            if (pump_mode1 == 0)
            {
                lcd.print("Off");
            }
            else if (pump_mode1 ==1)
            {
                lcd.print("On ");
            }

            lcd.print(" ");

            lcd.print("Fog:");
            if (fog_mode1 == 0)
            {
                lcd.print("Off");
            }
            else if (fog_mode1 ==1)
            {
                lcd.print("On ");
            }
            
        }
        
    }
}




void Mode2_LCD_Botton()
{

    if (modeOn2 == HIGH)
    {
              lcd_key = read_LCD_buttons();
              switch (lcd_key)
              {
                    case btnRIGHT:
                    {
                        V_mode2 = (V_mode2 +1)%4;
                        delay(300);
                        break;
                    }
            
                    case btnLEFT:
                    {
                       V_mode2--;
                       if (V_mode2 <0)
                       {
                        V_mode2 = 3;
                       }
                       
                       delay(300);
                        break;
                    }
            
                    case btnUP:
                    {
                        if (V_mode2 == 0)
                        {
                            pump_mode2 = (pump_mode2+1) % 2;
                        }
                        else if (V_mode2 == 1)
                        {
                            fog_mode2 = (fog_mode2+1) % 2;
                        }
                        else if (V_mode2 == 2)
                        {
                            SoilHumidity_1setup = (SoilHumidity_1setup + 1)%100;
                        }
                        else if (V_mode2 == 3)
                        {
                            AirHumidity_1setup = (AirHumidity_1setup + 1)%100;
                        }
                        
                        
                        delay(300);
            
                        break;
                    }
            
                    case btnDOWN:
                    {
                        if (V_mode2 == 0)
                        {
                            pump_mode2 = (pump_mode2+1) % 2;
                        }
                        else if (V_mode2 == 1)
                        {
                            fog_mode2 = (fog_mode2+1) % 2;
                        }
                        else if (V_mode2 == 2)
                        {
                                SoilHumidity_1setup -- ;
                                    if(SoilHumidity_1setup < 0){
                                    SoilHumidity_1setup = 99 ;
                                    }
                        }
                        else if (V_mode2 == 3)
                        {
                                AirHumidity_1setup -- ;
                                    if(AirHumidity_1setup < 0){
                                    AirHumidity_1setup = 99 ;
                                    }
                        }

                        
                        delay(300);
            
                        break;
                    }
            
                    case btnSELECT:
                    {
                      select_Mode = 2;
                        mode = 5;
                       delay(300);
            
                        break;
                    }
              }


            lcd.setCursor(0, 1);
            lcd.print("Mode2");

            if (V_mode2 == 0)
            {
                lcd.setCursor(13, 1);
                lcd.print(" ");

                lcd.setCursor(12, 1);
                lcd.print(" ");


                lcd.setCursor(6, 1);
                lcd.print("^");

                lcd.setCursor(14, 1);
                lcd.print(" ");
            }
            
            else if (V_mode2 == 1)
            {
                lcd.setCursor(13, 1);
                lcd.print(" ");

                lcd.setCursor(12, 1);
                lcd.print(" ");


                lcd.setCursor(6, 1);
                lcd.print(" ");

                lcd.setCursor(14, 1);
                lcd.print("^");
            }

            else if (V_mode2 == 2)
            {
                lcd.setCursor(13, 1);
                lcd.print("^");

                lcd.setCursor(12, 1);
                lcd.print(" ");


                lcd.setCursor(6, 1);
                lcd.print(" ");

                lcd.setCursor(14, 1);
                lcd.print(" ");
            }

            else if (V_mode2 == 3)
            {
                lcd.setCursor(13, 1);
                lcd.print(" ");

                lcd.setCursor(12, 1);
                lcd.print("^");


                lcd.setCursor(6, 1);
                lcd.print(" ");

                lcd.setCursor(14, 1);
                lcd.print(" ");
            }

            if (V_mode2 <= 1)
            {
                    lcd.setCursor(0, 0);
                    lcd.print("Pump:");
                    if (pump_mode2 == 0)
                    {
                        lcd.print("Off");
                    }
                    else if (pump_mode2 ==1)
                    {
                        lcd.print("On ");
                    }

                    lcd.print(" ");

                    lcd.print("Fog:");
                    if (fog_mode2 == 0)
                    {
                        lcd.print("Off");
                    }
                    else if (fog_mode2 ==1)
                    {
                        lcd.print("On ");
                    }
            }
            else if (V_mode2 == 2)
            {
                    lcd.setCursor(0, 0);
                    lcd.print("Soil Hu Set:");
                    if (SoilHumidity_1setup < 10)
                    {
                        lcd.print("0");
                    }
                    lcd.print(SoilHumidity_1setup);
                    lcd.print("% ");
            }
            else if (V_mode2 == 3)
            {
                lcd.setCursor(0, 0);
                lcd.print("Air Hu Set:");
                if (AirHumidity_1setup<10)
                {
                    lcd.print("0");
                }
                lcd.print(AirHumidity_1setup);
                lcd.print("%  ");
            }
            
              
              
              

        
    }
}


void Mode3_LCD_Botton(){
  if (modeOn3 == HIGH)
    {
        lcd_key = read_LCD_buttons();
        switch (lcd_key)
        {
              case btnRIGHT:
              {
                  //delay(300);
                  break;
              }
      
              case btnLEFT:
              {
                  //delay(300);
                  break;
              }
      
              case btnUP:
              {
                  
                  
                  V_mode3 = (V_mode3+1)%5;

                  delay(300);
      
                  break;
              }
      
              case btnDOWN:
              {
                  
                  
                    V_mode3--;
                  if (V_mode3 <0)
                  {
                    V_mode3 = 4;
                  }
      
                  delay(300);
                  break;
              }
      
              case btnSELECT:
              {
                  select_Mode = 3;
                  mode = 5;
                  
                    
                  delay(300);
                  break;
              }
        }

        lcd.setCursor(0, 1);
        lcd.print("Mode3");
        lcd.setCursor(9, 1);
        lcd.print("^^^");

        lcd.setCursor(0, 0);
        lcd.print("Preset:");
        if (V_mode3 == 0)
        {
            lcd.print("Orchid   ");
        }
        else if (V_mode3 == 1)
        {
            lcd.print("Vegetable");
        }
        else if (V_mode3 == 2)
        {
            lcd.print("Flower   ");
        }
        else if (V_mode3 == 3)
        {
            lcd.print("Cactus   ");
        }
        else if (V_mode3 == 4)
        {
            lcd.print("Air Clean");
        }
        



    }
}





void Mode4_LCD_Botton()
{
if (modeOn4 == HIGH)
    {   
        lcd_key = read_LCD_buttons();
              switch (lcd_key)
              {
                    case btnRIGHT:
                    {
                        
                        Humidity_Mode = (Humidity_Mode + 1)%2;
                        delay(300);
                        break;
                    }
            
                    case btnLEFT:
                    {
                        
                         Humidity_Mode = (Humidity_Mode + 1)%2;
                       delay(300);
                        break;
                    }
            
                    case btnUP:
                    {
                        
                         Humidity_Mode = (Humidity_Mode + 1)%2;
                        delay(300);
                        break;
                    }
            
                    case btnDOWN:
                    {
                        
                         Humidity_Mode = (Humidity_Mode + 1)%2;
                        delay(300);
                        break;
                    }
            
                    case btnSELECT:
                    {
                        break;
                    }
              }
      
  
        lcd.setCursor(0,0);
        lcd.print(">");
        
        if(Humidity_Mode == 0) {
        lcd.print(" Soil Hu:");
        if (SoilHumidity < 10)
            {
                lcd.print("0");
            }
        lcd.print(SoilHumidity);
        lcd.print("%");
        lcd.print("  <");
        
        }

        if(Humidity_Mode == 1) {
        lcd.print("  Air Hu:");
        if (no_decimal_h < 10)
            {
                lcd.print("0");
            }
        lcd.print(no_decimal_h);
        lcd.print("% ");
        lcd.print(" <");
        
        }
        
        
        
        
        

        lcd.setCursor(0,1);
        lcd.print("   Temp:");
        //lcd.print(t);
        lcd.print(no_decimal_t);
        if (no_decimal_t < 10)
            {
                lcd.print("0");
            }
        lcd.write((byte)1);
        lcd.print("C");

    }
    }



void Mode5_LCD_Botton(){
    if(modeOn5 == HIGH){
    lcd_key = read_LCD_buttons();
              switch (lcd_key)
              {
                    case btnRIGHT:
                    {
                        select_Mode = (select_Mode +1)%4;
                        delay(300);
                        break;
                    }
            
                    case btnLEFT:
                    {
                       select_Mode --;
                       if (select_Mode < 0)
                       {
                        select_Mode = 3;
                       }
                       delay(300);
                        break;
                    }
            
                    case btnUP:
                    {
                        select_Mode = (select_Mode +1)%4;
                        delay(300);
                        break;
                    }
            
                    case btnDOWN:
                    {
                        select_Mode --;
                       if (select_Mode < 0)
                       {
                        select_Mode = 3;
                       }
                        delay(300);
                        break;
                    }
            
                    case btnSELECT:
                    {
                        
                        //delay(300);
                        break;
                    }
              }

    
    lcd.setCursor(0,0);
    lcd.print("You Use Mode:");
        if (select_Mode == 0)
        {
            lcd.print("-  ");
            lcd.setCursor(0,1);
            lcd.print("Manual          ");
        }
        else if (select_Mode == 1)
        {
            lcd.print("1  ");
            lcd.setCursor(0,1);
            lcd.print("Timer           ");
        }
        else if (select_Mode == 2)
        {
            lcd.print("2  ");
            lcd.setCursor(0,1);
            lcd.print("Humidity        ");
        }
        else if (select_Mode == 3)
        {
            lcd.print("3  ");
            lcd.setCursor(0, 1);
                lcd.print("Preset:");
                if (V_mode3 == 0)
                {
                    lcd.print("Orchid   ");
                }
                else if (V_mode3 == 1)
                {
                    lcd.print("Vegetable");
                }
                else if (V_mode3 == 2)
                {
                    lcd.print("Flower   ");
                }
                else if (V_mode3 == 3)
                {
                    lcd.print("Cactus   ");
                }
                else if (V_mode3 == 4)
                {
                    lcd.print("Air Clean");
                }
        }
    
    
    
    }
    
    
}





void dht22(){
    if (timerRun==HIGH){
            if (currentMillis - previousMillis_dht22 >= interval_dht22) {
                    previousMillis_dht22 = currentMillis;
                    //delay(200);
                    // Reading temperature or humidity takes about 250 milliseconds!
                    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
                    h = dht.readHumidity();
                    // Read temperature as Celsius (the default)
                    t = dht.readTemperature();
                    // Read temperature as Fahrenheit (isFahrenheit = true)
                    f = dht.readTemperature(true);


                    // Check if any reads failed and exit early (to try again).
                    if (isnan(h)  ,isnan(t) , isnan(f)) {
                                Serial.println(F("Failed to read from DHT sensor!"));
                                return;
                            }


                    // Compute heat index in Fahrenheit (the default)
                    hif = dht.computeHeatIndex(f, h);
                    // Compute heat index in Celsius (isFahreheit = false)
                    hic = dht.computeHeatIndex(t, h, false);


                    Serial.print(F("Humidity: "));
                    Serial.print(h);
                    Serial.print(F("%  Temperature: "));
                    Serial.print(t);
                    Serial.print(F("°C "));
                    Serial.print(f);
                    Serial.print(F("°F  Heat index: "));
                    Serial.print(hic);
                    Serial.print(F("°C "));
                    Serial.print(hif);
                    Serial.println(F("°F"));

                    no_decimal_h = h;
                    no_decimal_t = t;
                    no_decimal_f = f;


                    
                 

            }
    }

}




void SoilSensor(){
    if(timerRun==HIGH){
            if (currentMillis - previousMillis_SoilSensor >= interval_SoilSensor) {
                    previousMillis_SoilSensor = currentMillis;
            SoilSensorValue = analogRead(SoilSensorPin);
            SoilHumidity = map(SoilSensorValue,1023,0,0,100);
            Serial.print("Soil Humidity : ");
            Serial.print(SoilHumidity);
            Serial.println(" %");
            //delay(200);

            }
    }
}



void relayControl()
{
    digitalWrite(Relay1,HIGH);
    digitalWrite(Relay2,HIGH);
    Serial.println(Relay1_State);
    if (timerRun == HIGH && select_Mode == 1)
    {
        if (seconds == seconds_1setup && minutes == minutes_1setup && hours == hours_1setup)
        {
            Relay1_State = LOW;
            Relay2_State = LOW;
        }
        else if (seconds == (seconds_1setup + seconds_time_setup) && minutes == (minutes_1setup + minutes_time_setup) && hours == (hours_1setup + hours_time_setup))
        {
            Relay1_State = HIGH;
            Relay2_State = HIGH;
        }

        if (seconds == seconds_2setup && minutes == minutes_2setup && hours == hours_2setup)
        {
            Relay1_State = LOW;
            Relay2_State = LOW;
        }
        else if (seconds == (seconds_2setup + seconds_time_setup) && minutes == (minutes_2setup + minutes_time_setup) && hours == (hours_2setup + hours_time_setup))
        {
            Relay1_State = HIGH;
            Relay2_State = HIGH;
        }

        if (seconds == seconds_3setup && minutes == minutes_3setup && hours == hours_3setup)
        {
            Relay1_State = LOW;
            Relay2_State = LOW;
        }
        else if (seconds == (seconds_3setup + seconds_time_setup) && minutes == (minutes_3setup + minutes_time_setup) && hours == (hours_3setup + hours_time_setup))
        {
            Relay1_State = HIGH;
            Relay2_State = HIGH;
        }

        if (pump_mode1 == 1)
        {
            digitalWrite(Relay1, Relay1_State);
        }
        else{
            digitalWrite(Relay1, HIGH);
        }

        if (fog_mode1 == 1)
        {
            digitalWrite(Relay2, Relay2_State);
        }
        else{
            digitalWrite(Relay2, HIGH);
        }
        
    
    }


    if (timerRun == HIGH && select_Mode == 2){
        if (SoilHumidity <= SoilHumidity_1setup)
        {
            Relay1_State = LOW;
        }
        else
        {
            Relay1_State = HIGH;
        }

        if (no_decimal_h <= AirHumidity_1setup)
        {
            Relay2_State = LOW;
        }
        else
        {
            Relay2_State = HIGH;
        }

        if (pump_mode2 == 1)
        {
            digitalWrite(Relay1, Relay1_State);
        }
        else{
            digitalWrite(Relay1, HIGH);
        }

        if (fog_mode2 == 1)
        {
            digitalWrite(Relay2, Relay2_State);
        }
        else{
            digitalWrite(Relay2, HIGH);
        }

        
    }


    if (timerRun == HIGH && select_Mode == 3){
        if (V_mode3 ==0)
        {
            if (SoilHumidity <40)
            {
                digitalWrite(Relay1, LOW);
            }
            if (no_decimal_h <70)
            {
                digitalWrite(Relay2, LOW);
            }
        }

        else if (V_mode3 ==1)
        {
            if (SoilHumidity <40)
            {
                digitalWrite(Relay1, LOW);
            }
            if (no_decimal_h <40)
            {
                digitalWrite(Relay2, LOW);
            }
        }

        else if (V_mode3 ==2)
        {
            if (SoilHumidity <50)
            {
                digitalWrite(Relay1, LOW);
            }
            if (no_decimal_h <50)
            {
                digitalWrite(Relay2, LOW);
            }
        }

        else if (V_mode3 ==3)
        {
            if (SoilHumidity <20)
            {
                digitalWrite(Relay1, LOW);
            }
            if (no_decimal_h <10)
            {
                digitalWrite(Relay2, LOW);
            }
        }

        else if (V_mode3 ==4)
        {
            if (SoilHumidity <25)
            {
                digitalWrite(Relay1, LOW);
            }
            if (no_decimal_h <20)
            {
                digitalWrite(Relay2, LOW);
            }
        }
        

        
    }

}
