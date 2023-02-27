#include <Arduino.h>


//เวลา
int seconds = 0;
int minutes = 0;
int hours =   0;
int days = 1;
bool timerOn = LOW;
unsigned long previousMillis = 0;
const long interval = 1000;


//ui
int V = 0;


//mode
int mode = 0;
int preset = 0;
int save = 0;


//accessories
int SoilSensorValue;
int SoilHumidity;

//pinmode
int modeBotton = 26;
int presetBotton = 28;
int saveBotton = 30
int SoilSensorPin = A8;


//DHT22
#include "DHT.h"
#define DHTPIN 24
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

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
 pinMode(presetBotton,INPUT);
 pinMode(saveBotton,INPUT);
 
 lcd.begin(16, 2);
 lcd.setCursor(1,1);        
 lcd.print("^");  

}
 
void loop(){
    
    timeSytem();
    lcdBotton();

    dht22();
    SoilSensor();


    ModeSytem();


    //ต้องย้ายไปใน void ใหม่
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



 



}







void timeSytem(){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            if (timerOn) {
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

    lcd_key = read_LCD_buttons();      
    switch (lcd_key)  
            {
            case btnRIGHT:
                {
                        V = (V + 1) % 3;            
                        if (V == 0){
                        lcd.setCursor(1,1);      
                        lcd.print("^");          


                        lcd.setCursor(4,1);      
                        lcd.print(" ");          


                        lcd.setCursor(7,1);      
                        lcd.print(" ");          
                        }


                        else if(V==1){
                        lcd.setCursor(1,1);      
                        lcd.print(" ");          


                        lcd.setCursor(4,1);      
                        lcd.print("^");          


                        lcd.setCursor(7,1);      
                        lcd.print(" ");          
                        }


                        else {
                        lcd.setCursor(1,1);      
                        lcd.print(" ");          


                        lcd.setCursor(4,1);      
                        lcd.print(" ");          


                        lcd.setCursor(7,1);      
                        lcd.print("^");          
                        }


                        delay(300);
                    break;
                }




            case btnLEFT:  
                {
                    V--;                                                               
                    if (V < 0)   { V = 2 ; }          

                        if (V == 0){
                        lcd.setCursor(1,1);              
                        lcd.print("^");


                        lcd.setCursor(4,1);
                        lcd.print(" ");


                        lcd.setCursor(7,1);
                        lcd.print(" ");
                        }


                        else if(V==1){
                        lcd.setCursor(1,1);
                        lcd.print(" ");


                        lcd.setCursor(4,1);
                        lcd.print("^");


                        lcd.setCursor(7,1);
                        lcd.print(" ");
                        }


                        else {
                        lcd.setCursor(1,1);
                        lcd.print(" ");


                        lcd.setCursor(4,1);
                        lcd.print(" ");


                        lcd.setCursor(7,1);
                        lcd.print("^");
                        }


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


                    else {
                                seconds = (seconds+1)%60;                          
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


                else {
                            seconds--;                          
                            if (seconds <= 0)   { seconds = 59; }
                        }
                delay(300);

                break;      
            }




            case btnSELECT:          
                {
                    timerOn = HIGH;
                    delay(300);      
                    break;                  
                }






        }

}




void dht22(){
  delay(200);
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




void SoilSensor(){
    SoilSensorValue = analogRead(SoilSensorPin);
    SoilHumidity = map(SoilSensorValue,1023,0,0,100);
    Serial.print("Soil Humidity : ");
    Serial.print(SoilHumidity);
    Serial.println(" %");
    delay(200);
}




void ModeSytem(){
    switch (expression)
    {
    case /* constant-expression */:
        /* code */
        break;
    
    default:
        break;
    }
        

        


}