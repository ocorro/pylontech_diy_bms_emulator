// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13

#include <SPI.h>
#include <LiquidCrystal_I2C.h>//Libreria para LCD I2C
#include <Wire.h>//libreria requrida para usar SDA y SCL

#define CAN_2515
// #define CAN_2518FD

// Set SPI CS Pin according to your hardware

#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;
const int CAN_INT_PIN = 2;
#endif


#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif                           

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int num = 0;

void setup(void) {
  SERIAL_PORT_MONITOR.begin(115200);
  //pinMode(SPI_CS_PIN, OUTPUT);    // sets the digital pin 13 as output
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {             // init can bus : baudrate = 500k
      SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
      delay(100);
  }
  SERIAL_PORT_MONITOR.println("CAN init ok!");

  //LCD init 
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
}


void loop() {
  SERIAL_PORT_MONITOR.println("inicio");
  delay(500);
  lcd.home(); 
  lcd.print("indice ");
  lcd.print(num); 
  delay(500);
  
    unsigned char len = 0;
    unsigned char buf[8];
    //digitalWrite(SPI_CS_PIN, LOW);
    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        num++; 
        lcd.clear();
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned long canId = CAN.getCanId();

        SERIAL_PORT_MONITOR.println("-----------------------------");
        SERIAL_PORT_MONITOR.print("dta ID: 0x");
        SERIAL_PORT_MONITOR.println(canId, HEX);

        lcd.setCursor(0, 1); 
        lcd.print("ID: 0x");
        lcd.println(canId, HEX);


        lcd.setCursor(0, 2);
        for (int i = 0; i < len; i++) { // print the data
            SERIAL_PORT_MONITOR.print(buf[i], HEX);
            SERIAL_PORT_MONITOR.print("\t");
            lcd.print(buf[i], HEX);
            lcd.print(" ");

        }
        
        SERIAL_PORT_MONITOR.println("recibdo");
    }
    SERIAL_PORT_MONITOR.println("fin");
}
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
