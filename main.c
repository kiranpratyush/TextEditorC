#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
// Macro definition of the CNTRL_KEY detection
#define CNTRL_KEY(k) ((k) & 0x1f)

// global terminal state
struct termios original_terminal_state;

// exit the program with error
void die(char *s)
{
    perror(s);
    exit(1);
}
// back to the canonical mode
void disableraw()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal_state) == -1)
    {
        die("tcsetattr");
    };
}
void read_terminal_state()
{
    int status = tcgetattr(STDIN_FILENO, &original_terminal_state);
    if (status == -1)
    {
        die("tcgetattr");
    }
}

void enable_raw_mode()
{
    struct termios current_terminal_state = original_terminal_state;
    current_terminal_state.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    current_terminal_state.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    current_terminal_state.c_oflag &= ~(OPOST);
    current_terminal_state.c_cflag |= (CS8);
    current_terminal_state.c_cc[VMIN] = 0;
    current_terminal_state.c_cc[VTIME] = 1;
    // set the terminal attribute
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &current_terminal_state) == 1)
    {
        die("tcsetattr");
    };
}
// Read character byte
char read_char_byte()
{
    char c;
    int status = read(STDIN_FILENO, &c, 1);
    if (status == -1 && errno != EAGAIN)
    {
        die("read");
    }
    return c;
}

void preprocess_key_press()
{
    char c = read_char_byte();
    switch (c)
    {
    case CNTRL_KEY('q'):
        exit(0);
        break;
    }
}

int main()
{
    read_terminal_state();
    enable_raw_mode();
    atexit(disableraw);
    while (1)
    {
        preprocess_key_press();
    }
    return 0;
}
