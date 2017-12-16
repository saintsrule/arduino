#include <Adafruit_NeoPixel.h>

#include <Wire.h>           // To Be Able To Use Sensors Like IMU9050
#include <EEPROM.h>         // To Be Able To Store & Read Data Between Power Off
#include <ESP8266WiFi.h>    // The Basic Function Of The ESP NOD MCU
#include <math.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#ifdef __AVR__
  #include <avr/power.h>
 
#endif

#define PIN D5

//------------------------------------------------------------------------------------
// WIFI Authentication Variables
//------------------------------------------------------------------------------------
/* This Client Is Going To Connect To A WIFI Server Access Point
 * So You Have To Specify Server WIFI SSID & Password In The Code Not Here
 * Please See The Function Below Name (WiFi.Begin)
 * If WIFI dont need Any password Then WiFi.begin("SSIDNAME")
 * If WIFI needs a Password Then WiFi.begin("SSIDNAME", "PASSWORD")
 */
  char*         TKDssid;            // Wifi Name
  char*         TKDpassword;        // Wifi Password
//------------------------------------------------------------------------------------
// WIFI Module Role & Port
//------------------------------------------------------------------------------------
/* This WIFI Module Code Works As A Client
 * That Will Connect To A Server WIFI Modul With (IP ADDress 192.168.4.1)
 */
  int             TKDServerPort  = 9001;
  IPAddress       TKDServer(192,168,1,34);
  WiFiClient      TKDClient;
//====================================================================================

// added MQTT section //

/************************* MQTT Broker Setup *********************************/

#define AIO_SERVER      "192.168.1.34"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    ""
#define AIO_KEY         ""


/****************************** Feeds ***************************************/

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
//Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Client mqtt(&TKDClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


// Setup a feed for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish tv_runtime = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/tvruntime");
//Adafruit_MQTT_Publish photocell= Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");


// Setup a feed cfor subscribing to changes.
Adafruit_MQTT_Subscribe min_left= Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/timeleft");


/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

// End of MQTT section /////////////////


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);



// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

double run_time;

long time_left=25; // minutes
long num_mins;
long num_5mins;
long num_15mins;
long num_hours;




  void setup() 
  {
    // Setting The Serial Port ----------------------------------------------
    Serial.begin(9600);           // Computer Communication
/*
   Serial.begin(9600);           // Computer Communication

    
    // Setting The Mode Of Pins ---------------------------------------------
    pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
    pinMode(LED1, OUTPUT);          // Indicator For Client #1 Connectivity
//    pinMode(LED2, OUTPUT);          // Indicator For Client #2 Connectivity
    pinMode(D2, OUTPUT);          // Indicator For Client #2 Connectivity
    pinMode(D8, OUTPUT);          // Indicator For Client #2 Connectivity
    pinMode(D9, OUTPUT);          // Indicator For Client #2 Connectivity

    pinMode(BUTTON, INPUT_PULLUP);  // Initiate Connectivity
    digitalWrite(LED0, !LOW);       // Turn WiFi LED Off
    digitalWrite(D8, LOW);       // Turn WiFi LED Off
    
    // Print Message Of I/O Setting Progress --------------------------------
    Serial.println("\nI/O Pins Modes Set .... Done");
*/
    // Starting To Connect --------------------------------------------------
    if(WiFi.status() == WL_CONNECTED)
    {min);
      delay(50);
    }

    /* in this part it should load the ssid and password 
     * from eeprom they try to connect using them */
    
    WiFi.mode(WIFI_STA);            // To Avoid Broadcasting An SSID
//    WiFi.begin("TAKEONE", "");      // The SSID That We Want To Connect To
    WiFi.begin("BTHub3-8629", "a39e4ff654");      // The SSID That We Want To Connect To

    // Printing Message For User That Connetion Is On Process ---------------
    Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");

    // WiFi Connectivity ----------------------------------------------------
    CheckWiFiConnectivity();        // Checking For Connection

    // Stop Blinking To Indicate Connected ----------------------------------
//   digitalWrite(LED0, !HIGH);
    Serial.println("!-- Client Device Connected --!");

    // Printing IP Address --------------------------------------------------
    Serial.println("Connected To      : " + String(WiFi.SSID()));
    Serial.println("Signal Strenght   : " + String(WiFi.RSSI()) + " dBm");
    Serial.print  ("Server IP Address : ");
    Serial.println(TKDServer);
    Serial.print  ("Server Port Num   : ");
    Serial.println(TKDServerPort);
    // Printing MAC Address
    Serial.print  ("Device MC Address : ");
    Serial.println(String(WiFi.macAddress()));
    // Printing IP Address
    Serial.print  ("Device IP Address : ");
    Serial.println(WiFi.localIP());
    
    // Conecting The Device As A Client -------------------------------------
    TKDRequest();
    mqtt.subscribe(&min_left);

  

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

//  c=Color(255,0,0);


  Serial.begin(9600);
  Serial.println();
  Serial.println();

  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
    strip.show();
    }

  }

void loop() {

 // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();


tv_on = digitalRead(D2);
  Serial.print("tv_on state");
  Serial.print(tv_on);

strip.setBrightness(10); //255 = max power

run_time = millis()/1000;



// int reading = digitalRead(BUTTON);
  Serial.print("sending runtime");
  Serial.print(run_time);
//  Serial.print(reading);

if (! tv_runtime.publish(run_time)) {
    Serial.println(F("tvruntime publish Failed"));
  } else {
    Serial.println(F("tvruntime publish OK!"));
  }







 Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &min_left) {
      Serial.print(F("Got: "));
      Serial.println((char *)min_left.lastread);
//      flash=(char *)onoffbutton.lastread;
   }
//    if (subscription == &pin) {
//      Serial.print(F("Got: "));
//      Serial.println((char *)pin.lastread);
//      pin_ref=(char *)pin.lastread;
//    }  
  } // end of subscription loop




//delay(1000);

time_left=min_left;

String message=String(run_time);
Serial.println(message);


num_5mins=int(time_left/5);
num_mins=int(time_left-(num_5mins*5));

//message=String(num_5mins);


  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
    strip.show();
    }




// set 5 min leds
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (i<num_5mins)
    {

    strip.setPixelColor(i, strip.Color(255,255,0));
    strip.show();
    }
    else 
    {
//    strip.setPixelColor(i, 0);
//    strip.show();
    }
  }

// set 1 min leds
  for(uint16_t i=0; i<5; i++) {
    if (i<num_mins)
    {
    strip.setPixelColor((7-i), strip.Color(255,0,0));
    strip.show();
    }
  }


 





//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 1000); // Green



  
// Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
// Send a theater pixel chase in...
//  theaterChase(strip.Color(127, 127, 127), 50); // White
//  theaterChase(strip.Color(127, 0, 0), 50); // Red
//  theaterChase(strip.Color(0, 0, 127), 50); // Blue

//  rainbow(20);
//  rainbowCycle(20);
//  theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint64_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}






// Fill the dots one after the other with a color
void countdown(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


//====================================================================================

  void CheckWiFiConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      for(int i=0; i < 10; i++)
      {
//        digitalWrite(LED0, !HIGH);
        delay(250);
//        digitalWrite(LED0, !LOW);
        delay(250);
        Serial.print(".");
      }
      Serial.println("");
    }
  }

//====================================================================================

  void I2C_Begin()
  {
    /* ----------------------------------------------------------------------
     * Setting The I2C Pins SDA, SCL
     * Because We Didnt Specify Any Pins The Defult
     * SDA = D4 <GPIO2>, SCL = D5 <GPIO14> For ESP8266 Dev Kit Node MCU v3
     --------------------------------------------------------------------- */
    // Frequency Of The I2C
    #define TWI_FREQ  400000L       // I2C Frequency Setting To 400KHZ
    
    // If Ever You Needed To Use Wire I2C
    Wire.begin();                   // Begginning The I2C
    
    // Setting Up The I2C Of The MPU9250 ------------------------------------
    Wire.setClock(TWI_FREQ);        // Setting The Frequency MPU9250 Require
  }
        
//====================================================================================

  void TKDRequest()
  {
    // First Make Sure You Got Disconnected
//    TKDClient.stop();   // THIS MAY BE NEEDED !!!

    // If Sucessfully Connected Send Connection Message
    if(TKDClient.connect(TKDServer, TKDServerPort))
    {
//      Serial.println    ("<" + ButtonColor + "-CONNECTED>");
//      TKDClient.println ("<" + ButtonColor + "-CONNECTED>");
    }
  }

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(1000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}






