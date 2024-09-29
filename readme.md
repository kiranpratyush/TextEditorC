It starts with the make file configuration but I won't do it right now.

So what is a text editor.

### What is a canonical mode.

Cannonical mode is when we type the query and then press enter then it goes into the program.

1. We need a way to be able to read from the stdin and directly pass to the variable with out having to press enter
2. When we are typing we don't need see what we are typing in the terminal.

## How to stop echoing

in termios structure we can change the bit fields of various flags which will change the structure of the terminal behaviour

termios_p is a structure pointer
fildes is a open file descriptor

int tcsetattr(int fildes, int optional_actions,
       const struct termios *termios_p);

What are optional_actions
CSAFLUSH, the change shall occur after all output written to fildes is transmitted, and all input so far received but not read shall be discarded before the change is made.


## How to implement a read timeout
In temrious there is a VTIME flag.
After VTIME read will send -1 without reading



### What are file descriptors

1. What are file descriptors
   1. File descriptors are the integers which tells identifies all the open files by a process
   2. Mostly any external file starts with the integer 3 because STD_INPUT is marked as 0 , STD_OUT is marked as 1 and STD_ERR is marked as 2;
### How to detect which contol character is pressed
1. ASCII control characters
   1. 0-31 in ASCII represents the control characters
   2. Then from 64-95 (lower case) and (96-127) represents the characters
   3. control+[character] represent the character with the 3 high bits cleared
   4. so mask 0x1f : 00011111 when anding with the characters clears the higher 3 bits
   5. so we get the control character so for example mask with character q makes the value control+q



https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html