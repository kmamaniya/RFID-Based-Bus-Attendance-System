#include <GSM.h>

#define PINNUMBER ""
#define GPRS_APN       "airtelgprs.com" // replace your GPRS APN
#define GPRS_LOGIN     ""    // replace with your GPRS login
#define GPRS_PASSWORD  "" // replace with your GPRS password

GSMClient client;
GPRS gprs;
GSM gsmAccess;

char server[] = "shauniop.localtunnel.me";
char path[] = "/childLocation/?&id=";
int port = 80; // port 80 is 

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  boolean notConnected = true;

  while (notConnected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      notConnected = false;
    } else {
      Serial.print('0');
      delay(1000);
    }
  }
  Serial.write(1);
}


void loop() {
  byte uid[4];
  Serial.flush();
  uint32_t cardid = 0;
  for(int i = 0; i<4; i++){
    while(!Serial.available());
    uid[i] = Serial.read();
    cardid<<=8;
    cardid+=uid[i];
    delay(20);
  }
  Serial.flush();
  Serial.write(3);
  while(!client.connect(server, port)){
    Serial.println("Connecting...");
    delay(1000);
  }
  client.print("POST ");
  client.print(path);
  client.print(cardid);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("User-Agent: Arduino/1.0");
  client.println("Connection: close");
  client.println("Accept: text/html");
  client.println();
  client.stop();
  //Serial.println("Sent");
  Serial.write(2);
  Serial.flush();
  delay(500);
}
