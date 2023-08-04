
// Import required libraries //data uploaddata tidak menggunakan little fs
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

#include <Wire.h>
#include "Sodaq_DS3231.h"

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

// Replace with your network credentials
const char* ssid = "esp8266_a_point";
const char* password = "123456789";

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;
float tekanan;
int data3;
const char* teks="nama"; 
String DataTeks;
int thn,bln,tgl,j,mnt,hr;
const char* bulan="";
const char* tanggal="";

unsigned long previousMillis=0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String getTemperature() {
  DateTime now = rtc.now();
  int ja=now.hour();
  int me=now.minute();
  int det=now.second();
  char buffer[30];
  sprintf(buffer, "%d : %d : %d", ja, me, det);
  //Serial.println(buffer);
  return String(buffer);//temperature);
}
  
String getHumidity() {
  DateTime now = rtc.now();
  int th= now.year();
  int bl=now.month();
  int tg=now.date();
  char buffer[30];
  sprintf(buffer, "%d / %d / %d", tg, bl, th);
  //Serial.println(humidity);
  return String(buffer); //humidity);
}

String getPressure() {    //ini tidak dipakai
  DateTime now = rtc.now();
  float pressure = 4.2;   //sebagai simulasi nilainya konstan
  //Serial.println(pressure);
  return String(pressure);
}

String AngkaAcak() {
  DateTime now = rtc.now();
  int pres=now.dayOfWeek();
  String buffer;
  if (pres=0){buffer="MINGGU";}
   if (pres=1){buffer="SENIN";}
    if (pres=2){buffer="SELASA";}
     if (pres=3){buffer="RABU";}
      if (pres=4){buffer="KAMIS";}
       if (pres=5){buffer="JUMAT";}
        if (pres=6){buffer="SABTU";}
  return String(buffer);//pressure);
}

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "OFF";
    }
    else{
      ledState = "ON";
    }
    Serial.print(ledState);
    return ledState;
  }
  else if (var == "JAM"){
    return getTemperature();
  }
  else if (var == "TANGGAL"){
    return getHumidity();
  }
  else if (var == "HARI"){
    return AngkaAcak();   //getPressure(); dicoba manampilkan nilai acak.
  }  
  return "ada yang error";
}


void setup(){
  // Serial port for debugging purposes
   Wire.begin();
    rtc.begin();
    
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

//  // Connect to Wi-Fi //ini jika esp bertindak ttidak sebagai accesspoint tapi konek ke sebuah jaringan wifi
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(1000);
//    Serial.println("Connecting to WiFi..");
//  }
//
//  // Print ESP32 Local IP Address
//  Serial.println(WiFi.localIP());

 // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  //mencetak acces point
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
//  // Route to load style.css file
//  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send(SPIFFS, "/style.css", "text/css");
//  });

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);  
    data3=data3+1;   
    if (data3>100){
      data3=0;
    }  
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH); 
    data3=data3+1;   
    if (data3>100){
      data3=0;
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  //route untuk menampilkan temperatur
  server.on("/jam", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTemperature().c_str());
  });
  
   //route untuk menampilkan humidity
   server.on("/tangal", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getHumidity().c_str());
  });

  //route untuk manmpilkan nilai pressure (dalam hal ini adalah nilai acak
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", AngkaAcak().c_str()); 
  });

  //route untuk manangani data form inputan dikirim
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request){
    String inputMessage1;
    String inputMessage2;
    String inputMessage3;
    String inputMessage4;
    String inputMessage5;
    String inputMessage6;
   
   // if (request->hasParam(teks)) {
      inputMessage1 = request->getParam("nama")->value();
      inputMessage2 = request->getParam("bulan")->value();
      inputMessage3 = request->getParam("tanggal")->value();
      inputMessage4 = request->getParam("jam")->value();
      inputMessage5 = request->getParam("menit")->value();
      inputMessage6 = request->getParam("hari")->value();
      //inputParam = teks;
    //}

    thn = inputMessage1.toInt();
    bln =inputMessage2.toInt();
    tgl =inputMessage3.toInt();
    j =inputMessage4.toInt();
    mnt =inputMessage5.toInt();
    //hr =inputMessage6.toInt();
    if (inputMessage6="Minggu"){hr=0;}
        if (inputMessage6="Senin"){hr=1;}
            if (inputMessage6="Selasa"){hr=2;}
                if (inputMessage6="Rabu"){hr=3;}
                    if (inputMessage6="Kamis"){hr=4;}
                        if (inputMessage6="Jumat"){hr=5;}
                            if (inputMessage6="Sabtu"){hr=6;}

    DateTime dt(thn, bln, tgl, j, mnt, 0, hr);

    rtc.setDateTime(dt);
    //Serial.println(teks);
    //DataTeks=inputMessage1;
    Serial.println(inputMessage1);
    Serial.println(inputMessage2);
    Serial.println(inputMessage3);
    Serial.println(inputMessage4);
    Serial.println(inputMessage5);
    Serial.println(inputMessage6);
   request->send(SPIFFS, "/index.html", String(), false, processor);
   
  });
  
  server.serveStatic("/",SPIFFS,"/");
  
  // Start server
  server.begin();
  //server.handleClient();
}
 
void loop(){
  //tampilAngcak(tekanan);
  //testdrawstyles();
  int data1 = random(1, 10000); // generate the integers
  float data2 = data1; // generate the numbers after the decimal point
  tekanan = data2 / 100; // sum the resulting values
  //tekanan = random(1000)/100;

 unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    DateTime now = rtc.now(); //get the current date-time
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.date(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.print(weekDay[now.dayOfWeek()]);
    Serial.println();
  }

  
}
