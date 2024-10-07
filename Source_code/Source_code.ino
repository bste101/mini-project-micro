#define BLYNK_TEMPLATE_ID "TMPL6KZJ2FDLW"
#define BLYNK_TEMPLATE_NAME "MailBox"
#define BLYNK_AUTH_TOKEN "18zLTflBG6vPdYhGTAgQdC6MwgSeUSx6"

//#include <MPU9250_asukiaaa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
// #include <WiFiMulti.h>
#include <ArtronShop_LineNotify.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
//#include <MPU9250_asukiaaa.h>
#include <time.h>
#define LINE_TOKEN "fpI2hwezmD19nx0fQsEolJ9hgvfIQFredyD3Ins9xd9"
#define BLYNK_PRINT Serial
#define SW_PIN 23
#define GLED 16
#define RLED 17
#define SDA_PIN 21
#define SCL_PIN 22

// WiFiMulti wifiMulti;

#define OLED_I2C_ADDRESS 0x3c 
//#define MPU_9250_ADDRESS 0x68
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//MPU9250_asukiaaa mySensor(MPU_9250_ADDRESS);
Servo locker;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;  // GMT+7 สำหรับประเทศไทย
const int daylightOffset_sec = 0;

volatile bool SW_Flag = false;
volatile bool ST_Flag = false;
static bool Blynk_Flag = false;
static bool isReply = false;
int servoPin = 4;

char ssid[] = "Best";       
char pass[] = "12345678";

void IRAM_ATTR SW_Press() {
  if (SW_Flag) {
    ST_Flag = true;
  } 
  SW_Flag = true;
}

BLYNK_WRITE(V1) {
  int buttonState = param.asInt(); 
  if (buttonState == 1) {
    Blynk_Flag = true; 
  }
}

void displayInitOled() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Press the button ");
  display.println("to open Mailbox");
  //display.setTextSize(1);
  //display.setTextColor(SH110X_WHITE);
  display.println();
  display.println("====================");
  display.println();
  display.println("Process'll take times");
  display.println("Wait for green LED");
  display.display();
} 

void LightBulbToggle() {
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, HIGH);
  delay(500);
  digitalWrite(GLED, LOW);
  digitalWrite(RLED, HIGH);
  delay(500);
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, HIGH);
}

/*
bool isthedoorOpen() {
  uint8_t sensorId;
  int result;
  float gX;
  result = mySensor.readId(&sensorId);
  result = mySensor.gyroUpdate();
  if (result == 0) {
    gX = mySensor.gyroX();
    if (gX >= 2) {
      return true;
    } else {
      return false;
    }
  }
}
*/

void setup() {
  Serial.begin(115200);
  pinMode(SW_PIN, INPUT);
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  display.begin(OLED_I2C_ADDRESS);
  display.setContrast(0); 
  //display.display();
  //delay(2000);
  //display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 28);
  display.println("Mock Bail");
  display.display();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  //Blynk.run();
  locker.attach(servoPin);
  Wire.begin(SDA_PIN, SCL_PIN);
  //mySensor.setWire(&Wire);
  //mySensor.beginAccel();
  //mySensor.beginGyro();
  //mySensor.beginMag();
  attachInterrupt(digitalPinToInterrupt(SW_PIN), SW_Press, RISING);
  LINE.begin(LINE_TOKEN);

   // ตั้งค่า NTP Server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(2000);
}

void loop() {
  //Blynk.run();
  /*if (Blynk.connected()) {
    Serial.println("Blynk connected");
  } else {
    Serial.println("Blynk not connected");
  }*/
  struct tm timeinfo;
  // สร้างข้อความพร้อมวันที่และเวลาปัจจุบัน
 // สร้างข้อความสำหรับวันที่
  char dateStr[32];
  strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", &timeinfo);  // จัดรูปแบบเป็นวัน-เดือน-ปี

  // สร้างข้อความสำหรับเวลา
  char timeStr[32];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);  // จัดรูปแบบเป็นชั่วโมง:นาที:วินาที

  if (SW_Flag) {
    String message = "มีพัสดุมาส่ง ณ วันที่: ";
    message += dateStr;
    String twomessage = " และเวลา: ";
    twomeesage += timeStr;
    message += twomeesage;
    LINE.send(message);
    if (!Blynk_Flag) {
      Blynk.run();
      LightBulbToggle();
    }
    // Blynk get data input for servo lock
    if (Blynk_Flag) {
      // line Notification
      digitalWrite(GLED, HIGH);
      digitalWrite(RLED, LOW);
      locker.write(90);

      /*display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(0, 0);
      display.println("Green Door Open!");
      display.display();*/
      /*for (int i = 0; i < 120; i++) {
        delay(1500);
        Blynk.run();
      }*/
      for (int i = 180; i >= 0; i--) {
        display.clearDisplay();                
        display.setTextSize(1);             
        display.setTextColor(SH110X_WHITE);  
        display.setCursor(0, 0);     
        display.println("Green Door Open!");         

        // Calculate minutes and seconds
        int minutes = i / 60;                   
        int seconds = i % 60;                 

        // Print time left in minutes:seconds format
        display.println("Time left: ");
        display.setTextSize(2);                
        display.setCursor(40, 30);           
        display.printf("%d:%02d", minutes, seconds); 
        display.display();   
               
        delay(1000);                        
        Blynk.run();  
        if (ST_Flag) {
          
          ST_Flag = false;
          Blynk.run(); 
          break;
        }                     
      }
      locker.write(0);
      Blynk.virtualWrite(V1, 0);
      digitalWrite(GLED, LOW);
      digitalWrite(RLED, HIGH);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(10, 20);
      display.println(" Door");
      display.println("Locked!");
      display.display();
      String message = "ประตูปิด ณ วันที่: ";
      message += dateStr;
      String twomessage = " และเวลา: ";
      twomeesage += timeStr;
      message += twomeesage;
      LINE.send(message)
      ST_Flag = false;
      SW_Flag = false;
      Blynk_Flag = false;
      // send Line Notification
      delay(3000);
    }
  } else {
    digitalWrite(GLED, LOW);
    digitalWrite(RLED, LOW);
    displayInitOled();
    // send data to Blynk
  }
}
