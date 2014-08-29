 
//Deaconu Ioan 342C1
//Tema 1 - IA

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class SocketConnection {

	private int port;
	private InetAddress ipAddress;

	Socket requestSocket;
	DataOutputStream out;
	BufferedReader in;
	String message;
	
	public static final int NODE_FILES_PORT = 8889, NODE_DATA = 8888;
 
	public SocketConnection(int port) {
		this.port = port;
		try {
			this.ipAddress = InetAddress.getByName("127.0.0.1");//"192.168.1.1");//ipAddress.getHostAddress());
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			//e.printStackTrace();
		}
	}
	
	public SocketConnection(int port, String ipAdr) {
		this.port = port;
		try {
			this.ipAddress = InetAddress.getByName(ipAdr);
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			//e.printStackTrace();
		}
	}

	public boolean openSocket() {
		try {
			requestSocket = new Socket(ipAddress, port); 
			System.out.println("Connected to "+ ipAddress.getHostAddress()  +" in port " + port);
			out = new DataOutputStream(  requestSocket.getOutputStream());

			in = new BufferedReader(new InputStreamReader(requestSocket.getInputStream()));

		} catch (Exception exception) {
			//exception.printStackTrace();
		}
		return true;
	}

 
	public boolean sendString(String msg) {
		if(msg.indexOf('\n') == -1) {
			msg +='\n';
		}
		try {
			out.writeBytes(msg);
			
			//System.out.println("sent: " + msg);
		} catch (Exception exception) {
			exception.printStackTrace();
			return false;
		}
		return true;
	}

	public String receiveString() {
		String msg = null;
		try { 
			msg= in.readLine();
			System.out.println("received: " + msg);
		} catch (Exception exception) {
			//exception.printStackTrace();
			msg = null;
		}
		return msg;
	}
	public char[] receiveChars(int size) {
		String msg = null;
		char buffer[] = new char[size];
		try { 
			if(in.read(buffer) <=0) {
				return null;
			}
			System.out.println("received: " + msg);
			return buffer;
		} catch (Exception exception) {
			//exception.printStackTrace();
			msg = null;
		}
		return null;
	}
	public boolean close() {
		try {
			out.close();
			in.close();
			requestSocket.close();
		} catch (Exception exception) {
			//exception.printStackTrace();
			return false;
		}
		return true;
	}
}
