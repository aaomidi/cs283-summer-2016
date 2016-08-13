package com.aaomidi.jchat.chat.data;

import com.aaomidi.jchat.minirsa.api.RSA;
import lombok.Getter;
import lombok.RequiredArgsConstructor;

import java.util.List;

@RequiredArgsConstructor
public class Message {
    @Getter
    private final List<Long> message;

    public String decrypt(RSA rsa) {
        return rsa.dec(message);
    }
}
