#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6XG5NF5e3"
#define BLYNK_TEMPLATE_NAME "Tubes"
#define BLYNK_AUTH_TOKEN "2f4r87sP8MUn_zKsxpCGuSfLYVTyyTVT"

#include <DHT.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleShieldEsp8266.h>

#define DHTPIN A2
#define DHTTYPE DHT11
#define RELAY_PIN A3

SoftwareSerial EspSerial(4, 3); // RX, TX

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "FSJKUN";
char pass[] = "fadhlan15";

#define ESP8266_BAUD 115200
ESP8266 wifi(&EspSerial);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(RELAY_PIN, OUTPUT);       // Set relay pin as output
  Serial.begin(115200);             // Initialize serial communication
  digitalWrite(RELAY_PIN, HIGH);
  dht.begin();                      // Initialize DHT sensor
  EspSerial.begin(ESP8266_BAUD);    // Initialize ESP8266 serial communication
  Blynk.begin(auth, wifi, ssid, pass); // Initialize Blynk with authentication token
  
}

void sendData(float temperature) {
  Blynk.virtualWrite(V0, temperature);
}

BLYNK_WRITE(V1) {
  int pinValue = param.asInt(); // Get the value from V1
  if (pinValue == 1) {
    digitalWrite(RELAY_PIN, LOW); // Turn on relay
    Serial.println("Relay ON via Blynk");
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Turn off relay
    Serial.println("Relay OFF via Blynk");
  }
}

void loop() {
  Blynk.run();  // Process Blynk events
  delay(10);  // Wait 500 milliseconds between readings

  float temperature = dht.readTemperature(); // Read temperature in Celsius
  if (isnan(temperature)) { // Check if reading was successful
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");
  Blynk.virtualWrite(V0, temperature);
  // sendData(temperature); // Send temperature data to Blynk

  if (temperature < 27) {
    digitalWrite(RELAY_PIN, LOW); // Turn on relay
    Serial.println("Lamp ON due to temperature");
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Turn off relay
    Serial.println("Lamp OFF due to temperature");
  }
}
