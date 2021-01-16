#include <M5StickC.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";
const char* password = "";
const char* entranceUrl = "";

uint32_t ipNumber;
const int lightDpin = 32; // M5StickC
const int lightApin = 33; // M5StickC
const int openLightThreadshold = 3000; // M5StickC
const int closeLightThreadshold = 3700; // M5StickC

void setup() {
  M5.begin();
  connectWiFi();
  initLightSetting();
  initPinSetting();
}

uint16_t analogRead_value = 0;
//uint16_t digitalRead_value = 0;
boolean runningStatus = true;

void loop() {
  checkLight();
  
  if(!runningStatus && analogRead_value < openLightThreadshold) {
    runningStatus = true;
    openEntrance();

    M5.Lcd.fillRect(75, 40, 10, 10, BLACK);
    M5.Lcd.fillRect(10, 60, 150, 20, BLACK);
    M5.Lcd.setCursor(10, 60);
    M5.Lcd.print("I opened entrance\n");
  }
  if (runningStatus && analogRead_value > closeLightThreadshold) {
    runningStatus = false;
    
    M5.Lcd.fillRect(75, 40, 10, 10, BLACK);
    M5.Lcd.fillRect(10, 60, 150, 20, BLACK);
    M5.Lcd.setCursor(10, 60);
    M5.Lcd.print("I am waiting for opening entrance\n");
  }
}

void connectWiFi(){
  // try to connect to the internet
  WiFi.begin(ssid, password);  
  // waiting for connect to the internet
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  M5.Lcd.setCursor(10, 20);
  ipNumber = WiFi.localIP();
}

void openEntrance(){
  if ( WiFi.status() == WL_DISCONNECTED ) {
    connectWiFi();
  }
  HTTPClient http;
  http.begin(entranceUrl);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String response = http.getString();
//    M5.Lcd.setCursor(10, 60);
//    M5.Lcd.print(response);  
  } else {
//    M5.Lcd.setCursor(10, 60);
//    M5.Lcd.print("Error on HTTP request");  
  }
  http.end();
}

void initLightSetting(){
  // set LCD
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setCursor(20, 2);
  M5.Lcd.printf("OpenEntrance\n");  
  M5.Lcd.setCursor(10, 30);
  M5.Lcd.print("GPIO33:");
//  M5.Lcd.setCursor(10, 40);
//  M5.Lcd.print("GPIO32:");  
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.print("status :");  
}

void initPinSetting(){
  // set PIN
  pinMode(lightDpin, INPUT);
  pinMode(lightApin, ANALOG);

}

void checkLight(){
  // put your main code here, to run repeatedly:
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(75, 30);
  M5.Lcd.printf("%d\n", analogRead_value);
//  M5.Lcd.setCursor(75, 40);
//  M5.Lcd.printf("%d\n", digitalRead_value);
  M5.Lcd.setCursor(75, 40);
  M5.Lcd.printf("%d\n", runningStatus);
  
  analogRead_value = analogRead(33);
//  digitalRead_value = digitalRead(32);
  
  M5.Lcd.setTextColor(YELLOW);  
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.printf("%d\n", ipNumber);  
  M5.Lcd.setCursor(75, 30);
  M5.Lcd.printf("%d\n", analogRead_value);
//  M5.Lcd.setCursor(75, 40);
//  M5.Lcd.printf("%d\n", digitalRead_value);
  M5.Lcd.setCursor(75, 40);
  M5.Lcd.printf("%d\n", runningStatus);
  delay(10);
}
