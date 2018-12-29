#ifndef _FNV_HEADER_
#define _FNV_HEADER_
// This hash is from http://isthe.com/chongo/tech/comp/fnv

// 32 bit FNV-1a non-zero initial basis
#define FNV1_32A_INIT ((unsigned)0x811c9dc5)

// external function
unsigned fnvHash(const char* buff, unsigned hashVal = FNV1_32A_INIT);

#endif