/*
Name: Michael Sterritt
Date: 4/29/17
CS 362 - Random Testing Quiz 2


Steps:

1) Looked at what was necessary for printing the desired output message. Determined that the random string needed to be "reset\0" and state had to be 9
2) Looked at how to get to state = 9 and determined that the random characters must go through each if statement.
3) At first, I set the inputString() function to return "reset\0" and went to working on the random character function first.
4) Implemented random character by generating a random int between 0 and 127 and converting it to character through ascii.
5) Compiled code and ran it to see if error was produced. Found that after a random number of iterations with the string set to "reset\0", error would be printed.
6) Next, I moved onto implementing the random string function.
7) I decided that the random string created would be only 6 characters long since I know we are only looking at the first 6 characters of the string.
7) Then, I determined that the final character of the string must always be '\0' since c-strings must always be terminated with a null character.
8) Now, in order to generate the first 5 characters of the string I implemented a similar method as inputChar() and generated a random character in each slot based off random ascii values.
9) I then decided to try running the code again with the change and after letting it run a while and completing ~20,000,000 iterations, I decided that the chances of creating random were to low (1/128^5) and decided
	to change how I generate random stings.
10) In order to better optimize random strings, I decided to reduce the random characters in the string to only be in the rand of a to z.
11) After trying to run the code again, I realized there was a bug in my function and that it was only returning a char instead of a string.
12) Fixed bug, and then ran the code again generating only random lowercase letters for the string. After ~6,000,000 iterations, program generated the correct string and error message was printed.
13) Decided to retry generating random strings with all ascii characters since previous bug prevented completion in step 9.
14) After waiting ~15 minutes and calculating that 'reset' would only appear 1 time in ~34 billion attempts, I decided to keep the previous limitation and restrict random string to lowercase characters.
15) Reran after reverting to lowercase characters to make sure code still worked. Found error after ~3 million iterations.
*/