#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
// Macro definition of the CNTRL_KEY detection
#define CNTRL_KEY(k) ((k) & 0x1f)

// function defintions

void clean_up_and_reset_cursor();
void die(char *);

// global terminal state
struct termios original_terminal_state;
// Global terminal_size
struct terminal_size
{
    int n_cols;
    int n_rows;
};

struct terminal_size global_terminal_size;

void update_terminal_size()
{ // Move the terminal to the right bottom right
    printf("\033[999C\033[999B");
    // Now request the cursor position
    printf("\033[6n");
    fflush(stdout);
    // Now read byte by byte
    char buf[32];
    int i = 0;
    while (i < 32)
    {
        if (read(STDIN_FILENO, &buf[i], 1) != 1)
            break;
        if (buf[i] == 'R')
            break;
        i++;
    }
    // Add the the null char to the end buf
    buf[i] = '\0';
    int rows, cols;
    // Use sscanf to parse the format
    if (sscanf(buf, "\033[%d;%dR", &(&global_terminal_size)->n_rows, &(&global_terminal_size)->n_cols) != 2)
    {
        die("terminal");

    }
    printf("\033[u");
    fflush(stdout);
        
}
// exit the program with error
void die(char *s)
{
    clean_up_and_reset_cursor();
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
        clean_up_and_reset_cursor();
        exit(0);
        break;
    }
}

// Refresh the editor screen

void clean_up_and_reset_cursor()
{
    // This clears up the terminal
    write(STDOUT_FILENO, "\x1b[2J", 4);
    // This does the reposition of cursor
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void draw_hash_on_screen()
{
    for (int i = 0; i < global_terminal_size.n_rows; i++)
    {
        write(STDOUT_FILENO, "#\r\n", 3);
    }
}

void refresh_editor_screen()
{
    clean_up_and_reset_cursor();
    draw_hash_on_screen();
    write(STDOUT_FILENO, "\x1b[H", 3);
}

int main()
{
    read_terminal_state();
    enable_raw_mode();
    atexit(disableraw);
    update_terminal_size();
    refresh_editor_screen();
    while (1)
    {
        preprocess_key_press();
    }
    return 0;
}
