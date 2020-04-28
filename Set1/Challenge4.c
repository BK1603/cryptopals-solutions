#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

double *efreq;

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

/*
 * Parse hex from the string.
 */
unsigned int *getHex(char *buffer) {
  // Getting hex to bytes.
  unsigned int len = ceil(strlen(buffer)/8.0);
  unsigned int *hex = (int *)malloc(sizeof(int) * (len + 1));
  int i, j;
  memset(hex, 0, sizeof(unsigned int) * len);
  hex[0] = len;
  for(i = 0, j = 1; i < strlen(buffer); j++) {
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
    i++;
  }
  return hex;
}

/*
 * Caculate and return the variance of the frequency table
 * wrt frequency from hg.txt
 */
double score(double *freq) {
  double chi = 0.0d;
  for(int i = 0; i < 256; i++) {
    chi += pow(freq[i] - efreq[i], 2);
  }
  return chi;
}

/*
 * Calculate the key using variance.
 * The key that results in the lowest variance is returned.
 * (Goodfit test.)
 */
int getKeyUsingVariance(unsigned int *hex) {
  int len = hex[0];
  int key = 0;
  double chi = 100.0d;
  double sum = 0.0d;
  double freq[256];
  for(int k = 0x00000000; k < 0xffffffff; k += 0x01010101) {
    for(int i = 1; i <= len; i++) {
      hex[i] = hex[i] ^ k;
    }
    // Initialize freq
    for(int i = 0; i < 256; i++)
      freq[i] = 0.0d;
    // Calculate freq for XORed array ----------------------------
    for(int i = 1; i <= len; i++) {
      for(int l = 0; l < 4; l++) {
        unsigned int x = (hex[i] & (0x000000ff << (3 - l) * 8)) >> ((3 - l) * 8);
        freq[x]++;
      }
    }

    for(int i = 0; i < 256; i++)
      sum += freq[i];

    for(int i = 0; i < 256; i++)
      freq[i] /= sum;
    //------------------------------------------------------------

    double c = score(freq);

    if(c < chi) {
      chi = c;
      key = k;
    }
    for(int i = 1; i <= len; i++)
      hex[i] = k ^ hex[i];
  }
  return key;
}

/*
 * XOR the hex against the key.
 */
void xor(unsigned int *hex, int key) {
  for(int i = 1; i <= hex[0]; i++)
    hex[i] = hex[i] ^ key;
}

/*
 * Print the hex key.
 */
void printHex(int *hex) {
  for(int i = 1; i <= hex[0]; i++) {
    printf("%08x ", hex[i]);
  }
  printf("\n");
}

/*
 * Approach: Calculate the variance of all the lines in the text file
 * using means as the frequencies of each character in english language
 * (eg: a long book). The one with the maximum frequence is not in
 * english.
 * To decode it, since it is a single character key, xor it against all
 * 256 possible values, the one with the lowest frequency is the answer
 */
int main() {
  init();
  FILE *fp;
  fp = fopen("hex.txt", "r");
  char buffer[60];
  int linenr = 0;
  int m_hex[9];
  double chi = -1;
  while(!feof(fp)) {
    fscanf(fp, "%s", buffer);
    unsigned int *hex = getHex(buffer);
    int len = hex[0];
    int sum = 0;
    double freq[256];
    memset(freq, 0, sizeof(double) * 256);

    // Calculate frequency -------------------------------------------
    for(int i = 1; i <= len; i++) {
      for(int l = 0; l < 4; l++) {
        unsigned int x = (hex[i] & (0x000000ff << (3 - l) * 8)) >> ((3 - l) * 8);
        freq[x]++;
      }
    }

    for(int i = 0; i < 256; i++)
      sum += freq[i];

    for(int i = 0; i < 256; i++)
      freq[i] /= sum;
    //----------------------------------------------------------------

    double c = score(freq);

    // store one with the most score
    if (c > chi) {
      chi = c;
      for (int i = 0; i <= hex[0]; i++)
        m_hex[i] = hex[i];
    }
    free(hex);
  }
  fclose(fp);

  printf("%lf \n", chi);
  printHex(m_hex);

  int key = getKeyUsingVariance(m_hex);
  printf("%08x\n", key);
  xor(m_hex, key);

  printHex(m_hex);

  free(efreq);
  return 0;
}
