  
#include <arch/zx.h>

#include <stdio.h>
#include <stdlib.h>

/*  
struct teststruct {
    int a;
    int b;
};

int main()
{
  struct teststruct tst = {10, 20};

  printf("%d, %d", tst.a, tst.b);
  return 0;

}
*/

typedef struct breadSlice {
  int thickness;
  int cookedness;
} slice;

struct breadToToast {
  struct breadSlice *breads;
  int totalSlices;
  int currentSlice;
};

// 1.) Forward declaration: Here is the name of the structure
// but member-details are omitted.
struct GameComponent_struct;

// 2.) typedef of the structure
// Still no details on the members.
typedef struct GameComponent_struct  GameComponent;

// 3.) The parameter to the function is listed, using the definition
// from step 2.)  (note: Still no details on the members yet)
typedef int (*GameComponentFunc)(GameComponent* input);

// 4.) Now we actually detail the members of the structure
struct GameComponent_struct {
  int                        address;
  GameComponentFunc          func;
};


int print_address (GameComponent *input){
  printf("The address is %d \n", input->address);
  //printf("Push any key to continue:");
  //getchar();
  (input->address)++;
  return input->address;
}

int change_border(GameComponent *input) {
  zx_border(input->address);
  input->address = (input->address + 1) %8;
  return input->address;
}

int wait_cycle(GameComponent *input) {
  //Time LSD sits at 23672
  int cont  = 1;
  int iloop = 0;
  int maxloop = 10;
  char* ptr;
  char ch = 0;
  ptr = (char *)0x5C78; /* address to be peeked */

  while ((cont == 1) && (iloop < maxloop)) {
    iloop++;
    ch  = *ptr;
    if ((int)ch != input->address) {
      cont = 0;
    }
  }
  if (iloop >= maxloop) {
    printf("\x16\x05\x05" "Max loop %d \n", input->address);
  }
  input->address = (int)ch + 1;

  return input->address;
}

int operate_toaster(GameComponent *input)
{

  int *ptrCurrentSlice = &(((struct breadToToast*)(input->address))->currentSlice);
  if (*ptrCurrentSlice > 0) {
    int stateOfSlice = --(((struct breadToToast*)(input->address))->breads[*ptrCurrentSlice].thickness);
    if (stateOfSlice == 0) {
      printf("\x16\x01\x0A" "Toast done. %d slices to go", *ptrCurrentSlice);
      (*ptrCurrentSlice)--;
    }
  }
  return *ptrCurrentSlice;
}

int main()
{
  int i;
  /* Set up the bread */
  int totalSlices = rand()%200;
  printf("We are going to toast %d slices\n", totalSlices);
  slice *allTheSlices = malloc(sizeof(struct breadSlice) * totalSlices);
  struct breadToToast allTheBread = { .breads = allTheSlices,
                                      .totalSlices = totalSlices,
                                      .currentSlice = totalSlices - 1
                                    };
  for (int i = 0; i < totalSlices; i++) {
    /* Initialise the slices, all untoasted*/
    allTheBread.breads[i].thickness = rand()%40;
    allTheBread.breads[i].cookedness = 0;
  }

  GameComponent input = {.address    = 16,
                        .func        = &print_address};

  GameComponent border = {.address    = INK_CYAN,
                        .func         = &change_border};

  GameComponent metronome = {.address      = 0,
                             .func         = &wait_cycle};

  GameComponent toastBread = { .address     =(int) &allTheBread,
                              .func        = &operate_toaster};

  for (i = 0; i < 1000; i++) {
    //input.func(&input);
    border.func(&border);
    metronome.func(&metronome);
    toastBread.func(&toastBread);
    printf("\x16\x0A\x0B" "i = %d %d \n", i, *((char *)0x5C78));
  }
  printf("i = %d \n", i);
  free(allTheSlices);
  return print_address(&input);
  //return 0;
} 