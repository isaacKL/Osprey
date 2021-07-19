import numpy

ROW_MAX=8

table=[[],[],[]]
leftServo=[]
rightServo=[]
leftMotor=[]
rightMotor=[]
formula={
  -1:"",
  0:"",
  1:""
  }
colLabel=["Yaw (Y)","Pitch (P)","Roll (R)","RightServo","LeftServo","LeftMotor","RightMotor"]


def filltable(table):
  for i in range(ROW_MAX):
    table[0].append(0 if i%4==0 else 1)
    table[1].append(0 if i%3==0 else 1)
    table[2].append(0 if i%2==0 else 1)
  return

def requestResponse(output):
  return



if __name__=="__main__":
    filltable(table)