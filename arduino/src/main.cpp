#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>   //MOSI: 11  MISO: 12  CLK: 13  CS: 4
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

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

SemaphoreHandle_t temp_value_mutex;
SemaphoreHandle_t light_value_mutex;
SemaphoreHandle_t sdCard_mutex;


int old_temperature, new_temperature, old_brightness, new_brightness;
int cooler_speed, cooler_speed_manual;
int led_brightness, led_brightness_manual; 
bool cooler_auto;
bool light_auto;
char message[100];
File sdCard;


const int rs = A4, en = A3, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void semaphore_init(){
  sdCard_mutex = xSemaphoreCreateMutex();
  temp_value_mutex = xSemaphoreCreateMutex();
  light_value_mutex = xSemaphoreCreateMutex();
}

void sdCard_init(){
  Serial.print("SD Card init...");
  if (!SD.begin(4)) {
    Serial.println("init failed...");
    while (1);
  }

  Serial.println("[OK]");

  if (!SD.exists("data.txt")) {
    Serial.println("Creating \"data.txt\"");
  }
}

void write_on_sdCard(char lable, int value){
  char data[100];
  sprintf(data,"%c: %d",lable, value);

  xSemaphoreTake(sdCard_mutex,portMAX_DELAY);
    sdCard = SD.open("data.txt",FILE_WRITE);
    if (sdCard){
      sdCard.println(data);
      sdCard.close();
    }
    else {
      Serial.println("Cannot Open File");
    }
  xSemaphoreGive(sdCard_mutex);
}

void LCD_init() {
  Serial.print("LCD init ...");
  lcd.begin(16, 2);
  Serial.println("[OK]");
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
  Serial.print("LM35 init...");
  pinMode(LM35_PORT, INPUT);
  pinMode(COOLER_PORT, OUTPUT);
  new_temperature = 0;
  old_temperature = -1;
  Serial.println("[OK]");
}

void read_temperature_task(void* params) {
  for (;;){
    Serial.println("T");
    new_temperature = analogRead(LM35_PORT) * 500.0 /1024;
    Serial.println(new_temperature);
    write_on_sdCard('T', new_temperature);
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void cooler_speed_task(void* params) {
  for (;;){
    if (cooler_auto){
      if (new_temperature <= OFF_TEMPERATURE){
        cooler_speed = 0;
      }
      if (new_temperature < 35){
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
      else{
        cooler_speed = 100;
      }
    } 
    else{
      xSemaphoreTake(temp_value_mutex, portMAX_DELAY);
        cooler_speed = cooler_speed_manual;
      xSemaphoreGive(temp_value_mutex); 
    }

    cooler_speed = map(cooler_speed, 0, 100, 0, 255);
    analogWrite(COOLER_PORT, cooler_speed);
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void read_brightness_init(){
  Serial.print("Photocell init...");
  pinMode(PHOTOCELL_1, INPUT);
  pinMode(PHOTOCELL_2, INPUT);
  pinMode(LED_PORT, OUTPUT);
  new_brightness = 0;
  old_brightness = -1;
  Serial.println("[OK]");
}

void read_brightness_task(void* params) {
  for (;;){
    Serial.println("L");
    int value = ((analogRead(PHOTOCELL_1) + analogRead(PHOTOCELL_2)) / 2);
    new_brightness = map(value, 0, 1023, 0, 100) ;
    Serial.println(new_brightness);
    write_on_sdCard('L', new_brightness);
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
        xSemaphoreTake(light_value_mutex, portMAX_DELAY);
          led_brightness = led_brightness_manual;
        xSemaphoreGive(light_value_mutex);
      }

    led_brightness = map(led_brightness,0,100,0,255);
    analogWrite(LED_PORT, led_brightness);
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void set_led_auto_manual(){
  light_auto = !light_auto;
}

void  set_cooler_auto_manual(){
  cooler_auto = !cooler_auto;
}

void dip_switch_init(){
  pinMode(DIP_SW_1, INPUT);  // led
  pinMode(DIP_SW_2, INPUT); // cooler
  attachInterrupt(digitalPinToInterrupt(DIP_SW_1), set_led_auto_manual, RISING);
  attachInterrupt(digitalPinToInterrupt(DIP_SW_2), set_cooler_auto_manual, RISING);
  cooler_auto = true;
  light_auto = true;
}

void recieve_manual_value(void* params){
  for(;;){
    String status;
    if ((!light_auto) || (!cooler_auto)){
      if (Serial.available() > 0){
        status = Serial.readString();
        if (status == "T") {
          if (Serial.available() > 0) {
            int value = Serial.parseInt();
            xSemaphoreTake(temp_value_mutex, portMAX_DELAY);
              cooler_speed_manual = value;
            xSemaphoreGive(temp_value_mutex); 
          }
        }
        else if (status == "L") {
          if (Serial.available() > 0) {
            int value = Serial.parseInt();
            xSemaphoreTake(light_value_mutex, portMAX_DELAY);
            led_brightness_manual = value;
            xSemaphoreGive(light_value_mutex);
          }
        }
      }
    }
    vTaskDelay(TOTAL_DELAY / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}


void setup() {
  Serial.begin(9600);

  semaphore_init();
  LCD_init();
  read_temperature_init();
  read_brightness_init();
  dip_switch_init();
  sdCard_init();


  xTaskCreate(cooler_speed_task, "Cooler Speed Task", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  xTaskCreate(read_temperature_task, "Read Temperture Task", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  xTaskCreate(read_brightness_task, "Read Brightness Task", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  xTaskCreate(LED_brightness_task, "LED Brightness Task", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  xTaskCreate(LCD_showInfo, "Show Info LCD Task", 128, NULL, tskIDLE_PRIORITY + 3, NULL);
  // xTaskCreate(recieve_manual_value, "Recieve temp and light from user", 128, NULL, tskIDLE_PRIORITY + 3, NULL);

}

void loop() {}