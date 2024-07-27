### **Project Description: ESP32 HTTPClient Jokes API Example**

This project demonstrates how to use an ESP32 microcontroller to connect to a Wi-Fi network, fetch jokes from a web API, and display them on a TFT display. The example uses several libraries to handle Wi-Fi connectivity, HTTP requests, JSON parsing, and graphics rendering.

#### **Components and Libraries Used**
1. **ESP32 Microcontroller**: The main controller handling Wi-Fi and HTTP operations.
2. **Adafruit ILI9341 TFT Display**: A 240x320 pixel color display used to show the fetched jokes.
3. **Wi-Fi Library (`WiFi.h`)**: Manages Wi-Fi connectivity.
4. **HTTPClient Library (`HTTPClient.h`)**: Handles HTTP requests and responses.
5. **ArduinoJson Library (`ArduinoJson.h`)**: Parses JSON responses from the API.
6. **Adafruit GFX and ILI9341 Libraries (`Adafruit_GFX.h`, `Adafruit_ILI9341.h`)**: Provides graphics functions for the TFT display.

#### **Constants and Pin Definitions**
- **SSID and Password**:
  ```cpp
  const char* ssid = "Wokwi-GUEST";
  const char* password = "";
  ```
  These are the credentials for the Wi-Fi network the ESP32 will connect to.
- **TFT and Button Pins**:
  ```cpp
  #define BTN_PIN 5
  #define TFT_DC 2
  #define TFT_CS 15
  Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
  ```
  Defines the pins for the TFT display and the button.

- **Jokes API URL**:
  ```cpp
  const String url = "https://v2.jokeapi.dev/joke/Programming";
  ```
  URL of the API that provides programming jokes.

#### **Functions**

1. **`getJoke()`**:
   - Sends an HTTP GET request to the jokes API.
   - Parses the JSON response to extract the joke.
   - Handles both single-line jokes and two-part jokes (setup and delivery).
   - Returns the joke as a string.

   ```cpp
   String getJoke() {
     HTTPClient http;
     http.useHTTP10(true);
     http.begin(url);
     http.GET();
     String result = http.getString();
     DynamicJsonDocument doc(2048);
     DeserializationError error = deserializeJson(doc, result);
     if (error) {
       Serial.print("deserializeJson() failed: ");
       Serial.println(error.c_str());
       return "<Error>";
     }
     String type = doc["type"].as<String>();
     String joke = doc["joke"].as<String>();
     String setup = doc["setup"].as<String>();
     String delivery = doc["delivery"].as<String>();
     http.end();
     return type.equals("single") ? joke : setup + " " + delivery;
   }
   ```

2. **`nextJoke()`**:
   - Displays a loading message.
   - Fetches a joke using `getJoke()`.
   - Prints the joke to the TFT display.

   ```cpp
   void nextJoke() {
     tft.setTextColor(ILI9341_WHITE);
     tft.println("\nLoading Joke.....");
     String Joke = getJoke();
     tft.setTextColor(ILI9341_GREEN);
     tft.println(Joke);
   }
   ```

3. **`setup()`**:
   - Configures the button pin.
   - Connects to the specified Wi-Fi network.
   - Initializes the TFT display.
   - Displays the IP address once connected.
   - Fetches and displays the first joke.

   ```cpp
   void setup(void) {
     pinMode(BTN_PIN, INPUT_PULLUP);
     WiFi.begin(ssid, password, 6);
     tft.begin();
     tft.setRotation(1);
     tft.setTextColor(ILI9341_WHITE);
     tft.setTextSize(2);
     tft.print("attempt to connect to wifi...");
     while (WiFi.status() != WL_CONNECTED) {
       delay(100);
       tft.print(".");
     }
     tft.print("\nOK IP=");
     tft.print(WiFi.localIP());
     nextJoke();
   }
   ```

4. **`loop()`**:
   - Checks if the button is pressed.
   - If pressed, clears the screen and fetches a new joke.

   ```cpp
   void loop(void) {
     if (digitalRead(BTN_PIN) == LOW) {
       tft.fillScreen(ILI9341_BLACK);
       tft.setCursor(0, 0);
       nextJoke();
     }
   }
   ```

#### **Summary**
This project demonstrates how to use the ESP32 to connect to a Wi-Fi network, send HTTP requests to a jokes API, parse the JSON responses, and display the jokes on a TFT display. It includes handling for both single-line and two-part jokes and allows fetching new jokes by pressing a button.
