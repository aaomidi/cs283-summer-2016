package com.aaomidi.jchat.chat.data;


import lombok.Getter;

public class Configuration {
    @Getter
    private final int bindNumber;
    @Getter
    private final int portNumber;
    @Getter
    private final String serverAddress;

    public Configuration(int bindNumber, int portNumber, String serverAddress) {
        this.bindNumber = bindNumber;
        this.portNumber = portNumber;
        this.serverAddress = serverAddress;
    }
}
