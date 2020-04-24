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
		udp = new UDP(8080);
		c_handler= new ControllerHandler(controller1,controller2);
		System.out.println("Connecting....");
		int tries=0;
		udp.open();			
		System.out.println((udp.connected())?"Connected":"Error");
		do {
			udp.recieve();
		}while(!udp.hasSubscribers());
		System.out.println("Obtained subscribers");
		byte[] data="Ready back".getBytes();
		while (true) {
			udp.send(data, 0);
		}		
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
