#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <math.h>       /* for pow() */

#define MAXRECVSTRING 1024  /* Longest string to receive */


void read_double(char **pointer, double *value) {
  uint64_t bits, mantissa;
  char *bitsp, *pointerp;
  char sign;
  uint32_t exponent;
  int32_t sexponent;

  bitsp = (char *)&bits;
  pointerp = *pointer + 7;
  for (int i = 0; i < 8; i++) {
    *bitsp = *pointerp;
    bitsp++;
    pointerp--;
  }

  sign = ((bits >> 63) == 0) ? 1 : -1;
  exponent = (bits >> 52) & 0x7ff;
  mantissa = bits & 0xfffffffffffff;

  sexponent = exponent - 1023;
  if (mantissa == 0 && exponent == 0 && sign == 0)
    *value = 0.0;
  else
    *value = sign * (1.0 + (double)mantissa / pow(2.0, 52)) * pow(2.0, (double)(sexponent));

  printf("read_double: sign=%d sexponent=%d ", (int)sign, sexponent);
//  for (int i = 0; i < 8; i++)
//    printf("%02x", (char)*(value + i) & 0xff);
//  printf("*value=%.1f ", *value);
//  printf(" bits: %lu ", bits);
//  printf(" **pointer: %lu ", (uint64_t)**pointer);
//  for (int i = 0; i < 8; i++)
//    printf("%02x", (char)(&bits)[i] & 0xff);
  printf("\n");
  *pointer += 8;
}

int main(int argc, char *argv[])
{
    char *dst, *bitsp;
    char int1;
    int32_t i, msg, int4;
    double duoble;
    int64_t bits;

  uint64_t bits, mantissa;
  char *bitsp, *pointerp;
  char sign;
  uint32_t exponent;
  int32_t sexponent;

  bitsp = (char *)&bits;
  pointerp = *pointer + 7;
  for (int i = 0; i < 8; i++) {
    *bitsp = *dst;
    bitsp++;
    pointerp--;
  }

  sign = ((bits >> 63) == 0) ? 1 : -1;
  exponent = (bits >> 52) & 0x7ff;
  mantissa = bits & 0xfffffffffffff;

  sexponent = exponent - 1023;
  if (mantissa == 0 && exponent == 0 && sign == 0)
    *value = 0.0;
  else
    *value = sign * (1.0 + (double)mantissa / pow(2.0, 52)) * pow(2.0, (double)(sexponent));

  printf("read_double: sign=%d sexponent=%d ", (int)sign, sexponent);
//  for (int i = 0; i < 8; i++)
//    printf("%02x", (char)*(value + i) & 0xff);
//  printf("*value=%.1f ", *value);
//  printf(" bits: %lu ", bits);
//  printf(" **pointer: %lu ", (uint64_t)**pointer);
//  for (int i = 0; i < 8; i++)
//    printf("%02x", (char)(&bits)[i] & 0xff);
  printf("\n");

  read_double(*pointer, &duoble);



    exit(0);
}
