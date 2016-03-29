#include <Manchester.h>

int temp;

  int Rain;

int amount;

long timeBetweenFlips;

long time;

boolean flipped = false;

long threshTime = 100000;

#define TX_PIN 3  //pin where your transmitter is connected
  

void setup(){
 
 man.setupTransmit(TX_PIN, MAN_1200);
    pinMode(1, INPUT);
  
}


void loop(){
 
  
  //timeBetweenFlips = pulseIn(1, HIGH, 6000);
  
  
  
  time++;
  
 readRain();
 if(time >= threshTime){
 readTemp();
 readWind();
 readRain();
 rfSend(2000);
 time = 0;
 }
}

  
  




void rfSend(long data){
  
 man.transmit(data);
  
}
  
  int readTemp(){
    
    
    

    
    
   int t1;
   int t2;
   int t3;
   int t4;
   
    int curTempInt;
    
    float curTemp;
    
    curTemp = (analogRead(A1)  * (5000/1024));
    curTemp = ((curTemp) - 500) / 10;
    
    curTemp = (curTemp * 9.0 / 5.0) + 32.0;
    
    
    
rfSend(30000);
rfSend(curTemp * 100);

    
    
  }
  
  
int readWind(){
  
  long WindSpeed;
  
 WindSpeed = analogRead(A2);

rfSend(40000);
rfSend(WindSpeed);


  
}

int readRain(){
 

  
  if(digitalRead(1) == 1 && flipped == false){

  delay(10);
  flipped = true;
   rfSend(50000);
   rfSend(60000); 
  }
  
    
  if(digitalRead(1) == 0 && flipped == true){
  flipped = false; 
  }
  

  
}
