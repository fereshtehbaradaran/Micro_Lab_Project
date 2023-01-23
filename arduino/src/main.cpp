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
int cooler_speed, cooler_speed_manual;
int led_brightness, led_brightness_manual; 
bool cooler_auto;
bool light_auto;
char message[100];
char data[100];


const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void LCD_init() {
  lcd.begin(16, 2);
}

void LCD_showInfo(void* params) {
  for (;;){
    sprintf(message, "Temp:%d Light:%d", new_temperature, new_brightness);
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

void cooler_speed_task(void* params) {
  for (;new_temperature <= OFF_TEMPERATURE;){
    if (cooler_auto){
      if (new_temperature >= 30 && new_temperature < 35){
        cooler_speed = 30;
      }
      else if (new_temperature < 40 ){
        cooler_speed = 50;
      }
      else if (new_temperature < 45){
        cooler_speed = 70;
      }
      else if (new_temperature < 50){
        cooler_speed = 100;
      }
    } 
    else{
      cooler_speed = cooler_speed_manual;
    }

    analogWrite(COOLER_PORT, cooler_speed);
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  analogWrite(COOLER_PORT, 0);
  vTaskDelete(NULL);
}

void read_brightness_init(){
  pinMode(PHOTOCELL_1, INPUT);
  pinMode(PHOTOCELL_2, INPUT);
  pinMode(LED_PORT, OUTPUT);
  new_brightness = 0;
  old_brightness = -1;
}

void read_brightness_task(void* params) {
  for (;;){
    int value = ((analogRead(PHOTOCELL_1) + analogRead(PHOTOCELL_2)) / 2);
    new_brightness = map(value, 0, 1023, 0, 100) ;
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void LED_brightness_task(void* params){
  for (;;){
    if (light_auto){
      if (new_brightness < 25){
        led_brightness = 100;
      }
      else if (new_brightness < 50 ){
        led_brightness = 75;
      }
      else if (new_brightness < 75){
        led_brightness = 50;
      }
      else if (new_brightness < 100){
        led_brightness = 25;
      }
    } 
    else{
      led_brightness = led_brightness_manual;
    }
    analogWrite(LED_PORT, led_brightness);
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void dip_switch_init(){
  pinMode(DIP_SW_1, INPUT);
  pinMode(DIP_SW_2, INPUT);
  cooler_auto = true;
  light_auto = true;
}

void set_auto_manual(void* params){
    for (;;){
      int dS1 = digitalRead(DIP_SW_1); // led
      int dS2 = digitalRead(DIP_SW_2); // cooler

      if (dS1){light_auto = false;} else {light_auto = true;}
      if (dS2){cooler_auto = false;} else {cooler_auto = true;}

      vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void send_status_to_GUI(void* params){
  for(;;){
    sprintf(data, "%d %d", new_temperature, new_brightness);
    Serial.print(data);
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void recieve_manual_value(void* params){
  for(;;){
    String status;
    if (Serial.available() > 0){
      status = Serial.readString();
      if (status == "T") {
        if (Serial.available() > 0) {
          cooler_speed_manual = Serial.parseInt();
        }
      }
      else if (status == "L") {
        if (Serial.available() > 0) {
          led_brightness_manual = Serial.parseInt();
        }
      }

    }
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}


void setup() {
  Serial.begin(9600);

  LCD_init();
  read_temperature_init();
  read_brightness_init();
  dip_switch_init();


  xTaskCreate(cooler_speed_task, "Cooler Speed Task", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  xTaskCreate(read_temperature_task, "Read Temperture Task", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  xTaskCreate(read_brightness_task, "Read Brightness Task", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  xTaskCreate(LED_brightness_task, "LED Brightness Task", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  xTaskCreate(LCD_showInfo, "Show Info LCD Task", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  // xTaskCreate(set_auto_manual, "Set auto or manual Task", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  xTaskCreate(send_status_to_GUI, "Send temperature and brightness to GUI", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  xTaskCreate(recieve_manual_value, "Recieve temp and light from user", 128, NULL, tskIDLE_PRIORITY + 3, NULL);

}

void loop() {}