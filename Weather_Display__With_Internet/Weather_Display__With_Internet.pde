import processing.serial.*;

Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port

boolean updateWebPage = false;

import processing.net.*;

String HTTP_GET_REQUEST = "GET /";
String HTTP_HEADER = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n";

Server s;
Client c;
String input;

String[] data;
String InTemp = "";
int i = 0;


void setup(){
  size(800, 600);
myPort = new Serial(this, "COM6", 9600); 
s = new Server(this, 8080); // start server on http-alt
}

void draw()
{
  
  
  if ( myPort.available() > 0) 
  {  // If data is available,
  val = myPort.readStringUntil('\n');         // read it and store it in val
  data = split(val, ',');
  background(255,255,255);
  i++;
  
updateWebPage = true;

if(val != null){
  

line(0, 270, 800, 270);
line(400, 0, 400, 430);

  
fill(0,255,0);
textSize(32);
text("Inside Temperature", 10, 50);
text(data[8], 10, 80);

fill(0,255,0);
textSize(32);
text("Outside Temperature", 10, 120);
text(data[0], 10, 160);

fill(0,255,0);
textSize(32);
text("Feels Like", 10, 200);
text(data[5], 10, 240);

fill(0,0,255);
textSize(32);
text("Pressure", 10, 300);
text(data[6], 10, 340);


fill(0,0,255);
textSize(32);
if(int(data[7]) == 0){
text("Steady", 150, 300);
}

if(int(data[7]) == 2){
text("Rising", 150, 300);
}

if(int(data[7]) == 1){
text("Falling", 150, 300);
}

fill(0,0,255);
textSize(32);
text("Predicton", 10, 380);
text(data[12], 10, 420);

line(0, 430, 800, 430);

fill(0,255,255);
textSize(32);
text("Wind Speed", 10, 460);
text(data[3], 10, 500);

fill(0,255,255);
textSize(32);
text("Todays Rain Acc.", 10, 540);
text(data[9], 10, 580);

fill(255,0,255);
textSize(32);
text("Relative Humidity", 410, 50);
text(data[4] + "%", 410, 90);

fill(0,255,255);
textSize(32);
text("Todays Rain Acc.", 10, 540);
text(data[9], 10, 580);

fill(255,0,255);
textSize(32);
text("Dew Point", 410, 130);
text(data[11], 410, 170);


fill(255,0,0);
textSize(32);
text("High Temperature", 410, 300);
text(data[1], 410, 340);

fill(0,0,255);
textSize(32);
text("Low Temperature", 410, 380);
text(data[2], 410, 410);

fill(0,255,255);
textSize(32);
text("Rain/Hour", 410, 460);
text(data[10], 410, 490);

///////////internet things////////////////////

c = s.available();
  if (c != null) {
    input = c.readString();
    input = input.substring(0, input.indexOf("\n")); // Only up to the newline
    
    if (input.indexOf(HTTP_GET_REQUEST) == 0) // starts with ...
    {
    c.write(HTTP_HEADER);  
    
    // some html
    c.write("<html><head><title>Salem Road Weather</title></head><body><h3>Salem Road Weather Conditions<p>------------------------------------------------</p><p>Outside Temperature: ");
      c.write("<meta http-equiv=refresh content=5>");  
    c.write(data[0]);
    c.write("</p>");
    c.write("<p>Feels Like: ");
    c.write(data[5]);
    c.write("</p>");
    c.write("<p>High Temp: ");
    c.write(data[1]);
     c.write("    Low Temp: ");
    c.write(data[2]);
    c.write("</p>");
    c.write("<p>Inside Temperature: ");
    c.write(data[8]);
    c.write("</p>");
    c.write("<p>Pressure: ");
    c.write(data[6]);
    
    
    if(int(data[7]) == 0){
c.write(" Steady");
}

if(int(data[7]) == 2){
c.write(" Falling");
}

if(int(data[7]) == 1){
c.write(" Rising");
}
    
    c.write("</p>");
    c.write("<p>Wind Speed: ");
    c.write(data[3]);
    c.write(" MPH</p>");
    c.write("<p>Top Speed: ");
    c.write(data[14]);
    c.write(" MPH</p>");
    c.write("<p>Relative Humdity: ");
    c.write(data[4]);
    c.write("%</p>");
    c.write("<p>Dew Point: ");
    c.write(data[11]);
    c.write("</p>");
    c.write("<p>Todays Total Rainfall: ");
    c.write(data[9]);
    c.write(" in</p>");
    c.write("</p>");
    c.write("<p>Rain/Hour: ");
    c.write(data[10]);
    c.write(" in</p>");
    c.write("<p>Todays Lightning Strike Count: ");
    c.write(data[15]);
    c.write("</p>");
    
    c.write("<p>------------------------------------------------</p>");
    
    c.write("<p>Prediction: ");
    c.write(data[12]);
    c.write("</p>");
    
    c.write("<p>Storm Alert: ");
    if(int(data[13]) == 1){
    c.write("Yes!");
    }
    if(int(data[13]) == 0){
    c.write("No");
    }
    c.write("</p>");
  
    
    c.write("</html>");
    // close connection to client, otherwise it's gonna wait forever
    c.stop();
    }
  }


}
}
}