
#include<stdlib.h>
#include<termios.h>
#include<stdio.h>
#include<unistd.h>
#include<ctype.h>
#include<errno.h>

#define CNTRL_KEY(k) ((k)&0x1f)

struct termios original_terminal_state ;

void disableraw()
{
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&original_terminal_state)==-1)
    {
        die("tcsetattr");
    };

}
void die(char *s)
{
    perror(s);
    exit(1);
}


int main()
{
if(tcgetattr(STDIN_FILENO,&original_terminal_state)==-1)
{
    die("tcgetattr");
};
atexit(disableraw);
// change the terminal flags
struct termios current_terminal_state = original_terminal_state;

current_terminal_state.c_lflag &= ~(ECHO | ICANON | ISIG|IEXTEN);
current_terminal_state.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
current_terminal_state.c_oflag&= ~(OPOST);
current_terminal_state.c_cflag |= (CS8);
current_terminal_state.c_cc[VMIN]=0;
current_terminal_state.c_cc[VTIME]=1;
// set the terminal attribute

if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&current_terminal_state)==1)
{
    die("tcsetattr");
};


while(1)  
{   char c = '\0';
    if(read(STDIN_FILENO,&c,1)==-1 && errno != EAGAIN) die("read");
    // Check if the character is a control character
    if(iscntrl(c))
    {
        printf("%d \r\n",c);
    }
    else
    {
        printf("%d %c \r\n",c,c);
    }
    if(c==CNTRL_KEY('c'))
    {
        break;
    }
}
return 0;
}
