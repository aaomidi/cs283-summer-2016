package com.aaomidi.jchat.minirsa;

import com.aaomidi.jchat.minirsa.api.MiniRSA;
import com.aaomidi.jchat.minirsa.api.RSA;

import java.security.SecureRandom;
import java.util.Random;

public class Main {
    public static void main(String... args) {
        int n, m;
        Random random = new SecureRandom();
        if (args.length == 0) {
            n = random.nextInt(500);
            m = random.nextInt(500);
        } else {
            n = Integer.valueOf(args[0]);
            m = Integer.valueOf(args[1]);
        }

        long nPrime = MiniRSA.nthPrime(n);
        long mPrime = MiniRSA.nthPrime(m);

        RSA rsa = new RSA(nPrime, mPrime);

        System.out.println(rsa.toString());
    }
}
