#include <stdio.h>
/* gcc -std=c89 -Wall -Wextra gbk_encode.c -o gbk_encode
   echo -en '\r\n<' | ./gbk_encode --> %E5%98%8D%E5%98%8A%E5%98%BC */
int main(void) {
  int c;
  while ((c = getchar()) != EOF) {
    unsigned char ch = (unsigned char)c;
    unsigned int high = (ch >> 4) & 0xF;
    unsigned int low  = ch & 0xF;
    /* Add 8 to the first hex digit, keep one hex digit */
    high = (high + 8) & 0xF;
    printf("%%E5%%98%%%X%X", high, low);
  }
  return 0;
}

