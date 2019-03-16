#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <math.h>       /* for pow() */

#define MAXRECVSTRING 1024  /* Longest string to receive */

void DieWithError(char *errorMessage);  /* External error handling function */

void read_string(char **pointer, char *string) {
  int32_t len, rlen;
  memcpy(&rlen, *pointer, 4);
  len = ntohl(rlen);
//  printf("\nread_string: len=%d\n", len);
  if (len != -1) {
    memcpy(string, *pointer + 4, len);
    string[len] = '\0';
    *pointer += len + 4;
  }
  else {
    string[0] = '\0';
    *pointer += 4;
  }
}

void read_int4(char **pointer, int32_t *value) {
  int32_t rvalue;
  memcpy(&rvalue, *pointer, 4);
  *value = ntohl(rvalue);
//  printf("read_int4: *value=%d ", *value);
  *pointer += 4;
}

void read_int1(char **pointer, char *value) {
  *value = **pointer;
  *pointer += 1;
}

void read_double(char **pointer, double *value) {
  int64_t bits;
  char sign;
  int32_t exponent;
  int64_t mantissa;

//  for (int i = 0; i < 8; i++)
//    memcpy(&bits + i, *pointer + 7 - i, 1);

  memcpy(&bits, *pointer, 8);
  sign = ((bits >> 63) == 0) ? 1 : -1;
  exponent = ((bits >> (63 - 12)) & 0xfff) - 1023;
  mantissa = (exponent == 0) ?
        (bits & 0x7fffffffffffff) << 1 :
        (bits & 0x7fffffffffffff) | 0x80000000000000;
  *value = sign * mantissa * pow(2.0, (double)(exponent - 55));

  printf("read_double: sign=%d exponent=%d ", (int)sign, exponent);
//  for (int i = 0; i < 8; i++)
//    printf("%02x", (char)*(value + i) & 0xff);
//  printf("*value=%.1f ", *value);
  printf(" Pointer: ");
  for (int i = 0; i < 8; i++)
    printf("%02x", (char)*(*pointer + i) & 0xff);
  printf("\n");
  *pointer += 8;
}

int main(int argc, char *argv[])
{
    char *dst;
    char int1;
    int32_t i, msg, int4;
    int sock;                         /* Socket */
    struct sockaddr_in broadcastAddr; /* Broadcast Address */
    unsigned short broadcastPort;     /* Port */
    char recvString[MAXRECVSTRING+1]; /* Buffer for received string */
    int recvStringLen;                /* Length of received string */
    char string[64];
    double duoble;

    if (argc != 2)    /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage: %s <Broadcast Port>\n", argv[0]);
        exit(1);
    }

    broadcastPort = atoi(argv[1]);   /* First arg: broadcast port */

    /* Create a best-effort datagram socket using UDP */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct bind structure */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
    broadcastAddr.sin_port = htons(broadcastPort);      /* Broadcast port */

    /* Bind to the broadcast port */
    if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0)
        DieWithError("bind() failed");

    for (msg = 0; msg < 10; msg++) {

      /* Receive a single datagram from the server */
      if ((recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, NULL, 0)) < 0)
          DieWithError("recvfrom() failed");

//      recvString[recvStringLen] = '\0';

      switch (recvString[11]) {
        case 0: printf("Heartbeat: ");
                dst = recvString + 12;
                read_string(&dst, string); printf("ID: \"%s\" ", string);
                break;
        case 1: printf("Status:    ");
                dst = recvString + 12;
                read_string(&dst, string); printf("ID: \"%s\" ", string);
                read_int4(&dst, &int4);
                read_int4(&dst, &int4); printf("bfreq: %d ", int4);
                read_string(&dst, string); printf("Mode: \"%s\" ", string);
                read_string(&dst, string); printf("Call: \"%s\" ", string);
                read_string(&dst, string); printf("SNR: \"%3s\" ", string);
                read_string(&dst, string); printf("Mode: \"%s\" ", string);
                read_int1(&dst, &int1);
                read_int1(&dst, &int1);
                read_int1(&dst, &int1);
                read_int4(&dst, &int4);
                read_int4(&dst, &int4);
                read_string(&dst, string); printf("de Call: \"%s\" ", string);
                read_string(&dst, string); printf("DE grid: \"%s\" ", string);
                read_string(&dst, string); printf("DX grid: \"%s\" ", string);
                read_int1(&dst, &int1);
                read_string(&dst, string); 
                break;
        case 2: printf("Decode:    ");
                dst = recvString + 12;
                read_string(&dst, string); printf("ID: \"%s\" ", string);
                read_int1(&dst, &int1); // New decode
                read_int4(&dst, &int4); // Time
                read_int4(&dst, &int4); printf("SNR: %3d ", int4);
                printf("dt hex: ");
                for (int i = 0; i <8; i++)
                  printf("%02x%s", dst[i] & 0xff, (i == 7) ? " " : ".");
                read_double(&dst, &duoble); printf("dt: %.1e ", duoble);
                read_int4(&dst, &int4); printf("Frq: %4d ", int4);
                read_string(&dst, string); printf("Rx Mode: \"%s\" ", string);
                read_string(&dst, string); printf("Message: \"%s\" ", string);
                read_int1(&dst, &int1);
                read_int1(&dst, &int1);
                break;
        default: printf("Unsupported ");
      }
      printf("\n");
//    printf("Received: %s\n", recvString);    /* Print the received string */

    } // for # of messages

    close(sock);
    exit(0);
}
