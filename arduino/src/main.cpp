#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <Arduino_FreeRTOS.h>

#define MOTOR_PORT 6
#define LED_PORT 5

#define LM35_PORT A2
#define PHOTOCELL_1 A0
#define PHOTOCELL_2 A1
#define DIP_SW_1 2
#define DIP_SW_2 3

#define UART_DELAY 20000   //2 seconds
#define OFF_TEMPERATURE 25


char message[100];


const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void LCD_init() {
  lcd.begin(16, 2);
}

void LCD_showInfo(void* params) {
  for (;;){
    // sprintf(message, "T:%d S:%d E:%d", new_temperture, new_motor_speed, current_error_code_eeprom());
    lcd.clear();
    lcd.println(message);
    // vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}






void setup() {
  Serial.begin(9600);

  LCD_init();


  pinMode(MOTOR_PORT,OUTPUT);
  pinMode(LED_PORT, OUTPUT);

  pinMode(LM35_PORT, INPUT);
  pinMode(PHOTOCELL_1, INPUT);
  pinMode(PHOTOCELL_2, INPUT);
  pinMode(DIP_SW_1, INPUT);
  pinMode(DIP_SW_2, INPUT);

}

void loop() {}