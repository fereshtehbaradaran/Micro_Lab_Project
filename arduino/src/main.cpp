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


void setup() {
  pinMode(MOTOR_PORT,OUTPUT);
  pinMode(LED_PORT, OUTPUT);

  pinMode(LM35_PORT, INPUT);
  pinMode(PHOTOCELL_1, INPUT);
  pinMode(PHOTOCELL_2, INPUT);
  pinMode(DIP_SW_1, INPUT);
  pinMode(DIP_SW_2, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
}