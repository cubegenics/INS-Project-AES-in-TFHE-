/* "structures.h" defines the look-up tables and KeyExpansion function 
 * used in encrypt.cpp and decrypt.cpp
 */
#ifndef STRUCTURES_H
#define STRUCTURES_H

// Encryption: Forward Rijndael S-box 
extern unsigned char s[256];

// Encryption: Multiply by 2 for MixColumns
extern unsigned char mul2[256];

// Encryption: Multiply by 3 for MixColumns
extern unsigned char mul3[256];

// Used in KeyExpansion
extern unsigned char rcon[256];

// Decryption: Inverse Rijndael S-box
extern unsigned char inv_s[256];

// Decryption: Multiply by 9 for InverseMixColumns
extern unsigned char mul9[256];

// Decryption: Multiply by 11 for InverseMixColumns
extern unsigned char mul11[256];

// Decryption: Multiply by 13 for InverseMixColumns
extern unsigned char mul13[256];

// Decryption: Multiply by 14 for InverseMixColumns
extern unsigned char mul14[256];

// Auxiliary function for KeyExpansion
void KeyExpansionCore(unsigned char * in, unsigned char i);


/* The main KeyExpansion function
 * Generates additional keys using the original key
 * Total of 11 128-bit keys generated, including the original
 * Keys are stored one after the other in expandedKeys
 */
void KeyExpansion(unsigned char inputKey[16], unsigned char expandedKeys[176]);


#endif /* STRUCTURES_H */
