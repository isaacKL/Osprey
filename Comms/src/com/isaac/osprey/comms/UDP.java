package com.isaac.osprey.comms;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.SocketException;

public class UDP {

	
	private DatagramSocket socket;
	
	String[] ip;
	InetAddress subscribers[]= new InetAddress[2];
	private int port;
	
	
	public UDP(int port) {
		setPort(port);
	}
	
	public UDP() {
		port=8192;
	}
	
	public void open() {
		try {
			socket=new DatagramSocket(port);		
		}catch(SocketException e) {
			if(e.getMessage()=="Address already in use: Cannot bind") {
				System.out.println('.');
				socket.close();
				System.exit(1);
			}
		}
	}
	
	public boolean hasSubscribers() {
		return subscribers.length>0;
	}
	//TODO FIX
	public boolean reconnect(String ip) {
		try {
			socket=new DatagramSocket(port);
		}catch(SocketException e) {
			return false;
		}
		return true;
	}
	public void close() throws SocketException{
		this.socket.close();
	}
	
	public void recieve() {
		byte[] data=new byte[1024];
		try {	
			DatagramPacket packet= new DatagramPacket(data, data.length);
			socket.receive(packet);
			String msg= new String(packet.getData());
			System.out.println(msg);
			if(msg.length()>0) {
				subscribers[0]=packet.getAddress();
				port=packet.getPort();
				send("Bootup Notice Recieved".getBytes(),0);
			}else if(msg.length()>0){
				//log or something maybe battery.
			}
		} catch (IOException e) {
			e.printStackTrace();		
		}
	}
	
	public boolean connected() {
		//return socket.isConnected();
		return true;
	}
	public void setIp(String ip) {
		//subscribers[(subscribers.length==0)?0:subscribers.length-1]=new InetSocketAddress(ip,port);
		//subscribers[0]=new InetSocketAddress(ip,port);
	}
	
	private void setPort(int port) {
		this.port=port;
	}	

	public int send(byte[] data,int pair) {
		InetAddress temp=subscribers[pair];
		try {	
			DatagramPacket packet= new DatagramPacket(data, data.length,temp,port);
			socket.send(packet);
		} catch (IOException e) {
			e.printStackTrace();
		}
		return 0;
	}
}
