#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

/*
 * Used variance as a score for the current string.
 * The string with minimum variance will be the answer.
 */

double *efreq;

int ctox(char ch) {
  if(ch >= '0' && ch <= '9')
    return (int)(ch - '0');
  if(ch >= 'a' && ch <= 'f')
    return ((int)ch - 87);
}

/*
 * Initialize the expected word frequence.
 * Read a book hg.txt for the purpose.
 */
void init() {
  unsigned long long int total = 0;

  efreq = (double *)malloc(sizeof(double) *256);
  for(int i = 0; i < 256; i++) {
    efreq[i] = 0;
  }
  FILE *fp;
  fp = fopen("hg.txt", "r");
  char ch;
  while((ch = fgetc(fp)) != EOF) {
    total++;
    efreq[(int)ch]++;
  }
  fclose(fp);

  for(int i = 0; i < 256; i++)
    efreq[i] /= total;
}

int main() {
  init();
  // Input string.
  char buffer[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  size_t len = strlen(buffer);
  // The length of the integer array.
  len = ceil(len/8.0);

  // Getting hex to bytes.
  unsigned int *hex = (int *)malloc(sizeof(int) * len);
  int i, j;
  for(i = 0, j = 0; i < strlen(buffer); i++) {
    int k;
    hex[j] = 0;
    for(k = 0; k < 7; k++) {
      if(i < strlen(buffer))
        hex[j] |= ctox(buffer[i]);
      hex[j] <<= 4;
      i++;
    }
    // assign the last value without shifting.
    hex[j] |= (i >= strlen(buffer)) ? 0 : ctox(buffer[i]);
    printf("%x ", hex[j]);
    j++;
  }
  printf("\n");

  double freq[256];
  double chi = 100.0;
  int key;
  // Check all single character keys possible
  for(int k = 0; k < 0xffffffff; k += 0x01010101) {
    for(int i = 0; i < len; i++) {
      hex[i] ^= k;
    }
    double c = 0.0d;
    double sum = 0.0d;
    // Initialize freq
    for(int i = 0; i < 256; i++)
      freq[i] = 0;
    // XOR the input with the key
    for(i = 0; i < len; i++) {
      for(int l = 0; l < 4; l++) {
        unsigned int x = (hex[i] & (0x000000ff << (3 - l) * 8)) >> ((3 - l) * 8);
        freq[x]++;
      }
    }

    // Calculate freq
    for(int i = 0; i < 256; i++)
      sum += freq[i];

    for(int i = 0; i < 256; i++)
      freq[i] /= sum;

    // Store the variance
    for(int i = 0; i < 256; i++) {
      c += pow(freq[i] - efreq[i], 2);
    }

    // Check for minimum variance
    if(c < chi) {
      chi = c;
      key = k;
    }

    // Restore the input to original value
    for(int i = 0; i < len; i++)
      hex[i] ^= k;
  }

  // Print the key with the lowest variance wrt efreq
  printf("%f\n", chi);
  printf("%x\n", key);
  for(int i = 0; i < len; i++) {
    printf("%08x ", hex[i] ^ key);
  }
  printf("\n");
  free(hex);
  free(efreq);
}
