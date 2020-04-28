 //Osprey Control Unit//
///////////////////////

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
String ssid="Omega";
String pass="cb#75?=1";

int motors[]={0,0};
int data[]={0,0,0,0};
int wings[]={0,0};
int throttle=0;

Servo r_wing,l_wing;

WiFiUDP comms;
IPAddress server=IPAddress(192,168,1,16);

void setup(){

    Serial.begin(115200);
    debug=Serial.available();
    
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
    int data[4];
    if(values){
        getData();
        getMotorValues(data[0],data[3]);
        getWingValues(data[2],data[1]);
    }
    r_wing.write(wings[0]);
    l_wing.write(wings[1]);
    analogWrite(RIGHT_MOTOR,motors[0]);
    analogWrite(LEFT_MOTOR,motors[1]);

}

void getWingValues(int yaw,int pitch){
    //yaw is rotate pitch is forward
    //0 is the right wing, 1 is the left wing
    int vals[2];
    bool LR=(yaw>0);
    yaw*=(LR)?1:-1;
    yaw=map(yaw,0,1000,0,180);
    int temp_pitch=map(abs(pitch),0,1000,0,180);
    int sync=-(temp_pitch-yaw);
    pitch*=(pitch>0);
    pitch=map(pitch,0,1000,0,180);
    vals[0]=((LR)?sync:pitch);
    vals[1]=((LR)?pitch:sync);
    vals[0]=map(vals[0],0,1000,0,180);
    wings[0]=vals[0];
    wings[1]=vals[1];
    
}

int * getMotorValues(int roll, int thrust){
    //roll is right/left 
    int val[2];
    int throttle=throttle+thrust;
    int throttle=clamp(throttle,0,255);
    bool LR=roll>0;
    roll*=(LR)?1:-1;
    int diff=map(roll,0,1000,0,255);
    val[0]=throttle-((LR)?diff:0);
    val[1]=throttle-((LR)?0:diff);
    motors[0]=val[0];
    motors[1]=val[1];
    }

void(* resetFunc) (void) = 0;

void blink(int duration,int period, bool steady){
    bool io=true;
    int loop_count=1000*duration*period;
    for(int i=0;i<loop_count;i++){
        digitalWrite(LED_BUILTIN,io||steady);
        delay(period); 
        io=!io;
    }
}

//Connect to wifi and start udp connection
void connect(){
    int attempts=0;
    WiFi.begin(ssid,pass);
    //
    while(WiFi.status() != WL_CONNECTED){
        if(attempts>15){
            blink(5,150,false);
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
    char bootup[]="Osprey bootup Initiated";
    comms.beginPacket(server,8080);
    comms.write(bootup);
    comms.endPacket();
    delay(100);
    connected=comms.parsePacket()>0;
    }while(!connected);
    //read first packet
    int size = comms.read(data_packet, 256);
    if (size > 0) data_packet[254] = 0;

    if(debug){
        Serial.print("Server response: ");
        Serial.println(data_packet);
    }
    

}

void getData(){
    int size = comms.read(data_packet, 256);
    if (size > 0) data_packet[254] = 0;
    for(int i=0;i<4;i++){
        String value="";
        value+=data_packet[i*4];
        value+=data_packet[1+i*4];
        value+=data_packet[2+i*4];
        value+=data_packet[3+i*4];
        data[i]=value.toInt();
    }
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