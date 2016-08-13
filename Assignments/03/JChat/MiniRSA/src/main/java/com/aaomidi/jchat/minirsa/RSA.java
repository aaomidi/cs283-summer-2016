package com.aaomidi.jchat.minirsa;

import lombok.ToString;

import java.util.LinkedList;
import java.util.List;

@ToString
public class RSA {
    private final long prime1;
    private final long prime2;

    private final long c;
    private final long m;
    private final long e;
    private final long d;

    public static int encChar(char ch, int[] pubKey) {
        return (int) MiniRSA.endecrypt(ch, pubKey[0], pubKey[1]);
    }

    public static List<Integer> enc(String str, int[] pubKey) {
        List<Integer> list = new LinkedList<>();
        for (char ch : str.toCharArray()) {
            list.add(encChar(ch, pubKey));
        }
        return list;
    }

    /**
     * Create an RSA out of two primes.
     *
     * @param prime1 First prime.
     * @param prime2 Second prime.
     */
    public RSA(long prime1, long prime2) {
        this.prime1 = prime1;
        this.prime2 = prime2;

        this.c = prime1 * prime2;
        this.m = (prime1 - 1) * (prime2 - 1);
        this.e = MiniRSA.coprime(m);
        this.d = MiniRSA.modInverse(e, m);
    }

    public String dec(List<Integer> list) {
        String str = "";
        for (int i : list) {
            str += decChar(i);
        }
        return str;
    }

    public char decChar(long enc) {
        char res = (char) MiniRSA.modulo(enc, d, c);

        return res;
    }

    /**
     * Get the public key.
     *
     * @return public key
     */
    public long[] getPubKey() {
        long[] x = new long[2];
        x[0] = e;
        x[1] = c;
        return x;
    }

    /**
     * Get the private key.
     *
     * @return private key
     */
    public long[] getPrivKey() {
        long[] x = new long[2];
        x[0] = d;
        x[1] = c;
        return x;
    }
}
