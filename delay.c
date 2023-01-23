#include <stdio.h>
#include <termios.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{

 char c;
 for(;;)
   {
     c=getc(stdin);
     if (c == EOF) return 0;
     usleep(37037); // specify delay for your experiment SHOULD BE 20 - 30 MS value Rd 1/Rd
     printf("%c",c);
     fflush(stdout);
   }
 return 0;

}
