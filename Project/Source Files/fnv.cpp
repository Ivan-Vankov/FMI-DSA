#include "fnv.h"
// 
// fnvHash - perform a 32 bit Fowler/Noll/Vo FNV-1a hash on a string
//
// input:
//	str	- string to hash
//	hval	- previous hash value or 0 if first call
//
// returns:
//	32 bit hash as a static hash type
//
unsigned fnvHash(const char *str, unsigned hval) {
	unsigned char *s = (unsigned char *)str;
	// FNV-1a hash each octet in the buffer
	while (*s) {
		// xor the bottom with the current octet 
		hval ^= (unsigned)*s++;
		// add a *magic* number to hval
		hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);
	}
	// return our new hash value
	return hval;
}