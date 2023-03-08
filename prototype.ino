#include <Arduino.h>


//เวลา
int seconds = 0;
int minutes = 0;
int hours =   0;
int days = 1;
bool timerRun = HIGH;
unsigned long previousMillis = 0;
const long interval = 1000;

//all about mode
//อยู่ในโหมดไหม
bool modeOn = LOW;
//โหมดที่เท่าไร
int mode = 1;
//ปุ่มกดใน modeOn
int modeBotton = 26;
//ลูกศรปรับค่าในmode
int V_mode = 0;


//all about time
int V = 0;
bool timeOn = LOW;
int timeBotton = 28;


//SoilSensor
int SoilSensorValue;
int SoilHumidity;
unsigned long previousMillis_SoilSensor = 0;
const long interval_SoilSensor = 500;
int SoilSensorPin = A8;





//DHT22
#include "DHT.h"
#define DHTPIN 24
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
unsigned long previousMillis_dht22 = 0;
const long interval_dht22 = 500;

//จอ
#include <LiquidCrystal.h>


const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);


int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
 

int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      
 if (adc_key_in > 1500) return btnNONE;  
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP;    
 if (adc_key_in < 380)  return btnDOWN;  
 if (adc_key_in < 500)  return btnLEFT;  
 if (adc_key_in < 800)  return btnSELECT;
 return btnNONE;  
}




void setup()
{
 Serial.begin(9600);
 dht.begin();

 pinMode(modeBotton,INPUT);
 
 
 lcd.begin(16, 2);
  

}
 
void loop(){
    
    timeSystem();
    lcdBotton();
    Mode_LCD_Botton();

    dht22();
    SoilSensor();


   
    //DisplaySystem();





 



}







void timeSystem(){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            if (timerRun) {
                seconds++;
                
                if (seconds == 60) {
                    minutes++;
                    seconds = 0;
                }
                if (minutes == 60) {
                    hours++;
                    minutes = 0;
                }
                if (hours == 24){
                    days++;
                    hours =0;
                }
                if (days == 7){
                    days = 1;
                }
    
            }
        }


}




void lcdBotton(){


    if (timeOn == HIGH)
    {
    lcd_key = read_LCD_buttons();      
    switch (lcd_key)  
            {
            case btnRIGHT:
                {
                        V = (V + 1) % 4;            
                        delay(300);
                    break;
                }




            case btnLEFT:  
                {
                    V--;                                                               
                    if (V < 0)   { V = 3 ; }          
                    delay(300);
                    break;
                    
                }




            case btnUP:      
                {
                    if (V == 0){
                            hours = (hours+1)%24;                          
                        
                        }


                    else if(V==1){
                                minutes = (minutes+1)%60;

                            }

                    else if(V==2){
                                seconds = (seconds+1)%60;

                            }

                    else {
                                days++;
                                if (days>7)
                                {
                                    days=1;
                                }
                                                        
                            }
                    delay(300);

                    break;
                
                }




            case btnDOWN:            
            {
                if (V == 0){
                            hours--;                          
                            if (hours <= 0)   { hours = 23; }      

                        }


                else if(V==1){
                            minutes--;                          
                            if (minutes <= 0)   { minutes = 59; }
                        }

                else if(V==2){
                            seconds--;                          
                            if (seconds <= 0)   { seconds = 59; }
                        }

                else {
                            days--;
                                if (days<1)
                                {
                                    days=7;
                                }
                        }
                delay(300);

                break;      
            }




            case btnSELECT:          
                {
                    timerRun = HIGH;
                    delay(300);      
                    break;                  
                }






        }

                lcd.clear();
                lcd.setCursor(0, 0);
                //ต้องมาเพิ่มวัน
                if (hours < 10) {
                        lcd.print("0");
                    }
                lcd.print(hours);

                lcd.print(":");

                if (minutes < 10) {
                        lcd.print("0");
                    }
                lcd.print(minutes);

                lcd.print(":");

                if (seconds < 10){
                        lcd.print("0");
                    }
                lcd.print(seconds);

                lcd.print("  ");
                if (days == 1){      
                        lcd.print("Mon");                   
                        }
                        else if(days==2){
                        lcd.print("Tue");                
                        }
                        else if(days==3){
                        lcd.print("Wen");                
                        }
                        else if(days==4){
                        lcd.print("Thu");                
                        }
                        else if(days==5){
                        lcd.print("Fri");                
                        }
                        else if(days==6){
                        lcd.print("Sat");                
                        }
                        else {
                        lcd.print("Sun");      
                        }

                if (V == 0){
                        lcd.setCursor(1,1);      
                        lcd.print("^");                   
                        }
                        else if(V==1){
                        lcd.setCursor(4,1);      
                        lcd.print("^");                  
                        }
                        else if(V==2){
                        lcd.setCursor(7,1);      
                        lcd.print("^");                  
                        }
                        else {
                        lcd.setCursor(12,1);      
                        lcd.print("^");          
                        }



    }




}





void Mode_LCD_Botton(){
    if (modeOn == HIGH)
    {
        lcd_key = read_LCD_buttons();      
        switch (lcd_key)  
            {
            case btnRIGHT:
                {
                    V_mode = (V_mode + 1) % 3;
                    
                    delay(300);
                    break;
                }




            case btnLEFT:  
                {
                    
                    delay(300);
                    break;
                    
                }




            case btnUP:      
                {
                    if (V_mode == 0){
                        mode ++;
                        if (mode > 3) {
                            mode = 1;
                        }                           
                        
                        }


                    else if(V_mode==1){
                               

                            }


                    else {
                                                          
                            }
                    delay(300);
                    break;
                
                }




            case btnDOWN:            
            {
                
                delay(300);
                break;      
            }




            case btnSELECT:          
                {
                    
                    delay(300);      
                    break;                  
                }


        }
    lcd.clear();
    lcd.setCursor(0,0);              
    lcd.print("Mode");
    lcd.print(":");
    lcd.print(mode);
    lcd.print(" ");
    lcd.print("time");
    lcd.print(":");

            if(V_mode == 0){
                        lcd.setCursor(5,1);              
                        lcd.print("^");
                    }   
                    else if(V_mode == 1){
                        lcd.setCursor(12,1);
                        lcd.print("^");
                    }
                    else if(V_mode == 2){
                        lcd.setCursor(15,1);
                        lcd.print("^");
                    }

    
    }
    
    


}








void dht22(){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis_dht22 >= interval_dht22) {
            previousMillis_dht22 = currentMillis;
            //delay(200);
            // Reading temperature or humidity takes about 250 milliseconds!
            // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
            float h = dht.readHumidity();
            // Read temperature as Celsius (the default)
            float t = dht.readTemperature();
            // Read temperature as Fahrenheit (isFahrenheit = true)
            float f = dht.readTemperature(true);


            // Check if any reads failed and exit early (to try again).
            if (isnan(h)  ,isnan(t) , isnan(f)) {
                        Serial.println(F("Failed to read from DHT sensor!"));
                        return;
                    }


            // Compute heat index in Fahrenheit (the default)
            float hif = dht.computeHeatIndex(f, h);
            // Compute heat index in Celsius (isFahreheit = false)
            float hic = dht.computeHeatIndex(t, h, false);


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


            
           

    }
    

}




void SoilSensor(){
    unsigned long currentMillis = millis();
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




void DisplaySystem(){
    lcd.clear();
    lcd.setCursor(0,0);              
    lcd.print("Hu:");
    lcd.print(h);
    lcd.print("% Temp:");
    lcd.print(t);
    lcd.print("°C");
    lcd.setCursor(0,1);
    lcd.print("Soil Hu:");
    lcd.print(SoilHumidity);
    lcd.print("%");
    
   


}