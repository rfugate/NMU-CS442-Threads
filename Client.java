//NMU Computer Science
//442 - Advanced Network Programming
//Rob Fugate
//Purpose: Connect to a server and retrieve the current value of pi

import java.io.*;
import java.net.*;

public class Client {
    public static void main(String[] args) throws IOException {

        Socket echoSocket = null;
        PrintWriter out = null;
        BufferedReader in = null;

        try {
            echoSocket = new Socket("euclid.nmu.edu", 40956);
            out = new PrintWriter(echoSocket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(
                                        echoSocket.getInputStream()));
        } catch (UnknownHostException e) {
            System.err.println("Don't know about host: euclid.");
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for "
                               + "the connection to: euclid.");
            System.exit(1);
        }
        System.out.println(in.readLine());

	out.close();
	in.close();
	echoSocket.close();
    }
}
