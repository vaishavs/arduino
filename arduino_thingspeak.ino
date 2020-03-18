#include <SoftwareSerial.h>

const int sensor = A0;  //moisture sensor
const int OUT =  2;     //digital output

String apiKey = "62AFF1XXK91V5KVU";   // Edit this API key according to your Account    

String Host_Name = "SSID";      // Edit Host_Name
String Password = "Password";   // Edit Password

SoftwareSerial ser(8, 9);       // RX, TX

void setup()
{                
  Serial.begin(115200);       // enable software serial
  ser.begin(115200);          // reset ESP8266
  //ser.println("AT+RST");    // Resetting ESP8266

  pinMode(sensor, INPUT);
  pinMode(OUT, OUTPUT);

  char inv ='"';

  // AT commands to connect ESP8266 to your WiFi Router
  String cmd = "AT+CWJAP";
  cmd+= "=";
  cmd+= inv;
  cmd+= Host_Name;
  cmd+= inv;	
  cmd+= ",";
  cmd+= inv;
  cmd+= Password;
  cmd+= inv;
  ser.println(cmd);
}

void loop() 
{

  int sensor1 =  analogRead(A0);  // Reading Moisture1 Value

  String s = “GET https://api.thingspeak.com/apps/thinghttp/send_request?”+“api_key=62AFF1XXK91V5KVU”;

  if (sensor1 >= 551)
    digitalWrite(RELAY, LOW);

  else if(sensor1 <= 185)
    digitalWrite(RELAY, HIGH);

  if (sensor1 > 561)
  {
    digitalWrite(RELAY, LOW);
    Serial.println("The field is too dry!!");
    ser.println(s);
  }
  
  else if(sensor1 <= 140)
  {
    Serial.println("The field is too wet!!");
    digitalWrite(RELAY, HIGH);
    ser.println(s);
  }
    
  String values = "\nSensor value: " + String(sensor1);
  Serial.println(values); 

  delay(1000);  

  // Establishing TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";          
  cmd += "18.214.44.70";    // api.thingspeak.com
  cmd += "\",80";           // port 80
  ser.println(cmd);

  Serial.println(cmd);

  if(ser.find("Error"))
  {
    Serial.println("AT+CIPSTART error");
    return;
  }

  // prepare GET string
  String getStr = "GET https://api.thingspeak.com/update?api_key=";         
  getStr += apiKey;           //API key
  getStr +="&field1=";
  getStr += String(sensor);   // Moisture sensor Data
  getStr += "\r\n\r\n";

  //Send the GET string
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

  Serial.println(cmd);

  if(ser.find(">"))
  {
    ser.print(getStr);
    Serial.print(getStr);
  }

  else
  {
    ser.println("AT+CIPCLOSE");                  // closing connection
    Serial.println("AT+CIPCLOSE");
  }
  delay(15000);                                  // Update after every 15 seconds
}
