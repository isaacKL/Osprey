package com.isaac.osprey.comms;

import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import org.lwjgl.input.Controller;
import org.lwjgl.input.Controllers;
import org.lwjgl.LWJGLException;

public class ControllerHandler implements ComponentListener {

	private Controller controller;
	
	
	public static boolean start=false;
	private int connected=0,
			indexA=-1;
	
	
	public ControllerHandler(){
		// TODO Auto-generated method stub
		try {
			Controllers.create();
		} catch (LWJGLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Controllers.poll();
		
		for(int i=0;i<Controllers.getControllerCount();i++){
			setController(Controllers.getController(i));
			System.out.println(getController().getName());
			//System.out.println(controller.getName());
			if(controller.getName().contains("xbox")) {
				indexA=i;
				break;
			}
		}
		System.out.println(getController().getAxisCount());
		
		/*if(indexA!=-1) {
			a=Controllers.getController(indexA);
		}else if(indexB!=-1) {
			b=Controllers.getController(indexB);
		}*/
		
		/*for(int i=0;i<getController().getAxisCount();i++){
			//System.out.println(i+": "+getController().getAxisName(i));
			//controller.setDeadZone(i,0);
		}
		//controller.setRumblerStrength(1, (float) 1.0);
		for(int i=0;i<getController().getButtonCount();i++){
		//	System.out.println(i + ": " +controller.getButtonName(i));
		}*/
		
	}
	
	private  void update(Controller current){
		current.poll();
	//	start=controller.isButtonPressed(1);{
		//	System.out.println(start);
	//		System.out.println(controller.getAxisValue(0));
		//	System.out.println(controller.getAxisValue(1));
			//System.out.println(controller.getAxisValue(2));
			//System.out.println(controller.getAxisValue(3));
			//System.out.println(controller.getAxisValue(4));
		//System.out.println();
	}
	
	@Override
	public void componentHidden(ComponentEvent arg0) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void componentMoved(ComponentEvent arg0) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void componentResized(ComponentEvent arg0) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void componentShown(ComponentEvent arg0) {
		// TODO Auto-generated method stub
		
	}
	private void setController(Controller temp) {
		this.controller=temp;
	}
	private Controller getController() {
		return this.controller;
	}
	
	public byte[] get() {
		
		String csv="";
		update(controller);
		
		int x=(int)(controller.getXAxisValue()*1000);
		int y=(int)(controller.getYAxisValue()*1000);
		int z=(int)(controller.getZAxisValue()*1000);
		int t=(int)(controller.getRXAxisValue()*1000);		
			csv=Integer.toString(x)+Integer.toString(y)+Integer.toString(z)+Integer.toString(t);
		//System.out.println(csv);
		return csv.getBytes();
	}
	
	
	
	public int getControllers() {
		return (connected==0)?1:connected;
	}
}


