 //Osprey Control Unit//
///////////////////////

#include <MPU3060 .h>
#include <Serial.h>
#include <ESP8266WiFi.h>
#include <WIFIUDP.h>
#include <Wire.h>
#include <Servo.h>


#define LEFT_MOTOR 3
#define RIGHT_MOTOR 2
#define RIGHT_WING 1
#define LEFT_WING 4

float gyro_raw[3] = {0,0,0};
float gyro_angle[3] = {0,0,0};

boolean debug=false;
boolean connected=false;

char data_packet[256];

int motors={0,0};
int wings={0,0};
int throttle=0;

Servo r_wing,l_wing;


void setup(){

    Serial.begin(115200);
    debug=(Serial.available())?true:false;
    
    //Onboard indicator LED
    pinMode(LED_BUILTIN,HIGH);
    //WIFI initiation
    connect();
    //bootup pin initialization
    pinMode(LEFT_MOTOR,OUTPUT);
    pinMode(RIGHT_MOTOR,OUTPUT);
    r_wing.attach(RIGHT_WING);
    l_wing.attach(LEFT_WING);
    
}
 
void loop(){
    int values=comms.parsePacket();
    
    if(values){
        int[] data=getData();
        motors=getMotorValues(data[0],data[3]);
        wings=getWingValues(data[2],data[1]);
    }
    r_wing.write(wings[0]);
    l_wing.write(wings[1]);
    analogWrite(RIGHT_MOTOR,motors[0]);
    analogWrite(LEFT_MOTOR,motors[1]);

}

int[] getWingValues(int yaw,int pitch){
    //yaw is rotate pitch is forward

}

int[] getMotorValues(int roll, int thurst){
    //roll is right/left 
    int throttle+=thrust;
    int throttle=clamp(throttle,0,255);
    
    roll=map(roll,0,1000,0,255);
    int r=(roll>0)?roll:throttle;
    int l=(roll<0)?roll:throttle;
    
}

void(* resetFunc) (void) = 0;

void blink(int duration,int period, bool steady){
    bool io=true;
    int loop_count=1000*duration*period
    for(int i=0;i<loop_count;i++){
        digitalWrite(LED_BUILTIN,io||steady);
        delay(period); v 
        io=!io;
    }
}

//Connect to wifi and start udp connection
void connect(){
    int attempts=0;
    Wifi.begin(ssid,pass);
    //
    while(WiFi.status() != WL_CONNECTED){
        if(attempts>15){
            blink(,150,false);
            resetFunc();
        }
        attempts++;
    }
    attempts=0;
    //Debug output
    if(debug){
    Serial.print("Connected to: ");
    Serial.println(ssid);
    Serial.print(" IPAddress: ");
    Serial.println(WiFi.localIP());
    }
    //start udp communication.
    do{
    char bootup[]='Osprey bootup Initiated';
    comms.beginPacket(server,8080);
    comms.write(bootup);
    comms.endPacket();
    delay(100);
    connected=comms.parsePacket()>0;
    }while(!connected);
    //read first packet
    int size = comms.read(data_packet, 256);
    if (size > 0) data_packet[len-1] = 0;

    if(debug){
        Serial.print("Server response: ");
        Serial.println(data_packet);
    }
    

}

int[] getData(){
    int size = comms.read(data_packet, 256);
    if (size > 0) data_packet[len-1] = 0;
    int data[4];
    for(int i=0;i<4;i++){
        String value='';
        value+=data_packet[i*4];
        value+=data_packet[1+i*4];
        value+=data_packet[2+i*4];
        value+=data_packet[3+i*4];
        data[i]=value.toInt();
    }
    return data;
}
