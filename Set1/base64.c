#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

/*
 * An implementation of the base64 algorithm. It is the solution
 * to the first problem in set 1. Still got to work on the padding part.
 * Algo source: http://www.herongyang.com/Encoding/Base64-Encoding-Algorithm.html
 */

char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char pad = '=';

int ctox(char ch) {
  if(ch >= '0' && ch <= '9')
    return (int)(ch - '0');
  if(ch >= 'a' && ch <= 'f')
    return ((int)ch - 87);
}

int main() {
  // Input string.
  char buffer[] = "a4";
  size_t len = strlen(buffer);
  // The length of the integer array.
  len = ceil(len/6.0);

  // First break the input bytes into groups of 3 bytes.
  // Will store 24 bits in a single integer
  // Since integer is 4 bytes (hex) each int will be assigned
  // a value 6 times. (In the k loop.)
  unsigned int *hex = (int *)malloc(sizeof(int) * len);
  memset(hex, 0, sizeof(hex));
  int i, j, pad = 0;
  for(i = 0, j = 0; i < strlen(buffer); i++) {
    int k;
    for(k = 0; k < 5; k++) {
      if(i < strlen(buffer))
        hex[j] |= ctox(buffer[i]);
      hex[j] <<= 4;
      i++;
    }
    // assign the last value without shifting.
    hex[j] |= (i >= strlen(buffer)) ? 0 : ctox(buffer[i]);
    j++;
  }
  int total_bytes = strlen(buffer) / 2;
  pad = 3 - (total_bytes%3);
  printf("%d\n", pad);

  // ------------ convert to base 64 here ------
  int written = 0;                // to store number of bytes written
  for(i = 0, j = 0; j < len; j++) {
    int mask = 0xfc0000;
    for(int k = 0; k < 4; k++) {
      written = i/8;
      if(total_bytes - written > 0)       // if there is still something to write.
        // get the first 6 bits by & fc0000 (since we stored
        // 24 bits, so the highest bit is in the 24th pos.)
        // the shift right and repeat.
        printf("%c", base64[(hex[j] & mask) >> (18 - k*6)]);
      else                                // what follows is the padding
        printf("%c", pad);
      mask >>= 6;
      i += 6;
    }
  }
  printf("\n");
  free(hex);
  return 0;
}
