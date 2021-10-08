
  f = atof(argv[1]);
  af = fabs(f);

  sign = (f < 0) ? 1 : 0;
  exponent = (uint64_t)(log(af)/log(2.0) + 1023);
  mantissa = (uint64_t)((af / pow(2, floor(log(af)/log(2.0))) - 1) * pow(2, 52));

  bits = (sign & 0x1) << 63 | (exponent & 0x7ff) << 52 | mantissa & 0xfffffffffffff;

  rbits = ((uint64_t)htonl(bits & 0xffffffff) << 32) | htonl(bits >> 32);

  printf("sign=%lu exponent=%lx mantissa=%lx bits=%016lx rbits=%016lx\n",
    sign, exponent, mantissa, bits, rbits);

