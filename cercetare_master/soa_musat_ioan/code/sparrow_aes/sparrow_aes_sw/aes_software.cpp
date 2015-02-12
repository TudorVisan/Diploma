/*
 * AES_software.cpp
 *
 *  Created on: 07.07.2014
 *      Author: Cristina
 */

#include "rijndael.h"
#include "aes_software.h"

#define KEYBITS 128

void xor_sw(const uint8_t* text, const uint8_t* vector, uint8_t* rezultat)
{
	int i;

	for(i=0; i<16; i++)
		rezultat[i] = text[i] ^ vector[i];
}


// ------------------------ Software -------------------------------------

int encrypt_ECB_sw(const uint8_t* key, const uint8_t* plaintext, uint8_t length, uint8_t* ciphertext)
{
	 unsigned long rk[RKLENGTH(KEYBITS)] = {0};
	 int i, padding, nrounds;

	 nrounds = rijndaelSetupEncrypt(rk, key, 128);

	 padding = (16 - (length % 16)) % 16;

	// for (i=length; i<length+padding; i++)
	//	 plaintext[i] = 0;

	 length = length + padding;

	 for (i=0; i<length; i+=16)
		 rijndaelEncrypt(rk, nrounds, plaintext+i, ciphertext+i);

	 return length;
}

int decrypt_ECB_sw(const uint8_t* key, const uint8_t* ciphertext, uint8_t length, uint8_t* plaintext)
{
	 unsigned long rk[RKLENGTH(KEYBITS)] = {0};
	 int i, nrounds;

	 nrounds = rijndaelSetupDecrypt(rk, key, 128);

	 for (i=0; i<length; i+=16)
		 rijndaelDecrypt(rk, nrounds, ciphertext+i, plaintext+i);

	 return length;
}


// ------------------ CBC ----------------

int encrypt_CBC_sw(const uint8_t* key, const uint8_t* plaintext, uint8_t length, uint8_t* ciphertext)
{
	int blocks, padding, nrounds;
	uint8_t IV[16] = {0};
	unsigned long rk[RKLENGTH(KEYBITS)] = {0};

	nrounds = rijndaelSetupEncrypt(rk, key, 128);
	xor_sw(plaintext, IV, IV);
	rijndaelEncrypt(rk, nrounds, IV, ciphertext);

	padding = (16 - (length % 16)) % 16;
	length = length + padding;

	for (blocks=16; blocks<length; blocks+=16)
	{
		xor_sw(ciphertext + blocks - 16, plaintext + blocks, IV);
		rijndaelEncrypt(rk, nrounds, IV, ciphertext + blocks);
	}

	return length;
}

void decrypt_CBC_sw(const uint8_t* key, const uint8_t* ciphertext, uint8_t length, uint8_t* plaintext)
{
	int blocks, padding, nrounds;
	uint8_t IV[16] = {0};
	unsigned long rk[RKLENGTH(KEYBITS)] = {0};

	nrounds = rijndaelSetupDecrypt(rk, key, 128);

	padding = (16 - (length%16)) % 16;
	length = length + padding;

	for (blocks=0; blocks < length; blocks+=16)
	{
		rijndaelDecrypt(rk, nrounds, ciphertext+blocks, plaintext+blocks);

		if (blocks==0)
			xor_sw(IV, plaintext, plaintext);
		else
			xor_sw(ciphertext + blocks - 16, plaintext + blocks, plaintext + blocks);
	}

}


// ------------------ CFB ----------------

int encrypt_CFB_sw(const uint8_t* key, const uint8_t* plaintext, uint8_t length, uint8_t* ciphertext)
{
	int blocks, nrounds;
	uint8_t IV[16] = {0};
	unsigned long rk[RKLENGTH(KEYBITS)] = {0};

	nrounds = rijndaelSetupEncrypt(rk, key, 128);

	rijndaelEncrypt(rk, nrounds, IV, ciphertext);
	xor_sw(plaintext, ciphertext, ciphertext);

	//padding = (16 - (length % 16)) % 16;
	//length = length + padding;

	for (blocks=16; blocks<length; blocks+=16)
	{
		rijndaelEncrypt(rk, nrounds, ciphertext - blocks, ciphertext + blocks);
		xor_sw(ciphertext + blocks, plaintext + blocks, ciphertext + blocks);

	}

	return length;
}

void decrypt_CFB_sw(const uint8_t* key, const uint8_t* ciphertext, uint8_t length, uint8_t* plaintext)
{
	int blocks, nrounds;
	uint8_t IV[16] = {0};
	unsigned long rk[RKLENGTH(KEYBITS)] = {0};

	nrounds = rijndaelSetupEncrypt(rk, key, 128);

	rijndaelEncrypt(rk, nrounds, IV, plaintext);
	xor_sw(ciphertext, plaintext, plaintext);

	//padding = (16 - (length%16)) % 16;
	//length = length + padding;

	for (blocks=16; blocks < length; blocks+=16)
	{
		rijndaelEncrypt(rk, nrounds, ciphertext - blocks, plaintext + blocks);
		xor_sw(ciphertext + blocks, plaintext + blocks, plaintext + blocks);

	}

}

// ------------------ CTR ----------------

int encrypt_CTR_sw(const uint8_t* key, const uint8_t* plaintext, uint8_t length, uint8_t* ciphertext)
{
	int blocks, nrounds;
	uint8_t counter[16] = {0};
	unsigned long rk[RKLENGTH(KEYBITS)] = {0};

	nrounds = rijndaelSetupEncrypt(rk, key, 128);

	for (blocks=0; blocks<length; blocks+=16)
	{
		rijndaelEncrypt(rk, nrounds, counter, ciphertext + blocks);
		xor_sw(ciphertext + blocks, plaintext + blocks, ciphertext + blocks);
		counter[0]++;
	}

	return length;
}

void decrypt_CTR_sw(const uint8_t* key, const uint8_t* ciphertext, uint8_t length, uint8_t* plaintext)
{
	int blocks, nrounds;
	uint8_t counter[16] = {0};
	unsigned long rk[RKLENGTH(KEYBITS)] = {0};

	nrounds = rijndaelSetupEncrypt(rk, key, 128);

	for (blocks=0; blocks<length; blocks+=16)
	{
		rijndaelEncrypt(rk, nrounds, counter, plaintext + blocks);
		xor_sw(plaintext + blocks, ciphertext + blocks, plaintext + blocks);
		counter[0]++;
	}

}
