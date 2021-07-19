import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d 
import time
# evenly sampled time at 200ms intervals

wings=[0,0]

def valmap(value, istart, istop, ostart, ostop):
  return ostart + (ostop - ostart) * ((value - istart) / (istop - istart))

def getWingValues( yaw:int, pitch:int):
    #yaw is rotate pitch is forward
    #0 is the right wing, 1 is the left wing
    vals=[]
    global wings
    yaw+=pitch
    LR=(yaw>0)
    yaw*=1 if LR else -1
    yaw=(9/100)*yaw+90
    
    yaw=valmap(yaw,0,1000,0,180)
    temp_pitch=valmap(-1000,0,1000,0,180)
    sync=yaw
    pitch*=(pitch>0)
    pitch=valmap(pitch,0,1000,0,180)
    
    vals.append(sync if LR else -sync)
    vals.append(-sync if LR else sync)
    

    #vals[0]=valmap(vals[0],0,1000,0,180)
    #vals[1]=valmap(vals[1],0,1000,0,180)

    wings[0]=vals[0]
    wings[1]=vals[1]
    
x = np.arange(-250,250)
z = np.arange(-250,250)

serv=np.arange(0,180)
y=[]
y1=[]
for i in range(len(x)):
    for j in range(len(z)):

      getWingValues(x[i],z[j])
      y.append(wings[0])
      y1.append(wings[1])

ax=plt.axes(projection="3d")
ax.scatter3D( x,z,z,cmap="Grens")

plt.axis([-1000,1000,0,180])
plt.ylabel("TOP")
plt.show()
time.sleep(10)        
# red dashes, blue squares and green triangles
'''
plt.plot(x, y, 'r--')
plt.show()'''