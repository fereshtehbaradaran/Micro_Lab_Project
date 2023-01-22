#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <Arduino_FreeRTOS.h>

#define COOLER_PORT 6
#define LED_PORT 5

#define LM35_PORT A2
#define PHOTOCELL_1 A0
#define PHOTOCELL_2 A1
#define DIP_SW_1 2
#define DIP_SW_2 3

#define UART_DELAY 20000   //2 seconds
#define OFF_TEMPERATURE 25
#define TOTAL_DELAY 300


int old_temperature, new_temperature, old_brightness, new_brightness;
bool Auto = true;
char message[100];


const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void LCD_init() {
  lcd.begin(16, 2);
}

void LCD_showInfo(void* params) {
  for (;;){
    sprintf(message, "T:%d L:%d", new_temperature, new_brightness);
    lcd.clear();
    lcd.println(message);
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void read_temperature_init() {
  pinMode(LM35_PORT, INPUT);
  pinMode(COOLER_PORT, OUTPUT);
  new_temperature = 0;
  old_temperature = -1;
}

void read_temperature_task(void* params) {
  for (;;){
    new_temperature = analogRead(LM35_PORT) * 500.0 /1024;
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}


void set_motor_speed_task(void* params) {
  for (;new_temperature <= OFF_TEMPERATURE;){
    int motor_speed = 30;
    if (new_temperature >= 30 && new_temperature < 35){
        motor_speed = 30;
    }
    else if (new_temperature < 40 ){
        motor_speed = 50;
    }
    else if (new_temperature < 45){
        motor_speed = 70;
    }
    else if (new_temperature < 50){
        motor_speed = 100;
    }

    analogWrite(COOLER_PORT, motor_speed);
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  analogWrite(COOLER_PORT, 0);
  vTaskDelete(NULL);
}



void setup() {
  Serial.begin(9600);

  LCD_init();
  read_temperature_init();



}

void loop() {}