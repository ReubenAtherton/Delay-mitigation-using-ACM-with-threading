#include <stdio.h>
#include <termios.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define width  40
#define height 18
#define fox_init_dir 'r'

// removes need to press enter after each direction by user
struct termios tty_prepare () {
  struct termios tty_attr_old, tty_attr;
  tcgetattr (0, &tty_attr);
  tty_attr_old = tty_attr;
  tty_attr.c_lflag &= ~(ECHO | ICANON);
  tty_attr.c_cc[VMIN] = 1;
  tcsetattr (0, TCSAFLUSH, &tty_attr);
  return tty_attr_old;
}

// restores keyboard to original settings -- must be included otherwise terminal corrupts
void tty_restore (struct termios tty_attr){
  tcsetattr (0, TCSAFLUSH, &tty_attr);
}

char fox_dir = fox_init_dir;
int count = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void keys_thread() {

    char key;

    while(1) {

        for(int i = 0; i < 3; i++){
            key = getchar();
        }

        // Sets the 'char' type variable for switch case in update_coord() function
        if(key == 65) {
            fox_dir = 'u';    // up
            pthread_mutex_lock(&mutex1);        // Prevents data inconsistencies due to race conditons. Global variable count is accessed here and in the main
            count++;                            // and so a mutex is needed. Mutex protects segment of memory from other threads.
            pthread_mutex_unlock(&mutex1);
        }
        if(key == 66) {
            fox_dir = 'd';    // down
            pthread_mutex_lock(&mutex1);
            count++;
            pthread_mutex_unlock(&mutex1);
        }
        if(key == 67) {
            fox_dir = 'r';    // left
            pthread_mutex_lock(&mutex1);
            count++;
            pthread_mutex_unlock(&mutex1);
        }
        if(key == 68) {
            fox_dir = 'l';    // right
            pthread_mutex_lock(&mutex1);
            count++;
            pthread_mutex_unlock(&mutex1);
        }
    }
}
//  Updates coordinates for both characters
void update_coord (int *x_ptr, int *y_ptr, char dir) {
    switch (dir) {
        case 'u':       // UP command
            if(1 <= *y_ptr <= height)                                     // makes sure the character stays within the limits of the given window
                if(*y_ptr == 0) *y_ptr = 1;                               // sets the characters minimum y coordinates as 1, even if the user directs below 1
                else if(*y_ptr == (height+1)) *y_ptr = height;           // sets the characters maximum y coordinates as 18, even if the user directs above 18
                (*y_ptr)--;                                               // decrements the characters y coordinates by 1 if user enters up-arrow key
                break;

        case 'd':       // DOWN command
            if (1 <= *y_ptr <= height)
                if(*y_ptr == 0) *y_ptr = 1;
                else if(*y_ptr == (height+1)) *y_ptr = height;
                (*y_ptr)++;     // increments the characters y coordinates by 1 if user enters down-arrow key
                break;

        case 'l':       //LEFT command
            if (1 <= *x_ptr <= width)
                if(*x_ptr == 0) *x_ptr = 1;
                else if(*x_ptr == (width+1)) *x_ptr = width;
                (*x_ptr)--;     // decrements the characters x coordinates by 1 if user enters left-arrow key
                break;

        case 'r':       //RIGHT command
            if (1 <= *x_ptr <= width)
                if(*x_ptr == 0) *x_ptr = 1;
                else if(*x_ptr == (width+1)) *x_ptr = width;
                (*x_ptr)++;     // increments the characters x coordinates by 1 if user enters right-arrow key
                break;
    }
}

int main(){

    // ******** Variable declarations ********
    int fox_x = 2, fox_y = 2, rabbit_x = 15, rabbit_y = 10;
    char rabbit_dir;

    struct termios term_back = tty_prepare();       // declares 'term_back' of the same type that tty_prepare() function returns

    pthread_t thread;

    pthread_create(&thread, NULL, (void *) keys_thread, NULL);

    while(1) {

        usleep(500000);

        update_coord (&fox_x, &fox_y, fox_dir);

        //Generate random direction for rabbit
        srand(time(NULL));
        int rabbit_rand = rand() % 4;

        if(rabbit_rand == 0) rabbit_dir = 'u';
        if(rabbit_rand == 1) rabbit_dir = 'd';
        if(rabbit_rand == 2) rabbit_dir = 'l';
        if(rabbit_rand == 3) rabbit_dir = 'r';

        update_coord(&rabbit_x, &rabbit_y, rabbit_dir);

        printf("\033[2J\033[%d;%dH>\033[%d;%dH*", fox_y, fox_x, rabbit_y, rabbit_x); // 22 characters in the printf
        //printf("\033[21;1HFOX(%d,%d)", fox_x, fox_y);        // \033[21;1 sets the cursor to these coordinates to fix these lines
        //printf("\033[22;1HRABBIT(%d,%d)", rabbit_x, rabbit_y);

        fflush(stdout);

        //if(fox_y == rabbit_y && fox_x == rabbit_x) break; // This can be implemented to make it harder

        // ****** CREATES A RANGE FOR THE CAPTURE OF THE RABBIT ****** -- this removes problem when coordinates swap instead of equalling
        int y_range_low_rabbit = (rabbit_y-1);
        int y_range_high_rabbit = (rabbit_y+1);

        int x_range_low_rabbit = (rabbit_x-1);
        int x_range_high_rabbit = (rabbit_x+1);

        if((y_range_low_rabbit <= fox_y && fox_y <= y_range_high_rabbit) && (x_range_low_rabbit <= fox_x && fox_x <= x_range_high_rabbit)) {
            break;
        }
    }

    pthread_cancel(thread);
    tty_restore(term_back);
    system("clear");
    printf("\n\n\n\n\t\t\tGAME OVER --- DIRECTIONAL CHANGES: %d\n\n\n\n", count);

    return 0;
}
