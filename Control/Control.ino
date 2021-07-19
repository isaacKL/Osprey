

 //Osprey Control Unit//
///////////////////////

#include <ESP8266WiFi.h>
#include <WIFIUDP.h>
#include <Wire.h>
#include "Servo.h"
#include <string.h>

#ifndef MOTORS
#define LEFT_MOTOR 3
#define RIGHT_MOTOR 1
#define RIGHT_WING 6
#define LEFT_WING 7

#endif
float gyro_raw[] = {0,0,0};
float gyro_angle[] = {0,0,0};

boolean debug=false;

int conn,loops=0;

bool off=false;
const int packetSize=255;

char data_packet[packetSize];
const char* ssid = "Grannygray";
const char* pass = "grace1953";

int motors[]={0,0};
int data[]={0,0,0,0};
int wings[]={20,20};
int throttle=0;
int port = 8192;
WiFiUDP comms;

Servo r_wing,l_wing;

IPAddress server(192,168,1,217);


void setup(){

    Serial.begin(9600);
    debug=(Serial.available())?true:true;
    if(debug)Serial.println("Initializing");
    //Onboard indicator LED
    pinMode(13,OUTPUT);

    //WIFI initiation
    connec();
    //bootup pin initialization
    pinMode(LEFT_MOTOR,OUTPUT);
    pinMode(RIGHT_MOTOR,OUTPUT);
    //pinMode(2,OUTPUT);
    //r_wing.attach(RIGHT_WING);
    //l_wing.attach(LEFT_WING);

    //r_wing.write(90);
    //l_wing.write(90);
    
}
 
void loop(){
  
    off=!off;
    int values=comms.parsePacket();
    Serial.println(values);
    if(values>0){
        getData();
        getMotorValues(data[0],data[3]);
        getWingValues(data[2],data[1]);
    }
    /*
    r_wing.write(wings[0]);
    l_wing.write(wings[1]);
    //digitalWrite(2,off);
    analogWrite(RIGHT_MOTOR,motors[0]);
    analogWrite(LEFT_MOTOR,motors[1]);
    */
    char bootup[]="Thanks";
    comms.beginPacket(server,port);
    comms.write(bootup);
    comms.endPacket();
    delay(1000);
    
}

void getWingValues(int yaw,int pitch){
    //yaw is rotate pitch is forward
    //0 is the right wing, 1 is the left wing
    int vals[2];
    bool LR=(yaw>0);
    yaw*=(LR)?1:-1;
    yaw=map(yaw,0,1000,0,180);
    int temp_pitch=map(abs(pitch),0,1000,0,180);
    int sync=-(yaw-temp_pitch);
    pitch*=(pitch>0);
    pitch=map(pitch,0,1000,0,180);
    vals[0]=((LR)?sync:pitch);
    vals[1]=((LR)?pitch:sync);
    vals[0]=map(vals[0],0,1000,0,180);
    wings[0]=vals[0];
    wings[1]=vals[1];
    
}

void getMotorValues(int roll, int thrust){
    //roll is right/left 
    int val[2];
    throttle=throttle+thrust;
    throttle=clamp(throttle,0,255);
    bool LR=roll>0;
    roll*=(LR)?1:-1;
    int diff=map(roll,0,1000,0,255);
    val[0]=throttle-((LR)?diff:0);
    val[1]=throttle-((LR)?0:diff);
    motors[0]=val[0];
    motors[1]=val[1];
    }

void(* resetFunc) (void) = 0;

void blinc(int duration,int period, bool steady){
    bool io=true;
    int loop_count=duration;
    for(int i=0;i<loop_count;i++){
        digitalWrite(13,io||steady);
        delay(period); 
        io=!io;
    }
}

//Connect to wifi and start udp connection
void connec(){
    int attempts=0;
    WiFi.begin(ssid,pass);
    //
    
    delay(1000);
    if(debug) Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED){
        if(attempts>15){
            blinc(5,150,false);
            resetFunc();
            break;
        }
        attempts++;
        delay(100);
    }
    attempts=0;
    //Debug output
    if(debug){
    Serial.print("Connected to: ");
    Serial.println(ssid);
    Serial.print("IPAddress: ");
    Serial.println(WiFi.localIP());
    }
    //start udp communication.
    comms.begin(port);
    Serial.print("UDP Begin ");
    do{
     Serial.print(".");
    char bootup[]="Osprey bootup Initiated";
    comms.beginPacket(server,port);
    comms.write(bootup);
    comms.endPacket();
    delay(100);
    conn=comms.parsePacket();
    }while(!conn);
    //read first packet
    int siz = comms.read(data_packet, packetSize);
    /*if (siz > 0){
     data_packet[siz] = 0;
    }*/
    if(debug){
        Serial.println();
        Serial.println("Server response: ");
        delay(250);
        Serial.printf("%s\n",data_packet);
        delay(250);
    }
    

}

void getData(){
    
    int siz = comms.read(data_packet, packetSize);
    if (siz > 0) {data_packet[siz] = '0';}
    char * v;
    data[0]=atoi(strtok(data_packet,";"));
    data[1]=atoi(strtok(NULL,";"));
    data[2]=atoi(strtok(NULL,";"));
    data[3]=atoi(strtok(NULL,";"));
    
    comms.beginPacket(server,port);
    comms.write(strtok(data_packet,";"));
    comms.endPacket();
    delay(1000);
}

int clamp(int var,int min, int max){
    if (var>=max){
        return var=max;
    }else if (var<=min){
        return var=min;
    }else{
        return var;
    }
}
