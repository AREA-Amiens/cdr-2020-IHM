#include <Arduino.h>
#include <SPI.h>
#include <MsTimer2.h>
#include <U8g2lib.h>
#include <FlexCan.h>
#include <kinetis_flexcan.h>


#define LED_1       0
#define LED_2       1
#define CAN_TX      2
#define CAN_RX      3
#define ENCODEUR_B  6
#define ENCODEUR_A  7
#define ENCODEUR_BP 5
#define BOUTON_1    8
#define BOUTON_2    9
#define BOUTON_3    10
#define LCD_MOSI    11
#define LCD_MISO    12
#define LCD_SCR     13

#define BUZZER      15

#define LED_RGB_R   20
#define LED_RGB_G   21
#define LED_RGB_B   22

byte ecrant_0();
byte ecrant_1();
int ecrant_1_condition();
byte ecrant_2();
int ecrant_2_condition();
byte ecrant_3();
int ecrant_3_condition();
byte ecrant_4();
int ecrant_4_condition();
void bpInterupEncodeur();
void tournInterupEncodeurRisingA();
void interpretationCan();
int comparChenCan(String comp);
void IntrerrupTimer();
