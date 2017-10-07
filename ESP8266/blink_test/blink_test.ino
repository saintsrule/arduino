void setup() {
  pinMode(D7, OUTPUT);
}
 
void loop() {
  digitalWrite(D7, HIGH);
  delay(500);
  digitalWrite(D7, LOW);
  delay(500);
}
