build:
	mvn clean package
keycrack: jar/Cracker.jar
	java -jar jar/Cracker.jar $(E) $(C)
genkey: jar/MiniRSA.jar
	java -jar jar/MiniRSA.jar $(MPRIME) $(NPRIME)
chat: jar/Chat.jar
	java -jar jar/Chat.jar $(BINDPORT) $(CLIENTPORT) $(SERVER) $(MPRIME) $(NPRIME)
preConfChat: jar/Chat.jar
	java -jar jar/Chat.jar $(BINDPORT) $(CLIENTPORT) $(SERVER) $(PRIME1) $(C) $(E)
clean:
	rm jar/*.jar
	rm -rf Chat/target
	rm -rf Cracker/target
	rm -rf MiniRSA/target
	rm -rf target