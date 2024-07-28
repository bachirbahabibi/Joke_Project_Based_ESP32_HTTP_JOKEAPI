/*
  ESP32 HTTPClient Jokes API Example

  https://wokwi.com/projects/342032431249883731

  Copyright (C) 2024, EL BACHIR BAHABIBI
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Adafruit_ILI9341.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

#define BTN_PIN 5
#define TFT_DC 2
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

const String url="https://v2.jokeapi.dev/joke/Programming"; 


String getJoke(){
  HTTPClient http; 
  http.useHTTP10(true);
  http.begin(url); 
  http.GET(); /* send request to server to get response body */
  String result = http.getString(); /* retreive the response body from the server as a string */ 
  DynamicJsonDocument doc(2048); /* create json doc in the memory */ 
  DeserializationError error= deserializeJson(doc,result);
  /* test if parsing succeded */ 
  if(error){
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return "<Error>";
  }
  String type = doc["type"].as<String>(); 
  String joke = doc["joke"].as<String>();
  String setup= doc["setup"].as<String>();
  String delivery=doc["delivery"].as<String>();
  http.end();
  return type.equals("single") ? joke : setup + " " + delivery;
}

void nextJoke(){
  tft.setTextColor(ILI9341_WHITE  ); /* set text color to white */
  tft.println("\nLoading Joke.....");
  String Joke=getJoke(); /* get the joke from the server using getJoke function */ 
  tft.setTextColor(ILI9341_GREEN);
  tft.println(Joke);  /* print the Joke */ 
}

void setup(void){

pinMode(BTN_PIN, INPUT_PULLUP); /* define the button input pin as pullup by resistor */ 
WiFi.begin(ssid, password,6); /* define wifi ssid, password, and channel frequency */ 
tft.begin();
tft.setRotation(1);
tft.setTextColor(ILI9341_WHITE);
tft.setTextSize(2);
tft.print("atempt to connect to wifi...");
while(WiFi.status() != WL_CONNECTED){
  delay(100);
  tft.print(".");
}
tft.print("\nOK IP=");
tft.print(WiFi.localIP());
nextJoke();
}

void loop(void){

if(digitalRead(BTN_PIN)== LOW){
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  nextJoke();
}


}