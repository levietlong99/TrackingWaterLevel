#ifndef ESP8266_FIRE_BASE_H_
#define ESP8266_FIRE_BASE_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>                // Thư viện cần có để chọn board ESP8266, Link để prefrence board
#include <LiquidCrystal_I2C.h>            // Thư viện này đính kèm ở file bên ngoài
#include <FirebaseArduino.h>              // Include thư viện FirebaseArduino trong file đính kèm
#include <EEPROM.h>
#include <String.h>
#include <ArduinoJson.h>

#define USER_NAME       "AndroidAP"
#define PASS_WORD       "ttt0611t"
#define FIRE_BASE_HOST  "vietlongpro1999-default-rtdb.firebaseio.com"
#define FIRE_BASE_AUTH  ""
#define PHONE_NUMBER    "+84335394634"

#define ABOVE_LIMIT_1     14
#define BELOW_LIMIT_1     36
#define ABOVE_LIMIT_2     15.5
#define BELOW_LIMIT_2     33
#define ABOVE_LIMIT_3     18
#define BELOW_LIMIT_3     30

#define M_TRACKING_0      0x00
#define M_TRACKING_1      0x01
#define M_PUMP            0x02
#define M_FAUCET          0x03
#define M_MESSAGE_STATUS  0x04
#define M_WATER_LEVEL     0x05
#define M_SIREN_STATUS    0x06

#define SIREN_PIN           13
#define PUMP_PIN            14
#define FAUCET_PIN          12
#define PUMP_BUTTON         10
#define FAUCET_BUTTON       15
#define LCD_ADDRESS       0x27
#define LCD_SIZE          2004

extern LiquidCrystal_I2C lcd;
extern float water_level;
extern volatile int pump;   // extern volatile uint8_t pump
extern volatile int faucet; // extern volatile uint8_t faucet
extern volatile bool new_button;

void lcd_init();
void setup_wifi_and_firebase();
void read_EEPROM();

void get_data_from_firebase();
void lcd_display(); 
void push_data_to_firebase(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t days, uint8_t months, uint16_t years);
void write_EEPROM();
void handle_warning();
void send_message();

#endif
