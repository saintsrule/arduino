

BlynkTimer timer;

void setup()   {
  Serial.begin(9600);
  sensors.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

Serial.println("here");

Blynk.disconnect();
//Blynk.config("0cffff9d8d3845aca64726e21e8347ba", "blynk-cloud.com", 8442);  // this worked before
Blynk.config("0cffff9d8d3845aca64726e21e8347ba", "blynk-cloud.com");



//Blynk.connectWiFi("BTHub3-8629", "a39e3ff654");

//  Blynk.begin(auth, "SSID", "PASS");
//  Blynk.begin("0cffff9d8d3845aca64726e21e8347ba", "BTHub3-8629", "a39e3ff654");

Serial.println("here2");
Blynk.connect();

  // declare all pins as output, works better on wemos

  pinMode(D0, OUTPUT); 
  
  pinMode(D4, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D8, OUTPUT);

//  digitalWrite(D0, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D8, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);



//  digitalWrite(D0, HIGH);
//  digitalWrite(D0, HIGH);
//  digitalWrite(D2, HIGH);
//  digitalWrite(D3, HIGH);
//  digitalWrite(D5, HIGH);
//  digitalWrite(D6, HIGH);
//  digitalWrite(D8, HIGH);
  delay(1000);


//  digitalWrite(D0, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D8, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);

Serial.println("here3");

}
