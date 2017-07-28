#include <Wire.h>
#include <Adafruit_PN532.h>
#include <LiquidCrystal.h>

#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
LiquidCrystal lcd(6,7, 8, 9, 10, 11); 

void setup(void) {

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("LCD initialized");
  
  Serial.begin(9600);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    lcd.clear();
    lcd.print("RFID Error");
    while (1); // halt
  }

  while(Serial.read()!=1);
  Serial.flush();
  lcd.clear();
  lcd.print("Internet Connected");
  delay(1000);

  nfc.SAMConfig();
  lcd.clear();
  lcd.print("RFID Start...");
  delay(1000);

}

void transmit(uint8_t uid[]){
  Serial.write(uid[0]);
  delay(20);
  Serial.write(uid[1]);
  delay(20);
  Serial.write(uid[2]);
  delay(20);
  Serial.write(uid[3]);
  delay(20);
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  lcd.clear();
  lcd.print("Put a card");
  delay(1000);  
  while(!nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength));
  lcd.clear();
  lcd.print("Found a card:");
  lcd.setCursor(0,1);
  delay(1000);
  uint32_t cardid = 0;
  for(int i=0; i<4; i++){
    cardid <<= 8;
    cardid |= uid[i];
  }
  lcd.print(cardid);
  transmit(uid);
  while(Serial.read()!=3);
  Serial.flush();
  lcd.clear();
  lcd.print("Sending..");
  while(Serial.read()!= 2);
  Serial.flush();
}

