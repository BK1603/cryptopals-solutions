#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

/*
 * An implementation of the base64 algorithm. It is the solution
 * to the first problem in set 1. Still got to work on the padding part
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
  char buffer[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  size_t len = strlen(buffer);
  // The length of the integer array.
  len = ceil(len/6.0);

  // First break the input bytes into groups of 3 bytes.
  // Will store 24 bits in a single integer
  // Since integer is 4 bytes (hex) each int will be assigned
  // a value 6 times. (In the k loop.)
  unsigned int *hex = (int *)malloc(sizeof(int) * len);
  memset(hex, 0, sizeof(hex));
  int i, j;
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
  printf("\n");

  // ------------ convert to base 64 here ------
  char answer[len * 4];
  for(i = 0, j = 0; j < len; j++) {
    for(int k = 0; k < 4; k++) {
      // get the first 6 buts by & fc0000 (since we stored
      // 24 bits, so the highest bit is in the 24th pos.)
      // the shift right and repeat.
      printf("%c", base64[(hex[j] & 0xfc0000) >> 18]);
      hex[j] <<= 6;
    }
    // TODO: padding code comes here.
  }
  free(hex);
  return 0;
}
