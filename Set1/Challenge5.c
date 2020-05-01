#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * Get the integer hex equivalent from
 * character.
 */
int ctox(char ch) {
  if(ch >= '0' && ch <= '9')
    return (int)(ch - '0');
  if(ch >= 'a' && ch <= 'f')
    return ((int)ch - 87);
}

/*
 * Get hexadecimal from ASCII string
 */
int *getHex(char *str) {
  int len = ceil(strlen(str) / 4.0);
  int *hex = (int *)malloc(sizeof(int) * (len + 1));
  memset(hex, 0, sizeof(int) * (len + 1));

  hex[0] = len;
  for(int i = 1, k = 0; i <= len; i++) {
    for(int j = 0; j < 3; j++) {
      if(k < strlen(str))
        hex[i] |= str[k];
      hex[i] <<= 8;
      k++;
    }
    hex[i] |= (k < strlen(str) ? str[k] : 0);
    k++;
  }

  return hex;
}

int *encryptRepXOR(int *hex, char *key) {
  int l = strlen(key);
  int *enc = (int *)malloc(sizeof(int) * hex[0]);
  enc[0] = hex[0];

  int k = 0;
  for(int i = 1; i <= hex[0]; i++) {
    for(int j = 0; j < 4; j++) {
      enc[i] |= hex[i] & (0xff000000 >> 8 * j);
      enc[i] ^= key[k] << (8 * (3 - j));
      k++;
      k %= l;
    }
  }

  return enc;
}

void printHex(int *hex) {
  for(int i = 1; i <= hex[0]; i++)
    printf("%08x ", hex[i]);
  printf("\n");
}

int main() {
  int *hex = getHex("Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal");
  int *enc = encryptRepXOR(hex, "ICE");
  printHex(enc);
  free(enc);
  free(hex);
  return 0;
}
