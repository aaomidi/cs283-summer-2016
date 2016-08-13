package com.aaomidi.jchat.cracker;


import com.aaomidi.jchat.minirsa.MiniRSA;

import java.util.Scanner;

public class Cracker {
    Scanner s = new Scanner(System.in);
    long e; // prime to m
    long d; // modular inverse of e mod m

    long a; // prime 1
    long b; // prime 2

    long c; // a * b
    long m; // a-1 * b-1

    long totient;

    public static void main(String[] args) {
        Cracker cracker = new Cracker();
        cracker.start();
    }

    private void start() {
        try {
            getPublicKey();
            crackPrivateKey();
            decrypt();
        } catch (Exception ex) {
            System.out.println("Incorrect entry.");
            ex.printStackTrace();
            System.exit(0);
        }

    }

    private void crackPrivateKey() {
        boolean isCracked = true;
        long i = 2;
        if (this.c % 2 != 0) {
            long end = (long) Math.sqrt(this.c) + 1;
            for (i = 3; i < end; i = i + 2) {
                if (c % i == 0) {
                    break;
                }
            }
            if (i == end) {
                isCracked = false;
            }
        }

        this.a = i;
        this.b = this.c / i;
        this.m = (a - 1) * (b - 1);
        this.d = MiniRSA.modInverse(e, m);
        this.totient = MiniRSA.totient(m);

        if (!isCracked) {
            System.out.println("Uuuuuuh no?");
            System.exit(0);
        } else {
            System.out.println("a was " + this.a + " b was " + this.b);
            System.out.println("The totient is " + this.totient);
            System.out.println("D was found to be " + this.d);
        }
    }

    private void getPublicKey() {
        System.out.println("Enter the public key value (e):");
        String pubKey = s.nextLine();
        this.e = Long.parseLong(pubKey);

        System.out.println("Enter the c that goes with the public key:");
        String cString = s.nextLine();
        this.c = Long.parseLong(cString);
    }

    private void decrypt() {
        String line;
        while (true) {
            System.out.println("Enter a number to decrypt or quit to exit:");
            line = s.nextLine();
            if (line.equals("quit")) {
                break;
            }
            long num = Long.parseLong(line);
            char decryptedChar = (char) MiniRSA.modulo(num, d, c);
            System.out.println("This char is decrypted to " + decryptedChar);
            System.out.println("This letter is " + (int) decryptedChar);
        }
        System.out.println("Done");
    }
}
