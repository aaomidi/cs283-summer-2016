package com.aaomidi.jchat.chat;

import com.aaomidi.jchat.chat.data.Configuration;
import com.aaomidi.jchat.chat.data.Message;
import com.aaomidi.jchat.chat.engine.ChatClient;
import com.aaomidi.jchat.chat.engine.ChatServer;
import com.aaomidi.jchat.minirsa.api.MiniRSA;
import com.aaomidi.jchat.minirsa.api.RSA;
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
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

        final Main instance = this;
        Integer bindNumber = Integer.valueOf(args[0]); // Server bound port
        Integer portNumber = Integer.valueOf(args[1]); // Other person's server
        String serverAddress = args[2]; // Server address,
        loadConfig(bindNumber, portNumber, serverAddress);

        if (args.length == 5) {
            Integer v1 = Integer.valueOf(args[3]); // Private key generation nth Prime
            Integer v2 = Integer.valueOf(args[4]); // Private key generation mth Prime
            rsa = new RSA(MiniRSA.nthPrime(v1), MiniRSA.nthPrime(v2));
        } else {
            Integer PRIME1 = Integer.valueOf(args[3]); // A
            Integer C = Integer.valueOf(args[4]); // C
            Integer E = Integer.valueOf(args[5]); // E

            rsa = new RSA(PRIME1, C, E);
        }
        chatServer = new ChatServer(instance);

        System.out.printf("Server constructed. Please press enter to connect to %s:%d\n", getLoadedConfiguration().getServerAddress(), getLoadedConfiguration().getPortNumber());
        try {
            in.readLine(); // Wait for one line
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.printf("Client connected. You can type \"quit\" to stop chatting.%n");
        chatClient = new ChatClient(instance);

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

        chatClient.shutdown();
        chatServer.shutDown();

    }

    /**
     * Loads a "fake" configuration file.
     *
     * @param bindNumber IP of the server to bind to.
     * @param portNumber IP of the server to connect to
     */
    public void loadConfig(int bindNumber, int portNumber, String serverAddress) {
        // TODO: Load configuration from a file.
        this.loadedConfiguration = new Configuration(bindNumber, portNumber, serverAddress);
    }
}
