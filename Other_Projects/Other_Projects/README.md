# Executive Summary
In this project you are writing a utility called "Vault" that provides secret, authenticated storage. The conceptual setup is "real world", although the particular cryptographic algorithms we use are not, because they are text-based historical techniques, rather than modern digital ones. The key points are:
data is stored by labels, so you give commands like "get the data associated with the label psptnum"
there are multiple users storing data in a single file
users are authenticated so that nobody sees any data other than their own
cryptography keeps the data in the file secret, even from someone who can read the file and who has a copy of the Vault program and its source code
the labels themselves are secured by encryption, so that someone who steals the file can get usernames but not data or the labels under which the data is stored.
This is probably a bit easier to see with an example:
file vault0	sample run
user joe shift+vigenere ?5d`54=22d5Boqqv   
user ann shift+caesar 2018^midGO_NAVY_
data joe vigenere LjTmZ8o*s=CEWyi.q
data joe caesar z53y5vJKDIHDHO
data ann caesar instrument_french_horn
~/$ java Vault vault0
username:  joe
password:           ← password not echoed to the screen!
Access granted!
>  labels
ssnum
combo
> get ssnum
892-88-1263
> quit
This project is designed to exercise your ability to write good Object-oriented code in Java, which will include good use of encapsulation, data-hiding, inheritance, polymorphism, class hierarchies, interfaces and exception handling.
The project has three main pieces: encryption algortihms, hash algorithms, and the vault. If you do a good job of following OOP best-practices in producing your solutions to the first two parts, you should be OK on the third. Otherwise ...