#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// decide what type ACM to use, look in the handout for pseudo-code -- Signal type ACM

char slots[3][23]; // 3 slots; add the size of each slot instead of ... 23 index's because 23 characters in the printf line
// Find how to use LUT
const int LUT[3][3] = {{2, 2, 1},
                        {2, 0, 0},
                        {1, 0, 1}};     // look up table = few clock cycles, very very fast

int w=0, r=1, l=2; // define and initialise the variables as in the handout

void reader_thread () {

  while(1) { // reader loop; includes a delay to reduce the output data rate

      while(l == r);   // wait until l != r condition is true

      r = l;

      printf("%s", slots[r]); // access slot; slots[i] is a pointer to slots[i][0] READ
                               // (slots[i][0],slots[i][1],... is a 0-terminated string)
      fflush(stdout);

      usleep(814000);  // limit output rate; calculate your own value (>500000) talking about frames, should be similar to game usleep value
                       // should be a smaller that the game by the same percentage that rd is < rg
                       // should be a trade off
                       // do the maths for a lag of 7 seconds
      //printf("%d", r);

    }

}

char inp() {    // getchar() wrapper which checks for End Of File EOF

  char c;
  c=getchar();
  if(c==EOF) exit(0); // exit the whole process if input ends
  return c;
}

int main () {
  //...; // variable declarations/initialisation, if needed

  pthread_t read_thread;

  pthread_create(&read_thread, NULL, (void *) reader_thread, NULL);   //expecting a NULL pointer. Very dangerous, can point to any type

  while (1) {  // writer loop

// access slot; modify this according to the output format of your game
// in this example I keep reading until '*' appears,
// then the symbol 0 is added, which is the sting terminator.
// The terminator is needed for printf("%s",...) of the reader.

      //printf("%d", l); // for testing the ACM on its own

      int j = 0;
      while ((slots[w][j++] = inp ()) != '*'); // the actual computation takes place inside the condition WRITE
      slots[w][j] = 0; // append the terminating symbol to the string

      l = w;

      w = LUT[r][l]; // Implement the look up table

    }

  pthread_cancel(read_thread);

  return 0;
}
