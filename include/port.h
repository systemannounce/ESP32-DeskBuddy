#pragma once

// VERSION
#define __DB_VERSION__ "0.1"

// CONTROL PRESS
#define BUTTON	13

// LED
#define LED_R	12
#define LED_G	14
#define LED_B	27

// HX711
#define HX711_SCK	33
#define HX711_DT	32

// SR501
#define SR501	34

// 光敏电阻
#define AO	36
#define DO	39

// LCD
#define LCD_SCL 18		//SCLK
#define LCD_SDA 23		//MOSI
#define LCD_RES 15		//RES
#define LCD_DC  2		//DC
#define LCD_CS  5		//CS
#define LCD_BLK 17		//BLK

//-----------------LCD端口定义----------------
#define LCD_SCLK_Clr() digitalWrite(LCD_SCL, LOW)	//SCL=SCLK
#define LCD_SCLK_Set() digitalWrite(LCD_SCL, HIGH)

#define LCD_MOSI_Clr() digitalWrite(LCD_SDA, LOW)	//SDA=MOSI
#define LCD_MOSI_Set() digitalWrite(LCD_SDA, HIGH)

#define LCD_RES_Clr() digitalWrite(LCD_RES, LOW)	//RES
#define LCD_RES_Set() digitalWrite(LCD_RES, HIGH)

#define LCD_DC_Clr() digitalWrite(LCD_DC, LOW)	//DC
#define LCD_DC_Set() digitalWrite(LCD_DC, HIGH)

#define LCD_CS_Clr()  digitalWrite(LCD_CS, LOW)	//CS
#define LCD_CS_Set()  digitalWrite(LCD_CS, HIGH)

#define LCD_BLK_Clr()  digitalWrite(LCD_BLK, LOW)	//BLK
#define LCD_BLK_Set()  digitalWrite(LCD_BLK, HIGH)