package com.aaomidi.jchat.minirsa;

import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class MiniRSA {
    private static List<Long> primes = new ArrayList<>();
    private static SecureRandom random = new SecureRandom();

    static {
        primes.add((long) 2);
        primes.add((long) 3);
    }

    public static long coprime(long x) {
        while (true) {
            long num = nextLong(random, x);
            if (gcd(x, num) == 1) {
                return num;
            }
        }
    }

    public static long endecrypt(char m, long key, long c) {
        return modulo(m, key, c);
    }

    public static long gcd(long a, long b) {
        long x, y;
        x = (a > b) ? a : b;
        y = (a > b) ? b : a;
        long div, mod;
        while (true) {
            mod = x % y;
            if (mod == 0) return y;
            x = y;
            y = mod;
        }
    }

    public static long modInverse(long base, long m) {
        long exp = totient(m) - 1;
        return modulo(base, exp, m);
    }

    public static long modulo(long a, long b, long c) {
        long i;
        long now = 1;
        for (i = 1; i <= b; i++) {
            now *= a;
            now = now % c;
        }
        return now;
    }

    public static long totient(long n) {
        long i;
        long tot = 0;
        for (i = 1; i < n; i++) {
            if (gcd(i, n) == 1)
                tot++;
        }
        return tot;
    }

    public static long nthPrime(int n) {
        int size = primes.size();
        if (size > n) {
            return primes.get(n);
        }

        for (long j = primes.get(size - 1); j < Long.MAX_VALUE; j += 2) {
            if (isPrime(j)) {
                primes.add(j);
                size++;
            }
            if (size > n) {
                break;
            }
        }
        return primes.get(n);
    }

    public static boolean isPrime(long x) {
        for (long prime : primes) {
            if (prime > x / 2) return true;
            if (x % prime == 0) return false;
        }

        System.out.printf("Called with X:%d and largest Prime:%d\n", x, primes.get(primes.size() - 1));
        // This part of the code shouldn't even be called.
        for (long i = primes.get(primes.size() - 1); i * i <= x; i += 2) {
            System.out.printf("I: %d\n", i);
            if (x % i == 0) return false;
        }
        return true;
    }

    private static long nextLong(Random rng, long n) {
        long bits, val;
        do {
            bits = (rng.nextLong() << 1) >>> 1;
            val = bits % n;
        } while (bits - val + (n - 1) < 0L);
        return val;
    }
}
