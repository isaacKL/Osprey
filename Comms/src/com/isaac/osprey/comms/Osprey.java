package com.isaac.osprey.comms;

import java.awt.Canvas;

import org.lwjgl.input.Controller;

public class Osprey {
	
	
	UDP udp;
	Controller controller1,controller2;
	boolean run=false;
	ControllerHandler c_handler;
	//TODO loop till received, get name and ip
	//loop send data check received for error
	//XxxX
	
	public Osprey() {
		udp = new UDP();
		c_handler= new ControllerHandler();
		System.out.println("Connecting....");
		int tries=0;
		udp.open();			
		System.out.println((udp.connected())?"Connected":"Error");
		do {
			udp.recieve();
		}while(!udp.hasSubscribers());
		System.out.println("Obtained subscribers");
		
		
	}
	
	
	public void main() {
		while(run) {
			for(int i=0;i<c_handler.getControllers();i++) {
				byte[] data=c_handler.get(i);
				udp.send(data, i);
			}
		}
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		new Osprey();
	}
	

}
