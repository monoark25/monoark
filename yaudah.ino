#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6U9YIshdq"
#define BLYNK_TEMPLATE_NAME "Smart Home"
#define BLYNK_AUTH_TOKEN "3-xm9svAuNxNz3NuwPI3wWJAdZPeABAZ"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>

#define DHT_PIN 15
#define RELAY_1 13
#define RELAY_2 23
#define RELAY_3 25
#define RELAY_4 26

#define SW_1 2
#define SW_2 12
#define SW_3 14

#define VPIN_SW1 V0
#define VPIN_SW2 V1
#define VPIN_SW3 V2
#define VPIN_SW4 V3
#define VPIN_SUHU V4

bool relay1_state = LOW;
bool relay2_state = LOW;
bool relay3_state = LOW;

bool sw1_state = LOW;
bool sw2_state = LOW;
bool sw3_state = LOW;

DHTesp dhtSensor;
LiquidCrystal_I2C lcd(0x3F, 16, 2);  

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Andro"; //ganti sesuai hotspot/wifi yang tersedia
char pass[] = "mau";

BlynkTimer timer;

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_SW1);
  Blynk.syncVirtual(VPIN_SW2);
  Blynk.syncVirtual(VPIN_SW3);
  Blynk.syncVirtual(VPIN_SW4);
}
//BLYNK V0
BLYNK_WRITE(VPIN_SW1)
{
 relay1_state = param.asInt();
 if(relay1_state == 1) {
  digitalWrite(RELAY_1, LOW);
 }
 else{
  digitalWrite(RELAY_1, HIGH);
 }
}
//BLYNK V1
BLYNK_WRITE(VPIN_SW2)
{
 relay2_state = param.asInt();
 if(relay2_state == 1) {
  digitalWrite(RELAY_2, LOW);
 }
 else{
  digitalWrite(RELAY_2, HIGH);
 }
}
//BLYNK V2
BLYNK_WRITE(VPIN_SW3)
{
 relay3_state = param.asInt();
 if(relay3_state == 1) {
  digitalWrite(RELAY_3, LOW);
 }
 else{
  digitalWrite(RELAY_3, HIGH);
 }
}
//BLYNK V3
BLYNK_WRITE(VPIN_SW4)
{
 int relay4_state = param.asInt();
 digitalWrite(RELAY_4, !relay4_state);
 }

//fungsi baca sensor
void sendSensor()
{
 TempAndHumidity  data = dhtSensor.getTempAndHumidity();
 float temperature = data.temperature;
 float humidity = data.humidity;
 lcd.setCursor(0,0);
 lcd.print("Temp: "+String(temperature));
 lcd.setCursor(0,1);
 lcd.print("Hum: "+String(humidity));
 Blynk.virtualWrite(VPIN_SUHU, temperature); 
}

//fungsi untuk kontrol manual tanpa Blynk
void manual_control()
{
  //switch1
  if (digitalRead(SW_1) == LOW && sw1_state == LOW) {
    digitalWrite(RELAY_1, LOW);
    Blynk.virtualWrite(VPIN_SW1, HIGH);
    relay1_state = HIGH;
    sw1_state = HIGH;
  }
  if (digitalRead(SW_1) == HIGH && sw1_state == HIGH) {
    digitalWrite(RELAY_1, HIGH);
    Blynk.virtualWrite(VPIN_SW1, LOW);
    relay1_state = LOW;
    sw1_state = 0;
  }
  //switch2
  if (digitalRead(SW_2) == LOW && sw2_state == LOW) {
    digitalWrite(RELAY_2, LOW);
    Blynk.virtualWrite(VPIN_SW2, HIGH);
    relay2_state = HIGH;
    sw2_state = HIGH;
  }
  if (digitalRead(SW_2) == HIGH && sw2_state == HIGH) {
    digitalWrite(RELAY_2, HIGH);
    Blynk.virtualWrite(VPIN_SW2, LOW);
    relay2_state = LOW;
    sw2_state = 0;
  }
    //switch3
  if (digitalRead(SW_3) == LOW && sw3_state == LOW) {
    digitalWrite(RELAY_3, LOW);
    Blynk.virtualWrite(VPIN_SW3, HIGH);
    relay3_state = HIGH;
    sw3_state = HIGH;
  }
  if (digitalRead(SW_3) == HIGH && sw3_state == HIGH) {
    digitalWrite(RELAY_3, HIGH);
    Blynk.virtualWrite(VPIN_SW3, LOW);
    relay3_state = LOW;
    sw3_state = 0;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  pinMode(SW_1, INPUT_PULLUP);
  pinMode(SW_2, INPUT_PULLUP);
  pinMode(SW_3, INPUT_PULLUP);

  //kondisikan relay OFF diawal
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);

  Blynk.begin(auth, ssid, pass);

  lcd.init();                      
  lcd.backlight();

  dhtSensor.setup(DHT_PIN, DHTesp::DHT11);
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
  manual_control();
}
