Assignment 3
-------------------

Amir Omidi
Systems Programming
Assignment 3
email: aao53@drexel.edu

In this assignment we were supposed to write a Server/Client and a MiniRSA implementation.
I wrote this assignment in Java, using the netty library.

The way the server works is that, each user constructs their own server, bound on a specific port. 
After that the clients then connect to each others servers and send an "EHLO" message to each other with their corresponding public keys. After the first cleartext message the messages after that will all be encrypted.

To run the specific goals of the program:
Make sure you run `make build` before trying to run any of the goals.

Genkey:

	MPRIME=8 NPRIME=13 make genkey

Clients: (Run both of these at the same time)
	
	BINDPORT=4121 CLIENTPORT=4122 SERVER=localhost MPRIME=34 NPRIME=42 make chat
	BINDPORT=4122 CLIENTPORT=4121 SERVER=localhost MPRIME=41 NPRIME=17 make chat

When you run these last two make goal. It'll construct the server and ask you to press enter to connect to each other. (Remember, make sure both of the clients are "active" before pressing enter on both.
There is another way to construct the "chat", it is at follows:
	
	BINDPORT=4122 CLIENTPORT=4121 SERVER=localhost PRIME1=41 C=779 E=433 make preConfChat

This method creates the RSA key with preconfigured information obtained from `make genkey`

You could also make a generated key with random "nth and mth" primes:

	make genkey

To transfer messages between the clients, I wrapped the messages inside a json string (using gson) and sent that string between the clients.

This assignment helped me learn a lot about netty and how it works.

After you're done, you can clean the generated files using `make clean`.