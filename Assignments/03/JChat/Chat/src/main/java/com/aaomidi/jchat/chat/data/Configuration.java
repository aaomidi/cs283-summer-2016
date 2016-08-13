package com.aaomidi.jchat.chat.data;


import lombok.Getter;

public class Configuration {
    @Getter
    private final int portNumber;

    public Configuration(int portNumber) {
        this.portNumber = portNumber;
    }
}
