#define OUTPUT_FILE "/Users/Zaerospi/Desktop/results.txt"

#define TEXT_LENGTH 22
#define K_PLUS_R { 26, 15, 18, 2, 14, 9, 18, 33, 10, 6, 11, 34, 1, 22, 25, 4, 12, 26, 29, 5, 27, 23 }

// uses underscore rather than space to be more obvious
#define CHAR_LIST "_0123456789abcdefghijklmnopqrstuvwxyz"
#define OFFSET 1

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef int_fast64_t longest;
typedef int_fast8_t fastest;

fastest i = 0;
int_fast32_t counter = 0;
longest lastRand = 0, myRand = 0;

const fastest kandr[TEXT_LENGTH] = K_PLUS_R;
char key[21] = {'\0'};

const char charList[sizeof CHAR_LIST] = CHAR_LIST;
#define decodeLetter(e) charList[(e - OFFSET) % (sizeof(CHAR_LIST) - 1)]

FILE *file = NULL;

// Modulus that is equal to the modulus in Scratch as long as b > 0
#define MOD(a, b)							\
  (									\
    ((a) == 0) ? 0 :/* 0 mod b = 0 */					\
    (									\
      ((a) < 0) /* check sign of a */					\
   									\
      /* if a is negative */						\
      ? ( (labs(a) < (b)) ?						\
	  ((b) - labs(a))						\
	  : ((b) - (labs(a) % (b))) )					\
   									\
      /* if a is positive */						\
      : (  ((a) < (b)) ?						\
	   (a)								\
	   : ((a) % (b))  )						\
    )   								\
  )									\

#define prngSetSeed(seed) lastRand = seed;

#define prngNext()							\
  {									\
    if(lastRand == -1812431566)						\
      lastRand = 0;							\
    myRand = MOD((lastRand + 2147483645) * 1430713 + 1240931, 4294967291); \
    myRand = MOD(myRand * 29036779 + 60899, 4294967291);		\
    myRand -= 2147483645;						\
    lastRand = myRand;							\
    if(lastRand == 0)							\
      lastRand = -1812431566;						\
    myRand = lastRand;							\
  }									\

fastest t, spaces;

int main(void)
{
  file = fopen(OUTPUT_FILE, "a");
  fprintf(file, "\n/** NEW RUN **/\n");
  
  for(counter = -2147483645; counter < 2147483646; counter++) {
    if(counter == 0)
      counter++;

    // for i = 0; we don't want it to begin with a space
    prngSetSeed(counter);
    prngNext();
    t = (kandr[0] - myRand - OFFSET) % (sizeof(CHAR_LIST) - 1);
    if(t > 10) {
      key[0] = charList[t];
      for(i = 1; i < 20; i++) {
	prngNext();
	t = (kandr[i] - myRand - OFFSET) % (sizeof(CHAR_LIST) - 1);
	if(t > 0 && t < 11) {
	  goto SKIP;
	}
	key[i] = charList[t];
      }
      fprintf(file, "%s\n", key);
      //printf("POSSIBLE KEY: %s\n", key);
    }
  SKIP:
    if((counter + 2147483645) % 10000000 == 0)
      printf("%i\n", counter);
  }

  printf("Done\n");
  fclose(file);
  return 0;
}
