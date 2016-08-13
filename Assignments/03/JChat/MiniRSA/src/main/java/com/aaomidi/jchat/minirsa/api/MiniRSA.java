package com.aaomidi.jchat.minirsa.api;

import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class MiniRSA {
    private static List<Long> primes = new ArrayList<>();
    // For security based applications, SecureRandom is best.
    private static SecureRandom random = new SecureRandom();

    static {
        primes.add((long) 2);
        primes.add((long) 3);
    }

    /**
     * Finds coprime of a number.
     *
     * @param x
     * @return
     */
    public static long coprime(long x) {
        while (true) {
            long num = nextLong(random, x);
            if (gcd(x, num) == 1) {
                return num;
            }
        }
    }

    /**
     * Encrypt and decrypt a character.
     *
     * @param m
     * @param key
     * @param c
     * @return
     */
    public static long endecrypt(char m, long key, long c) {
        return modulo(m, key, c);
    }

    /**
     * GCD of two numbers.
     * Euclid's Algorithm
     *
     * @param a
     * @param b
     * @return
     */
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

    /**
     * Inverse of a mod.
     *
     * @param base
     * @param m
     * @return
     */
    public static long modInverse(long base, long m) {
        long exp = totient(m) - 1;
        return modulo(base, exp, m);
    }

    /**
     * Modulo of numbers.
     *
     * @param a
     * @param b
     * @param c
     * @return
     */
    public static long modulo(long a, long b, long c) {
        long i;
        long temp = 1;
        for (i = 1; i <= b; i++) {
            temp *= a;
            temp = temp % c;
        }
        return temp;
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

    /**
     * Gets the nth prime.
     *
     * @param n
     * @return
     */
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

    /**
     * Checks if a number is prime.
     *
     * @param x
     * @return
     */
    public static boolean isPrime(long x) {
        for (long prime : primes) {
            if (prime > x / 2) return true;
            if (x % prime == 0) return false;
        }

        System.out.printf("Called with X:%d and largest Prime:%d\n", x, primes.get(primes.size() - 1));
        // This part of the code shouldn't even be called (unless this method is called directly)
        for (long i = primes.get(primes.size() - 1); i * i <= x; i += 2) {
            System.out.printf("I: %d\n", i);
            if (x % i == 0) return false;
        }
        return true;
    }

    /**
     * Gets the next long in a range... Somehow java doesn't have this.
     * Reference: https://stackoverflow.com/questions/2546078/java-random-long-number-in-0-x-n-range
     *
     * @param rng
     * @param n
     * @return
     */
    private static long nextLong(Random rng, long n) {
        long bits, val;
        do {
            bits = (rng.nextLong() << 1) >>> 1;
            val = bits % n;
        } while (bits - val + (n - 1) < 0L);
        return val;
    }
}
