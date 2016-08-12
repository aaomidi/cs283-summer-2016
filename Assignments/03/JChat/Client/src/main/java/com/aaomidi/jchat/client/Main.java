package com.aaomidi.jchat.client;

import com.aaomidi.jchat.client.data.Configuration;

import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class Main {
    private Configuration loadedConfiguration;

    public static void main(String... args) {
        new Main(args);
    }

    public Main(String... args) {
        loadConfig();
        startSocketServer();
    }


    public void loadConfig() {
        // TODO: Load configuration from a file.
        this.loadedConfiguration = new Configuration(7642);
    }

    private void startSocketServer() {
        try {
            ServerSocket socket = new ServerSocket(loadedConfiguration.getPortNumber());
            Socket client = socket.accept();

            PrintWriter out = new PrintWriter(client.getOutputStream(), true);
        } catch (Exception ex) {

        }
    }
}

