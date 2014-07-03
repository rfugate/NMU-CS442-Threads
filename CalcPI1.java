//NMU Computer Science
//442 - Advanced Network Programming
//Rob Fugate
//Purpose: Create a server that constantly gernerating a value for pi and return the value of pi whenever a client connects

import java.lang.*;
import java.net.*;
import java.io.*;
import java.util.*;

class CalcPI1{
	public static volatile double pi = 0;

	public static void main(String[] args) throws IOException{

		PIThread pt = new PIThread();
		pt.start();

		ServerSocket serverSocket = null;
		//Socket s = null;

		try{
			System.out.println("Trying to create Server Socket..");
			serverSocket = new ServerSocket(40960);
			//s = new Socket("euclid.nmu.edu", 40956);
			System.out.println("Created Server Socket!");
			System.out.println("serverSocket: " + serverSocket);
		}
		catch(IOException e){
			System.err.println("Could not listen on port: 40956");
			System.exit(-1);
		}

		while(true){
			new NetworkThread(serverSocket.accept()).start();
		}
	}


	static class PIThread extends Thread{
		double xValue = 0;
		double yValue = 0;
		double distance = 0;
		double inValue = 0;
		double count = 0;

		public void run(){
			while(1 == 1){
				count++;
				xValue = ((double)Math.random() / 1);
				yValue = ((double)Math.random() / 1);
				distance = ((xValue * xValue) + (yValue * yValue));
				if(distance <= 1.00)
					inValue++;
				pi = 4*(inValue/count);
			}
		}
	}

	static class NetworkThread extends Thread{
		//System.out.println("NetworkThread Started Successfully!");
		private Socket socket = null;

		public NetworkThread(Socket socket){
			this.socket = socket;
		}

		public void run(){
			try{
				PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
				while(true){
					out.println("Value of PI: " + pi);
					out.close();
					socket.close();
					return;
				}
			}
			catch(IOException e)
			{
				e.printStackTrace();
			}
		}
	}
}
