Assignment 1
------------

Amir Omidi
Systems Programming
Assignment 1
email: aao53@drexel.edu


In this assignment we were supposed to write an anagram finder and a scrabble solver.
To do this, I wrote a very simple HashTable to do this. To hash the strings I used a very simple ascii table implementation where I checked the word's ascii value and if they were "letters" I used their normalized ascii value to create the hashcode. Any non letter characters were ignored.
To remove the \n from the words in the dictionary, I basically made a method to check for \n and replace it with \0 before making an entry with it, this made comparing the strings at the end simpler.

 Clion with a Linux VM was used to develop and test the code for this assignment. The code, however, has been cloned to tux and tested there.

The program works perfectly with the make examples given in the website.
Sample runs:
```
make anagram
./Assignment1.o -a
 Finding all Anagrams for Amir
2. Mari
3. 3. Mira
```

``` 
make scrabble
./Assignment1.o -s   
Finding all possible scrabble values for Amir with m at 0
1. Mari
2. Mira

```
```
WORD=Caroline make anagram 
./Assignment1.o -a Caroline
Finding all Anagrams for Caroline
1. Cornelia

```
```
WORD=Amir LETTER=a PLACE=1 make scrabble
./Assignment1.o -s Amir a 1
Finding all possible scrabble values for Amir with a at 1
1. Mari
```

Its note worthy that according to valgrind, there are no memory leaks :)