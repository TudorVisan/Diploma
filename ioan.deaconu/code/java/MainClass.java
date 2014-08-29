//Deaconu Ioan 342C1
//Tema 1 - IA

import java.util.HashMap;

public class MainClass {

	private static SocketConnection socketConnection;
	 
 
	
	public static void main(String args[]) { 
		socketConnection = new SocketConnection(SocketConnection.NODE_DATA);
		
		socketConnection.openSocket();
 		while(true){
			socketConnection.receiveString();
			
		}
	}
}
