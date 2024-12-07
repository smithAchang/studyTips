#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>


/* RFC 1982 - Serial Number Arithmetic
 *
 * 2. Comparison
 *  Then, s1 is said to be equal to s2 if and only if i1 is equal to i2,
 *  in all other cases, s1 is not equal to s2.
 *
 * s1 is said to be less than s2 if, and only if, s1 is not equal to s2,
 * and
 *
 *      (i1 < i2 and i2 - i1 < 2^(SERIAL_BITS - 1)) or
 *      (i1 > i2 and i1 - i2 > 2^(SERIAL_BITS - 1))
 *
 * s1 is said to be greater than s2 if, and only if, s1 is not equal to
 * s2, and
 *
 *      (i1 < i2 and i2 - i1 > 2^(SERIAL_BITS - 1)) or
 *      (i1 > i2 and i1 - i2 < 2^(SERIAL_BITS - 1))
 */
 
/*
 * RFC 2960 - SCTP relatives
 *  1.6 Serial Number Arithmetic
 *
 * Comparisons and arithmetic on TSNs in this document SHOULD use Serial
 * Number Arithmetic as defined in [RFC1982] where SERIAL_BITS = 32.
 */
 
enum {
	TSN_SIGN_BIT = (1<<31)
};


/*previous version of older linux kernel*/
static inline int TSN_lt(uint32_t s, uint32_t t)
{
	return ((s) - (t)) & TSN_SIGN_BIT;
}

static inline int TSN_lte(uint32_t s, uint32_t t)
{
	return ((s) == (t)) || (((s) - (t)) & TSN_SIGN_BIT);
}

/*new version of linux kernel*/
static inline int TSN_lt2(uint32_t s, uint32_t t)
{
	return (int32_t)((s) - (t)) < 0;
}

static inline int TSN_lte2(uint32_t s, uint32_t t)
{
	return (int32_t)((s) - (t)) <= 0;
}

/* Compare two SSNs */
/*
 * RFC 2960
 *  1.6 Serial Number Arithmetic
 *
 * Comparisons and arithmetic on Stream Sequence Numbers in this document
 * SHOULD use Serial Number Arithmetic as defined in [RFC1982] where
 * SERIAL_BITS = 16.
 */
enum {
	SSN_SIGN_BIT = (1<<15)
};

static inline int SSN_lt(uint16_t s, uint16_t t)
{
	return ((s) - (t)) & SSN_SIGN_BIT;
}

static inline int SSN_lte(uint16_t s, uint16_t t)
{
	return ((s) == (t)) || (((s) - (t)) & SSN_SIGN_BIT);
}

static inline int SSN_lt2(uint16_t s, uint16_t t)
{
	return (int16_t)((s) - (t)) < 0;
}
static inline int SSN_lte2(uint16_t s, uint16_t t)
{
	return (int16_t)((s) - (t)) <= 0;
}

int main(int argc, char* argv[])
{
  printf("Test 32bits TSN wrapped feature ...\n");
  assert(TSN_lte(1, 1));
  assert(TSN_lt(1, 2));
  assert(TSN_lt(1, 0xFFFFFF));
  assert(TSN_lt(0xFFFFFFFF, 1));
  assert(!TSN_lt(0xFFFFFFFF, 0xFFFFFFFE));
  assert(!TSN_lt(0xFFFFFFFF, 0xFFFF0000));
  
  printf("odd behavior, but may accept. the case occurs with little chance\n");
  assert(!TSN_lt(1, 0xFFFFFFFF));
  
  printf("\nTest new version comparing functions ...\n");
  assert(TSN_lte2(1, 1));
  assert(TSN_lt2(1, 2));
  assert(TSN_lt2(1, 0xFFFFFF));
  assert(TSN_lt2(0xFFFFFFFF, 1));
  assert(!TSN_lt2(0xFFFFFFFF, 0xFFFFFFFE));
  assert(!TSN_lt2(0xFFFFFFFF, 0xFFFF0000));
  
  printf("odd behavior, but may accept. the case occurs with little chance\n");
  assert(!TSN_lt2(1, 0xFFFFFFFF));
  
  return 0;
}
