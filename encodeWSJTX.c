#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <math.h>       /* for pow() */

#define MAXRECVSTRING 1024  /* Longest string to receive */


void read_double(uint8_t **pointer, double *value) {
  uint64_t bits, mantissa, *ppointer;
  uint8_t *bitsp, *pointerp;
  int sign;
  uint32_t exponent;
  int32_t sexponent;
  double f;

  bitsp = (uint8_t *)&bits;
  pointerp = *pointer + 7;
  for (int i = 0; i < 8; i++) {
    *bitsp = *pointerp;
    bitsp++;
    pointerp--;
  }
  ppointer = (uint64_t *)*pointer;
  printf("read_double: **pointer=%016lx bits=%016lx\n", *ppointer, bits);

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
  printf("*value=%.1f ", *value);
//  printf(" bits: %lu ", bits);
//  printf(" **pointer: %lu ", (uint64_t)**pointer);
//  for (int i = 0; i < 8; i++)
//    printf("%02x", (char)(&bits)[i] & 0xff);
  printf("\n");
  *pointer += 8;
}

int main(int argc, char *argv[])
{
  double f, af, c, duoble;
  uint64_t exponent, sign, bits, mantissa, rbits;
  uint8_t *bitsp, *rbitsp;

  if (argc != 2) {
    printf("Usage: %s float\n", argv[0]);
    return 1;
  }

  f = atof(argv[1]);
  af = fabs(f);

  sign = (f < 0) ? 1 : 0;
  exponent = (uint64_t)(log(af)/log(2.0) + 1023);
  mantissa = (uint64_t)((af / pow(2, floor(log(af)/log(2.0))) - 1) * pow(2, 52));

  bits = (sign & 0x1) << 63 | (exponent & 0x7ff) << 52 | mantissa & 0xfffffffffffff;

  rbits = ((uint64_t)htonl(bits & 0xffffffff) << 32) | htonl(bits >> 32);

  printf("sign=%lu exponent=%lx mantissa=%lx bits=%016lx rbits=%016lx\n",
    sign, exponent, mantissa, bits, rbits);

  rbitsp = (char *)&rbits;
  read_double(&rbitsp, &duoble);

  exit(0);
}
