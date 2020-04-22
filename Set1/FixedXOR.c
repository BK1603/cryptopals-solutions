#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int ctox(char ch) {
  if(ch >= '0' && ch <= '9')
    return (int)(ch - '0');
  if(ch >= 'a' && ch <= 'f')
    return ((int)ch - 87);
}

int main() {
  char buffer1[] = "1c0111001f010100061a024b53535009181c";
  char buffer2[] = "686974207468652062756c6c277320657965";
  if(strlen(buffer1) != strlen(buffer2)) {
    puts("Buffers of different length");
    return 1;
  }
  size_t len = strlen(buffer1);
  len = ceil(len/6.0);

  // ------------ parse hex --------------
  // copied from the previous problem code.
  // TODO: Make it a function.
  unsigned int *hex1 = (int *)malloc(sizeof(int) * len);
  unsigned int *hex2 = (int *)malloc(sizeof(int) * len);
  memset(hex1, 0, sizeof(hex1));
  memset(hex2, 0, sizeof(hex2));
  int i, j;
  for(i = 0, j = 0; i < strlen(buffer1); i++) {
    int k;
    for(k = 0; k < 5; k++) {
      if(i < strlen(buffer1)) {
        hex1[j] |= ctox(buffer1[i]);
        hex2[j] |= ctox(buffer2[i]);
      }
      hex1[j] <<= 4;
      hex2[j] <<= 4;
      i++;
    }
    hex1[j] |= (i >= strlen(buffer1)) ? 0 : ctox(buffer1[i]);
    hex2[j] |= (i >= strlen(buffer2)) ? 0 : ctox(buffer2[i]);
    j++;
  }

  // -------------- actual xor ----------------
  unsigned int *hex3 = (int *)malloc(sizeof(int) * len);
  memset(hex3, 0, sizeof(hex3));

  for(i = 0; i < len; i++) {
    hex3[i] = hex2[i] ^ hex1[i];
    printf("%x", hex3[i]);
  }

  printf("\n");
  free(hex1);
  free(hex2);
  free(hex3);
  return 0;
}
