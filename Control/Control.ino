#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

// wifi connection variables
const char* ssid = "*****";
const char* password = "*******";
boolean wifiConnected = false;
IPAddress server=IPAddress(192,168,1,16);
// UDP variables
int localPort = 8080;
WiFiUDP UDP;
String ip="192.168.1.16";
boolean udpConnected = false;
char packetBuffer[1024]; //buffer to hold incoming packet,
char ReplyBuffer[] = "Ready"; // a string to send back

void setup() {
  // Initialise Serial connection
  Serial.begin(115200);  
  pinMode(LED_BUILTIN,OUTPUT);
  // Initialise wifi connection
  wifiConnected = connectWifi();
  //only proceed if wifi connection successful
  if(wifiConnected){
    udpConnected = connectUDP();
  }
}


void loop() {
  // check if the WiFi and UDP connections were successful
  if(wifiConnected&&udpConnected){
      // if there’s data available, read a packet
      int packetSize = UDP.parsePacket();
      Serial.println(packetSize);
      if(packetSize){
        Serial.println("New Packet: ");
        // read the packet into packetBufffer
        UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents:");
        int value = packetBuffer[0]*256 + packetBuffer[1];
        Serial.print(value);
        //send a reply, to the IP address and port that sent us the packet we received
        UDP.beginPacket(server, 8080);
        UDP.write(ReplyBuffer);
        UDP.endPacket();
     }
     delay(10);
  }
}

  
  // connect to UDP – returns true if successful or false if not
boolean connectUDP(){
  boolean state = false;
  
  Serial.println("");
  Serial.println("Connecting to UDP");
  UDP.beginPacket(server,8080);
  UDP.write(ReplyBuffer);
  UDP.endPacket();
  if(true){
    Serial.println("Connection successful");
    digitalWrite(LED_BUILTIN,HIGH);
    state = true;
  }
  else{
    Serial.println("Connection failed");
  }
  
  return state;
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1500);
  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
    if (i > 10){
      state = false;
      digitalWrite(LED_BUILTIN,LOW);
      break;
    }
    i++;
  }
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    digitalWrite(LED_BUILTIN,HIGH);
    delay(5000);
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    bool fail=true;
    for(int i=0;i<50;i++){
      digitalWrite(LED_BUILTIN,fail);
      fail=!fail;
      delay(300);
    }
    Serial.println("Connection failed.");
    digitalWrite(LED_BUILTIN,LOW);
  }
  return state;
}
