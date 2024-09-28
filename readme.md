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



### TO Learn in depth

1. What are file descriptors


https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html