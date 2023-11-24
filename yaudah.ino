#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID " saya"
#define BLYNK_TEMPLATE_NAME "aja aja ada"
#define BLYNK_AUTH_TOKEN "templet"

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

#define sakalr1 2
#define sakalr2 12
#define sakalr3 14

#define Saklar1V V0
#define Saklar2V V1
#define Saklar3V V2
#define Saklar4V V3
#define suhuv V4


bool relay1_state = LOW;
bool relay2_state = LOW;
bool relay3_state = LOW;

bool sw1_state = LOW;
bool sw2_state = LOW;
bool sw3_state = LOW;

DHTesp dhtSensor;
LiquidCrystal_I2C lcd(0x3F, 16, 2);  

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "aspalpanas"; 
char pass[] = "tulangpaus";

BlynkTimer timer;

BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(Saklar1V);
  Blynk.syncVirtual(Saklar2V);
  Blynk.syncVirtual(Saklar3V);
  Blynk.syncVirtual(Saklar4V);
}
//BLYNK V0
BLYNK_WRITE(Saklar1V)
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
BLYNK_WRITE(Saklar2V)
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
BLYNK_WRITE(Saklar3V)
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
BLYNK_WRITE(Saklar4V)
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
 Blynk.virtualWrite(suhuv, temperature); 
}

//fungsi untuk kontrol manual tanpa Blynk
void manual_control()
{
  //switch1
  if (digitalRead(sakalr1) == LOW && sw1_state == LOW) {
    digitalWrite(RELAY_1, LOW);
    Blynk.virtualWrite(Saklar1V, HIGH);
    relay1_state = HIGH;
    sw1_state = HIGH;
  }
  if (digitalRead(sakalr1) == HIGH && sw1_state == HIGH) {
    digitalWrite(RELAY_1, HIGH);
    Blynk.virtualWrite(Saklar1V, LOW);
    relay1_state = LOW;
    sw1_state = 0;
  }
  //switch2
  if (digitalRead(sakalr2) == LOW && sw2_state == LOW) {
    digitalWrite(RELAY_2, LOW);
    Blynk.virtualWrite(Saklar2V, HIGH);
    relay2_state = HIGH;
    sw2_state = HIGH;
  }
  if (digitalRead(sakalr2) == HIGH && sw2_state == HIGH) {
    digitalWrite(RELAY_2, HIGH);
    Blynk.virtualWrite(Saklar2V, LOW);
    relay2_state = LOW;
    sw2_state = 0;
  }
    //switch3
  if (digitalRead(sakalr3) == LOW && sw3_state == LOW) {
    digitalWrite(RELAY_3, LOW);
    Blynk.virtualWrite(Saklar3V, HIGH);
    relay3_state = HIGH;
    sw3_state = HIGH;
  }
  if (digitalRead(sakalr3) == HIGH && sw3_state == HIGH) {
    digitalWrite(RELAY_3, HIGH);
    Blynk.virtualWrite(Saklar3V, LOW);
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
  pinMode(sakalr1, INPUT_PULLUP);
  pinMode(sakalr2, INPUT_PULLUP);
  pinMode(sakalr3, INPUT_PULLUP);

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
