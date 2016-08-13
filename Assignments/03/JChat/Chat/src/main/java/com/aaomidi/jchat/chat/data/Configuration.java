package com.aaomidi.jchat.chat.data;


import lombok.Getter;

public class Configuration {
    @Getter
    private final int bindNumber;
    @Getter
    private final int portNumber;

    public Configuration(int bindNumber, int portNumber) {
        this.bindNumber = bindNumber;
        this.portNumber = portNumber;
    }
}
