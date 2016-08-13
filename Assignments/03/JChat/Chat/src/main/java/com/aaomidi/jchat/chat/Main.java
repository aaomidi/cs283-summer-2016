package com.aaomidi.jchat.chat;

import com.aaomidi.jchat.chat.data.Configuration;
import com.aaomidi.jchat.chat.data.Message;
import com.aaomidi.jchat.chat.engine.ChatClient;
import com.aaomidi.jchat.chat.engine.ChatServer;
import com.aaomidi.jchat.minirsa.MiniRSA;
import com.aaomidi.jchat.minirsa.RSA;
import com.google.gson.Gson;
import lombok.Getter;
import lombok.Setter;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Main {
    @Getter
    private Configuration loadedConfiguration;
    private ChatClient chatClient;
    private ChatServer chatServer;
    // Other person's public key
    @Getter
    @Setter
    private int[] otherPublicKey = new int[2];
    @Getter
    private RSA rsa;
    @Getter
    private Gson gson = new Gson();

    public static void main(String... args) {
        new Main(args);
    }

    public Main(String... args) {
        final Main instance = this;
        Integer bindNumber = Integer.valueOf(args[0]);
        Integer portNumber = Integer.valueOf(args[1]);

        Integer v1 = Integer.valueOf(args[2]);
        Integer v2 = Integer.valueOf(args[3]);
        loadConfig(bindNumber, portNumber);

        rsa = new RSA(MiniRSA.nthPrime(v1), MiniRSA.nthPrime(v2));

        chatServer = new ChatServer(instance);
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        chatClient = new ChatClient(instance);

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

            Message msg = new Message(RSA.enc(line, otherPublicKey));

            chatClient.sendMessage(gson.toJson(msg, Message.class));
        }

    }


    public void loadConfig(int bindNumber, int portNumber) {
        // TODO: Load configuration from a file.
        this.loadedConfiguration = new Configuration(bindNumber, portNumber);
    }
}
