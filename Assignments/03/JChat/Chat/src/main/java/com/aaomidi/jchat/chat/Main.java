package com.aaomidi.jchat.chat;

import com.aaomidi.jchat.chat.data.Configuration;
import com.aaomidi.jchat.chat.engine.ChatClient;
import com.aaomidi.jchat.chat.engine.ChatServer;
import com.aaomidi.jchat.minirsa.RSA;
import lombok.Getter;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Main {
    @Getter
    private Configuration loadedConfiguration;
    private ChatClient chatClient;
    private ChatServer chatServer;

    public static void main(String... args) {
        new Main(args);
    }

    public Main(String... args) {
        final Main instance = this;
        loadConfig();

        test();
        chatServer = new ChatServer(instance);
        chatClient = new ChatClient(instance);

        chatClient.sendMessage("Hi babe\n");


        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        for (; ; ) {
            String line = null;
            try {
                line = in.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }
            if (line == null || "quit".equalsIgnoreCase(line)) {
                break;
            }

            // Sends the received line to the server.
            chatClient.sendMessage(line);
        }

        while (true) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private void test() {
        RSA rsa = new RSA(43, 61);

        System.out.println(rsa.dec(rsa.enc("Hello?\nYou're cute!\tKek")));
    }


    public void loadConfig() {
        // TODO: Load configuration from a file.
        this.loadedConfiguration = new Configuration(7451);
    }
}
