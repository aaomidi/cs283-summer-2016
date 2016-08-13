package com.aaomidi.jchat.cracker;


import com.aaomidi.jchat.minirsa.RSA;

import java.util.Scanner;

public class Main {
    private Scanner s = new Scanner(System.in);
    private long e, c;
    private RSA rsa;

    public static void main(String... args) {
        Main main = new Main(args);
    }

    public Main(String... args) {
        start(args);
    }

    private void start(String... args) {
        try {
            if (args.length != 2) {
                getPublicKey();
            } else {
                e = Long.valueOf(args[0]);
                c = Long.valueOf(args[1]);
            }
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

        rsa = new RSA(i, c, e);

        if (!isCracked) {
            System.out.println("Uuuuuuh no?");
            System.exit(0);
        } else {
            System.out.printf("%s\n", rsa.toString());
        }
    }

    private void getPublicKey() {
        System.out.println("Enter the public key value (e):");
        String pubKey = s.nextLine();
        this.e = Long.valueOf(pubKey);

        System.out.println("Enter the c that goes with the public key: ");
        String cString = s.nextLine();
        this.c = Long.valueOf(cString);
    }

    private void decrypt() {
        String line;
        while (true) {
            System.out.println("Enter a number to decrypt or quit to exit: ");
            line = s.nextLine();
            if (line.equals("quit")) {
                break;
            }
            long num = Long.parseLong(line);
            char decryptedChar = rsa.decChar(num);
            System.out.printf("This char is decrypted to %s%n", decryptedChar);
            System.out.printf("This letter is %d%n", (int) decryptedChar);
        }
        System.out.println("Done");
    }
}
