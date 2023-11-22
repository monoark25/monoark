#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL69C8sinEi"
#define BLYNK_TEMPLATE_NAME "SWITCH DHT11"
#define BLYNK_AUTH_TOKEN "pFDUVPpVfMYPPz49LJklsAQ7lfT_5WuL"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//LiquidCrystal_I2C
#include <LiquidCrystal_I2C.h>
// set the LCD number of columns and rows
int lcdColumns = 40;
int lcdRows = 4;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
// Sesuaikan dengan datastream kalian 
#define vlampu1 V1
#define vlampu2 V2
#define vlampu3 V3
#define vkipas  V4
#define vsuhu   V5

char ssid[] = "182";
char pass[] = "721Makkasau";

int lampu1     =  13,//Relay pin Lampu 1
    lampu2     =  14,//Relay pin Lampu 2
    lampu3     =  12,//Relay pin Lampu 3
    kipas      =  26,//Relay pin Kipas
    saklar1    =  0, //State Saklar1
    saklar2    =  0, //State Saklar2
    saklar3    =  0, //State Saklar3
    saklarPin1 =  2, //Pin Saklar1
    saklarPin2 =  0, //Pin Saklar2
    saklarPin3 =  15;//Pin Saklar3


// DHT ESP32
#include <DHTesp.h>
DHTesp dhtsensor;
TempAndHumidity data;
const int dht_pin=23;


BLYNK_WRITE(vlampu1){              // V0
  int value = param.asInt();
  digitalWrite(lampu1, !value); // Disesuaikan Mau Menghidupkan Relay Mana
  // lcd.setCursor(0,1);
  // lcd.print("Lampu 1 Menyala");
} 
BLYNK_WRITE(vlampu2){              // V1
  int value = param.asInt();
  digitalWrite(lampu2, !value); // Disesuaikan Mau Menghidupkan Relay Mana
  // lcd.setCursor(0,1);
  // lcd.print("Lampu 2 Menyala");
}
BLYNK_WRITE(vlampu3){              // V2
  int value = param.asInt();
  digitalWrite(lampu3, !value); // Disesuaikan Mau Menghidupkan Relay Mana
  // lcd.setCursor(0,1);
  // lcd.print("Lampu 3 Menyala");
}
BLYNK_WRITE(vkipas){              //V3
  int value = param.asInt();
  digitalWrite(kipas, value); // Disesuaikan Mau Menghidupkan Relay Mana
  // lcd.setCursor(0,1);
  // lcd.print("Kipas Menyala");
}
// BLYNK_WRITE(V4){              //V4
//   // int value = param.asInt();
//   int temp = param[0].asDouble();
//   int humd = param[1].asDouble();
  
// }


void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(lampu1,OUTPUT);
  pinMode(lampu2,OUTPUT);
  pinMode(lampu3,OUTPUT);
  pinMode(kipas,OUTPUT);
  pinMode(saklarPin1,INPUT_PULLUP);
  pinMode(saklarPin2,INPUT_PULLUP);
  pinMode(saklarPin3,INPUT_PULLUP);
  dhtsensor.setup(dht_pin, DHTesp::DHT11);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  lcd.print("Connecting");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Okay Ready to Rock Baby");
  lcd.clear();
}

void loop()
{
  Blynk.run();
  data=dhtsensor.getTempAndHumidity();
  float temperature = data.temperature;
  float humidity = data.humidity;
  lcd.setCursor(0,0);
  lcd.print("Temp:  "+String(temperature));
  lcd.setCursor(0,1);
  lcd.print("Humid: "+String(humidity));
  Blynk.virtualWrite(vsuhu, temperature,humidity); // Ganti dengan relay yang ingin dinyalakan
  if(saklar1 != digitalRead(saklarPin1)){ 
    saklar1 = digitalRead(saklarPin1);
    digitalWrite(lampu1,!saklar1);
    Blynk.virtualWrite(vlampu1, !saklar1); // Ganti dengan relay yang ingin dinyalakan
    // Blynk.syncVirtual(vlampu1); // Hanya ubah Virtual Pin yang digunakan
  }
  if(saklar2 != digitalRead(saklarPin2)){
    saklar2 = digitalRead(saklarPin2);
    digitalWrite(lampu2,!saklar2);
    Blynk.virtualWrite(vlampu2, !saklar2);
    // Blynk.syncVirtual(vlampu2);
  }
  if(saklar3 != digitalRead(saklarPin3)){
    saklar3 = digitalRead(saklarPin3);
    digitalWrite(lampu3,!saklar3);
    Blynk.virtualWrite(vlampu3, !saklar3);
    // Blynk.syncVirtual(vlampu3);
  }
  delay(200);
  lcd.clear();
}
