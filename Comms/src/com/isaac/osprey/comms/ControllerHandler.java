package com.isaac.osprey.comms;

import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import org.lwjgl.LWJGLException;
import org.lwjgl.input.Controller;
import org.lwjgl.input.Controllers;

public class ControllerHandler implements ComponentListener {

	private Controller controller;
	
	
	public static boolean start=false;
	private int connected=0,
			indexA=-1,
			indexB=-1;
	
	
	public ControllerHandler(Controller a,Controller b){
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
			if(getController().getName().contains("controller") && getController().getAxisCount()>2) {
				indexB=(indexA!=-1)?i:-1;
				indexA=(indexA==-1)?i:indexA;
			}
		}
		/*for(int i=0;i<getController().getAxisCount();i++){
			//System.out.println(i+": "+getController().getAxisName(i));
			//controller.setDeadZone(i,0);
		}
		//controller.setRumblerStrength(1, (float) 1.0);
		for(int i=0;i<getController().getButtonCount();i++){
		//	System.out.println(i + ": " +controller.getButtonName(i));
		}*/
		
	}
	
	public  void update(Controller current){
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
	
	public byte[] get(int temp) {
		String csv="$G;";
		for(int i=0;i<controller(temp).getAxisCount();i++){
			float value=controller(temp).getAxisValue(i);
			csv+=Float.toString(value);
		}
		return csv.getBytes();
	}
	
	private Controller controller(int temp) {
		int index=(temp==connected)?indexB:indexA;
		return Controllers.getController(index);
	}
	
	public int getControllers() {
		return (connected==0)?1:connected;
	}
}


