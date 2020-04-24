// Ref : http://www.esp8266.com/viewtopic.php?f=28&t=2295&p=13730#p13730

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP port;

char packetBuffer[255];
unsigned int localPort = 8080;


void setup() {
  Serial.begin(115200);
  //WiFi.begin("yourSSID", "your Password");
  WiFi.begin("Omega", "cb#75?=1");
  port.begin(localPort);
//PACO
  Serial.print("[Connecting]: ");
  Serial.println(WiFi.localIP());
}

void loop() {
//RECEPCION
  int packetSize = port.parsePacket();
   Serial.print("Recibiendo(Size/dato): ");
   Serial.print(packetSize);Serial.print(" / ");
  if (packetSize) {
    int len = port.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len-1] = 0;
    Serial.print(packetBuffer);
    //port.beginPacket(port.remoteIP(),port.remotePort());
    //port.write("Your UDP packet was received OK\r\n");
    //port.endPacket();
  }
  Serial.println("");

//ENVIO
    //port.beginPacket("192.168.0.145",9999);
    port.beginPacket("192.168.1.16",8080);
    port.write("Envio millis: ");
    char buf[20];
    unsigned long testID = millis();
    sprintf(buf, "%lu", testID);
    Serial.print("enviando: ");Serial.println(buf);
   
    port.write(buf);
    port.write("\r\n");
    port.endPacket();

 delay(500);
  //PACO
  Serial.print("[Connected] ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}
