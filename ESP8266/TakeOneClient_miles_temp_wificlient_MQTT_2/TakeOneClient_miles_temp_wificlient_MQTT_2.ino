//------------------------------------------------------------------------------------
// Libraries Needed For This Project
//------------------------------------------------------------------------------------
  #include <Wire.h>           // To Be Able To Use Sensors Like IMU9050
  #include <EEPROM.h>         // To Be Able To Store & Read Data Between Power Off
  #include <ESP8266WiFi.h>    // The Basic Function Of The ESP NOD MCU
  #include <math.h>
  #include "Adafruit_MQTT.h"
  #include "Adafruit_MQTT_Client.h"


//------------------------------------------------------------------------------------
// Defining I/O Pins
//------------------------------------------------------------------------------------
  #define       LED0      D7         // WIFI Module LED
  #define       LED1      D0        // Indicate Connectivity With Client #1
  #define       LED2      D2        // Indicate Connectivity With Client #2
  #define       BUTTON    D1        // Connectivity ReInitiate Button
//------------------------------------------------------------------------------------
// BUTTON Variables
//------------------------------------------------------------------------------------
  int           ButtonState;
  int           LastButtonState   = LOW;
  int           LastDebounceTime  = 0;
  int           DebounceDelay     = 50;
  const String  ButtonColor       = "RED";

  const int ANALOG_PIN = A0; 


  unsigned int Rs = 1000;
  double Vcc = 3.3;


  
//------------------------------------------------------------------------------------
// LED Delay Variables
//------------------------------------------------------------------------------------
  int           LEDState          = LOW;
  unsigned long CurrMillis        = 0;
  unsigned long PrevMillis        = 0;
  unsigned long Interval          = 1000;
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

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "smileymiley"
#define AIO_KEY         "6e3492350ecd44e99866adf1b9e0768e"


/****************************** Feeds ***************************************/

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
//Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Client mqtt(&TKDClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

// End of MQTT section /////////////////

  void setup() 
  {
    // Setting The Serial Port ----------------------------------------------
//    Serial.begin(115200);           // Computer Communication
   Serial.begin(9600);           // Computer Communication

    
    // Setting The Mode Of Pins ---------------------------------------------
    pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
    pinMode(LED1, OUTPUT);          // Indicator For Client #1 Connectivity
    pinMode(LED2, OUTPUT);          // Indicator For Client #2 Connectivity
    pinMode(BUTTON, INPUT_PULLUP);  // Initiate Connectivity
    digitalWrite(LED0, !LOW);       // Turn WiFi LED Off
    
    // Print Message Of I/O Setting Progress --------------------------------
    Serial.println("\nI/O Pins Modes Set .... Done");

    // Starting To Connect --------------------------------------------------
    if(WiFi.status() == WL_CONNECTED)
    {
      WiFi.disconnect();
      WiFi.mode(WIFI_OFF);
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
    digitalWrite(LED0, !HIGH);
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

    mqtt.subscribe(&onoffbutton);
  
  }

//====================================================================================

uint32_t x=0;
  
  void loop()
  {


  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }

  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */


    
//  String s="Data "; 
  String s="Data "; 

//    s += "Analog Pin = ";
    s += String(analogRead(ANALOG_PIN));
//    Serial.print(s);
//    ReadButton();

  Serial.print(s);
  delay(1000);
//  TKDClient.println(Thermister(AnalogRead()));
  TKDClient.print(s);

          TKDClient.flush();
}

int AnalogRead() {
  int val = 0;
  for(int i = 0; i < 20; i++) {
    val += analogRead(A0);
    delay(1);
  }

  val = val / 20;
  return val;
}

double Thermister(int val) {
  double V_NTC = (double)val / 1024;
  double R_NTC = (Rs * V_NTC) / (Vcc - V_NTC);
  R_NTC = log(R_NTC);
  double Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * R_NTC * R_NTC ))* R_NTC );
  Temp = Temp - 273.15;          
  return Temp;

}


//====================================================================================

  void ReadButton()
  {
    // Reading The Button
    int reading = digitalRead(BUTTON);

    // If It Doest Match The Previous State
    if(reading != LastButtonState)
    {
      LastDebounceTime = millis();
    }

    // To Iliminate Debounce
    if((millis() - LastDebounceTime) > DebounceDelay)
    {
      if(reading != ButtonState)
      {
        ButtonState = reading;
        
        if(ButtonState == LOW)
        {
          LEDState    = !digitalRead(LED1);
          digitalWrite(LED1, LEDState);
          Serial.println("<" + ButtonColor + "-SCORED>");
          TKDClient.println("<" + ButtonColor + "-SCORED>");
          TKDClient.flush();
        }
      }
    }

    // Last Button State Concidered
    LastButtonState = reading;
  }

//====================================================================================

  void CheckWiFiConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      for(int i=0; i < 10; i++)
      {
        digitalWrite(LED0, !HIGH);
        delay(250);
        digitalWrite(LED0, !LOW);
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
    TKDClient.stop();

    // If Sucessfully Connected Send Connection Message
    if(TKDClient.connect(TKDServer, TKDServerPort))
    {
      Serial.println    ("<" + ButtonColor + "-CONNECTED>");
      TKDClient.println ("<" + ButtonColor + "-CONNECTED>");
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
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}



//====================================================================================
