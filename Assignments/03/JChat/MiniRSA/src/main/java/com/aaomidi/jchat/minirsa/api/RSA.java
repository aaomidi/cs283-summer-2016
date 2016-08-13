package com.aaomidi.jchat.minirsa.api;

import lombok.Getter;
import lombok.ToString;

import java.util.LinkedList;
import java.util.List;

@ToString
public class RSA {
    @Getter
    private final long prime1;
    @Getter
    private final long prime2;

    @Getter
    private final long c;
    @Getter
    private final long m;
    @Getter
    private final long e;
    @Getter
    private final long d;

    public static long encChar(char ch, int[] pubKey) {
        return MiniRSA.endecrypt(ch, pubKey[0], pubKey[1]);
    }

    public static List<Long> enc(String str, int[] pubKey) {
        List<Long> list = new LinkedList<>();
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

    public RSA(long a, long c, long e) {
        this.prime1 = a;
        this.prime2 = c / a;

        this.c = c;
        this.m = (prime1 - 1) * (prime2 - 1);
        this.e = e;
        this.d = MiniRSA.modInverse(e, m);
    }

    public String dec(List<Long> list) {
        String str = "";
        for (long i : list) {
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
