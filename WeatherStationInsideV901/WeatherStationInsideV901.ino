#include <Manchester.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

#include "DHT.h"

#define DHTPIN 8     

#define DHTTYPE DHT11   

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

DHT dht(DHTPIN, DHTTYPE);

#include <SoftwareSerial.h>
const int TxPin = 2;
SoftwareSerial lcd = SoftwareSerial(255, TxPin);

int yLightning = 0;
boolean hasntDisplayed = false;

float hi = 0;
int yGust = 0;

long OPFrames = 0;
float stormPressure = 0.0;

boolean dayLoop = false;
long forcastUpdateTime = 0;
long gustResetTime = 530000;
long dataUpdate;
long pTime;


int strikeCount;

boolean gotFirstRain = false;
float yRain = 0.0;

int seconds = 0;
int hour = 0;
int minute = 0;
long timeUpdate = 0;
int hourReset = false;

long pageTime;

boolean butReleased;

int digitPlace = 1;

int pageNum = 1;

float rainRate = 0.0;
float rainCount = 0.0;

float oldPressure = 0.00;
float newPressure = 0.00;
long pressure = 0;
boolean canSound = true;
long canSoundThresh = 0;
float HT = 0;
float LT = 0;

boolean clockSet = false;

int timeUntilGustReset;
int readings = 0;

boolean HL = false;

boolean readingWind = false;
boolean readingTemp = false;
int hoursPassed = 0;
boolean fullReadComplete = false;


int gust = 0;
int wind = 0;
String forecast = "Undecided";
boolean updateForecast = false;
long i;

int tempCal = 29;
float oldoldPressure;
float rainAmount = 0.0;

int temp = 0;
 
 
 float DP = 0.0;

#define RX_PIN 3 
uint8_t moo = 1;

long updateClock = 0;

///////////////Pressure Sensor/////////////////////////

long pFrames = 0;
boolean soundAlert = false;
int isROF = 0;

boolean pStartup = false;
long transmitSerialDataTime = 0;
float h = 0;

int Wadd = B11101110;
int Radd = B11101111;

long alertTime = 0;

float newTemp = 0.0;

float pressureCal = 0.66;

boolean startingUp = true;

boolean updatePress = false;
short temperature;



boolean AMPM = true; //true if AM

boolean canProgress = true;


void setup()
{
 Serial.begin(9600); 

  lcd.begin(19200);
  delay(100);
   pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
  delay(1000);
lcd.write(209);
lcd.write(223);
lcd.write(17);
lcd.print("Checking");
lcd.write(148);
lcd.print("BMP180");
delay(300);
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    lcd.print("BMP180 ERROR");
    while(1);
  }
  delay(1000);
  lcd.write(12);
  sensor_t sensor;
  bmp.getSensor(&sensor);
  lcd.print("BMP180");
  lcd.write(148);
  lcd.print("Initaliazed");
  delay(1000);
  lcd.write(12);
  lcd.print("Checking DHT11");
  delay(300);
  
  dht.begin();
  
  delay(1000);
  
   float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    lcd.write(12);
    lcd.print("DHT11 ERROR");
    return;
  }
  
  delay(1000);
  lcd.write(12);
  lcd.print("DHT11");
  lcd.write(148);
  lcd.print("Initalized");
  delay(500);
  //pinMode(11, OUTPUT);
  
  int serialForecast;
  
  pinMode(4, INPUT);           // set pin to input
digitalWrite(4, HIGH);       // turn on pullup resistors

pinMode(7, INPUT);
digitalWrite(7, HIGH);
  
   
  
  setupCharacters();
  lcd.write(12);
  lcd.write(17);
  lcd.write(223);
lcd.print("Weather Base");
lcd.write(148);
lcd.print("Version: 9.0.1"); //change log at bottom
delay(2000);
lcd.write(12);



//Read initalizing data;
  
  startingUp = false;
  

delay(100);
lcd.write(12);
lcd.print("Connecting to");
lcd.write(148);
lcd.print("Outside Base...");
  man.setupReceive(RX_PIN, MAN_1200);
  man.beginReceive();
}
void loop() {
  
  ReceiveData();
  
 if(digitalRead(4) == 0 && butReleased == true && canProgress){pageNum++; butReleased = false; soundAlert = false; delay(50);}
 if(digitalRead(4) == 1 && butReleased == false && canProgress){butReleased = true;delay(50);}
 if(digitalRead(7) == 0 && clockSet == true){minute++; delay(200);}
 
 if(analogRead(A2) >= 500){
   
   strikeCount++; 
 lcd.write(227);
 soundAlert = true;
 delay(500);
 
 }
 
 if(pageNum == 9){
   
   if(fullReadComplete){
    
    lcd.write(12);
    lcd.print("Lightning");
    lcd.write(148);
    lcd.print("Count:");
    lcd.print(strikeCount);
   fullReadComplete = false; 
   }
     
   }
   
  
  if(pageNum == 10 && canProgress)pageNum = 1;
  
  i++;
  
  rainRate = rainCount * 120;
  
  if(i >= 530000){
    rainCount = 0.0;  
    i = 0;
  }
  
  
  
  if(millis() >= alertTime + 60000){soundAlert = false; alertTime = millis(); if(pageNum == 0){pageNum = 1;}}
  
  if(soundAlert == true && fullReadComplete){
   
    
    if(!AMPM && hour < 10){
    
        pageNum = 0;
        lcd.write(12);
        lcd.print("Storm");
        lcd.write(148);
        lcd.print("Warning");
        lcd.write(225);
        man.transmit(70000);
    
    }
    
    if(AMPM && hour > 7){
    
        pageNum = 0;
        lcd.write(12);
        lcd.print("Storm");
        lcd.write(148);
        lcd.print("Storm Warning");
        lcd.write(225);
        man.transmit(70000);
      
    }
    
    fullReadComplete = false;
      
}
  
  if(readings >= 13){readings = 0;}
  
  
 if(millis() >= updateClock + 1003){
   //delayMicroseconds(100);
   updateClock = millis();
   seconds++;
   if(seconds == 60){minute++; seconds = 0;}
   if(minute >= 60){minute = 0; hour++; hoursPassed++; dayLoop = true;}
   if(hour >= 13){
   updateForecast = true;
   hour = 1;
   AMPM = !AMPM;
   if(clockSet)hourReset = true;
 }
 
 if(pageNum == 8){
    if(fullReadComplete){
      
     
      lcd.write(12);
      lcd.print("Humidity:");
      lcd.print(int(h));
      lcd.print("%");
      lcd.write(148);
      lcd.print("Dew Point:");
      lcd.print(DP);
      
    fullReadComplete = false;
    }
  }
 
  if(pageNum == 7){
    if(fullReadComplete){
    
      lcd.write(12);
      lcd.print("YR:");
      lcd.print(yRain);
      lcd.print("YL:");
      lcd.print(yLightning);
      lcd.write(148);
      lcd.print("YHRW:");
      lcd.print(yGust);
    fullReadComplete = false;
    }
  }
   
   /////Clock Setup///////////
 if(pageNum == 6){
   if(clockSet == false){
       lcd.write(12);
  canProgress = false;
  if(digitalRead(4) == 0){digitPlace++;}
  if(digitPlace == 4){digitPlace = 0; clockSet = true; canProgress = true;}
  if(digitalRead(7) == 0 && digitPlace == 1){hour++;}
  if(digitalRead(7) == 0 && digitPlace == 2){minute++;}
  lcd.print("Set Time:");
  lcd.write(148);
  lcd.print(hour);
  lcd.print(":");
  if(minute < 10){lcd.print("0");}
  lcd.print(minute);
  lcd.print(":");
  if(seconds < 10){lcd.print("0");}
  lcd.print(seconds);
  if(AMPM)lcd.print(" AM");
  if(!AMPM)lcd.print(" PM");
   }
   if(clockSet){
  lcd.write(12);
  lcd.print(hour);
  lcd.print(":");
  if(minute < 10){lcd.print("0");}
  lcd.print(minute);
  lcd.print(":");
  if(seconds < 10){lcd.print("0");}
  lcd.print(seconds);
  if(AMPM)lcd.print(" AM");
  if(!AMPM)lcd.print(" PM");
   }
 }
 }
 
  
 if(dayLoop){
   dayLoop = false;
 if(AMPM && hour == 1){ //This is done everyday
  
  HT = 0;
  LT = 0;
  HL = false;
  yRain = rainAmount;
  yLightning = strikeCount;
  rainAmount = 0.0;
  yGust = gust;
  gust = 0; 
  strikeCount = 0;
  
 }
 }
    
     ///////////////////pressure///////////////////////////////////
  
  //pressure = ReadUncompPress();
 if(millis() >= dataUpdate + 3000){
   
   sensors_event_t event;
  bmp.getEvent(&event);
  
      h = dht.readHumidity();
      float f = dht.readTemperature(true);
      float c = dht.readTemperature();
      hi = dht.computeHeatIndex(f, h);
      DP = c - ((100 - h)/5);              //calculate dew point and convert to farenheit
      DP = (DP * 9.0 / 5.0) + 32.0;
  
  temp = dht.readTemperature(true);
  
  if (event.pressure)
  {
    
    pressure = event.pressure;
    newPressure = (pressure * 0.0295299830714) + pressureCal;
    
    bmp.getTemperature(&newTemp);  

 }
  dataUpdate = millis();
 }
 
if(millis() >= OPFrames + 120000){
 
 if(stormPressure < .02){
  soundAlert = true;  
 }
 OPFrames = millis(); 
 stormPressure = newPressure;
}
  
 
   
  
  if(!pStartup){oldoldPressure = newPressure; stormPressure = newPressure; pStartup = true;}
  
  if(hour == 1 && updateForecast){
   
    h = dht.readHumidity();
    
    if(newPressure > oldoldPressure){
      forecast = "Improving WX"; 
      oldoldPressure = newPressure;
    }
      
    if(newPressure == oldoldPressure || newPressure == oldoldPressure - .01 || newPressure == oldoldPressure - .02 || newPressure == oldoldPressure - .03){
      oldoldPressure = newPressure;
      forecast = "Same As Today";
    }
      
    if(newPressure == oldoldPressure - .04){
      if(temp <= DP + 40){
      forecast = "sunny w/ cloud";
      oldoldPressure = newPressure;
      }
      
        
    }
    
    if(newPressure == oldoldPressure - .05 || newPressure == oldoldPressure - .06){    //30.05 < 
    
    if(temp <= DP + 25){                   //50 temp + 10  60 temp < 65
      forecast = "Cloudy w/ Drizz";
      oldoldPressure = newPressure;
    }
    else
    {
     forecast = "Cloudy";
     oldoldPressure = newPressure; 
    }
    }
    
    if(newPressure == oldoldPressure - .07 || newPressure == oldoldPressure - .09 || newPressure == oldoldPressure - .08){
     
      if(temp <= DP + 30){
      forecast = "Rain";
      oldoldPressure = newPressure;
    }
    else
    {
     forecast = "Cloudy w/ Drizz";
     oldoldPressure = newPressure; 
    }
    }
    
    if(newPressure <= oldoldPressure - .1){
        if(temp <= DP + 20){
      forecast = "Possible Storms";
      oldoldPressure = newPressure;
    }
    else
    {
     
     forecast = "Rain";
     oldoldPressure = newPressure; 
      
    }
    }
    
    updateForecast = false;
   
  }


  
  if(newPressure > oldPressure && millis() >= pFrames + 10800000){isROF = 2; pFrames = millis(); oldPressure = newPressure;}
  if(newPressure < oldPressure && millis() >= pFrames + 10800000){isROF = 1; pFrames = millis(); oldPressure = newPressure;}
  if(newPressure == oldPressure && millis() >= pFrames + 10800000){isROF = 0; pFrames = millis(); oldPressure = newPressure;}
  updatePress = false;
  hoursPassed = 0;
  

  
  
  
  
   if(pageNum == 5){
    if(fullReadComplete){
    lcd.write(12);
    lcd.print("24 Hour Weather");
    lcd.write(148);
    lcd.print(forecast);
    fullReadComplete = false;
    }
  }
     
  
  
     if(pageNum == 4){
  if(fullReadComplete){
    lcd.write(12);
  lcd.print("Pressure:");
  lcd.print(newPressure);
  lcd.print(" ");
  if(isROF == 2)lcd.write(2);
    if(isROF == 0)lcd.print("-");
      if(isROF == 1)lcd.write(1);
      
  lcd.write(148);
  lcd.print("In Temp:");
  newTemp = (newTemp * 9.0 / 5.0) + 32.0;
  lcd.print(newTemp);
  fullReadComplete = false;
    }
  }

  
  //////////////////////pressure finish/////////////////////////////////
    
    
  
  
  
  
  
  
  if(wind < gust-10){
    timeUntilGustReset++;
  }
  else
  {
    timeUntilGustReset = 0;
  }
  
  if(timeUntilGustReset >= gustResetTime)gust = 0;

  
  
  
  
  
  
  if(pageNum == 3){
    if(fullReadComplete){
   lcd.write(12);
   lcd.print("High Temp:");
   lcd.print(HT);
   lcd.print(" ");
   lcd.write(148);
   lcd.print("Low Temp:");
   lcd.print(LT);
    readings++;
    fullReadComplete = false;
  }
  
  }
  
  if(pageNum == 2){
    if(fullReadComplete){

   lcd.write(12);
   lcd.print("Rain Acc:");
   lcd.print(rainAmount);
   lcd.write(148);
   lcd.print("HRW:");
   lcd.print(gust);
   lcd.print(" RR:");
   lcd.print(rainRate);
   readings++;
    fullReadComplete = false;
  }
  }
  
  //read Temp from DHT11//////
  
 
     if(temp > 0 && HL == false){HT = temp; LT = temp; HL = true; lcd.write(12);}
     
     if(HL == true){
      
      if(temp < LT)LT = temp;
      if(temp > HT)HT = temp;
      
     }
  
  //clear screen and write new data if all dat is read
  
  if(pageNum == 1){
    if(fullReadComplete){
  lcd.write(12);
  lcd.print("Temp:");
  lcd.print(temp);
  lcd.print(" FL:");
  lcd.print(hi);
  lcd.write(148);
  lcd.print("Wind:");
  lcd.print(wind);
  }
  
  readings++;
    fullReadComplete = false;
    }
    
    
    if(millis() >= transmitSerialDataTime + 3000){
    //Serial.print(temp + "," + HT + "," + LT + "," + wind + "," + gust + "," + h + "," + hi + "," + newPressure + "," + isROF + "," + newTemp);
    Serial.print(temp); //out temp
    Serial.print(",");
    Serial.print(HT); // high temp
    Serial.print(",");
    Serial.print(LT); // low temp
    Serial.print(",");
    Serial.print(wind); // wind speed
    Serial.print(",");
    Serial.print(h); // humidity
    Serial.print(",");
    Serial.print(hi); // heat index
    Serial.print(",");
    Serial.print(newPressure); // pressure
    Serial.print(",");
    Serial.print(isROF); // rising or falling pressure
    Serial.print(",");
    Serial.print((newTemp * 9.0 / 5.0) + 32.0); // inside temp
    Serial.print(",");
    Serial.print(rainAmount); // rain accumulation
    Serial.print(",");
    Serial.print(rainRate); // rain rate
    Serial.print(",");
    Serial.print(DP); // dewpoint
    Serial.print(",");
    Serial.print(forecast); //forecast
    Serial.print(",");
    Serial.print(soundAlert); //Storm alert?
    Serial.print(",");
    Serial.print(gust); //highest recorded wind speed
    Serial.print(",");
    Serial.println(strikeCount); //number of lightning 
    
    transmitSerialDataTime = millis();
    }
    
}

void ReceiveData(){
  
   if (man.receiveComplete()) {
    uint16_t data = man.getMessage();
    man.beginReceive(); //start listening for next message right
                        //after you retrieve the message
                        
                     
   if(readingTemp){
     
     float dummytemp = data + tempCal;
     readingTemp = false;
     
   }  

  
  if(readingWind){
    
    if(data <= 200){
     wind = data; //convert wind to mph which is in presumably FPS
    } 
    if(wind <= 150 && wind > gust && wind < gust + 5){gust = wind; lcd.write(225); /*man.transmit(60000);*/}
     readingWind = false;
     
     if(readings < 10){
     readings++;
     }
     
     fullReadComplete = true;
     
     if(readings == 3 && !hasntDisplayed){lcd.write(12); lcd.print("Connected!"); hasntDisplayed = true;}
     
     //man.transmit(90000);
     
   }    
   
   if(data == 30000){readingTemp = true; readingWind = false;}
   if(data == 40000){readingTemp = false; readingWind = true;}
   if(data == 50000){readingTemp = false; readingWind = false; rainAmount += .01; rainCount+=.01; lcd.write(227); gotFirstRain = true;}
   /*
   if(!gotFirstRain){
     if(data == 60000){
        readingTemp = false; readingWind = false; rainAmount += .01; rainCount+=.01; lcd.write(227);
   }
 }
 */
 
 gotFirstRain = false;
   
   
                        
                        
                        
                        
                        
   moo = ++moo % 2;
   
  }
}







void setupCharacters(){
 
 lcd.write(249); ///Up Arrow
 lcd.write(B00100);
 lcd.write(B01110);
 lcd.write(B10101);
 lcd.write(B00100);
 lcd.write(B00100);
 lcd.write(B00100);
 lcd.write(B00100);
 lcd.write(B00100);
 
 lcd.write(250); ///Down Arrow
 lcd.write(B00100);
 lcd.write(B00100);
 lcd.write(B00100);
 lcd.write(B00100);
 lcd.write(B00100);
 lcd.write(B10101);
 lcd.write(B01110);
 lcd.write(B00100);

  
  
}

/*


/////////////////major system overall update changelog (including hardware)///////////////////////////




v 4.0.0        2/29/16
completly changed wireless functionality using the manchester library
mounted outside base station to a prop


v 5.0.0        3/04/16
added clock
added pressure sensor
added new anemometer


v 6.0.0        3/09/16
changed the pressure sensors interface
replaced atmega328 for i2c


v 7.0.0        3/10/16
added alarm integration; allerts when high pressure drop(incoming storm); 
added integration for a wireless battery powered alert box (hardware yet to be added)

v 8.0.0        3/14/16
added humdity sensor, DHT11.

added humdity
added better temperature for outside
added Dew Point (DP)
added Heat Index (HI)

v 9.0.0       3/23/16

added Lightning Detection

*/

