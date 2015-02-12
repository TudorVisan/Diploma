/*
 * AES_software.h
 *
 *  Created on: 07.07.2014
 *      Author: Cristina
 */

#ifndef AES_SOFTWARE_H_
#define AES_SOFTWARE_H_

#include "stdint.h"

int encrypt_ECB_sw(const uint8_t* key, const uint8_t* plaintext, uint8_t length, uint8_t* ciphertext);
int decrypt_ECB_sw(const uint8_t* key, const uint8_t* ciphertext, uint8_t length, uint8_t* plaintext);

int encrypt_CBC_sw(const uint8_t* key, const uint8_t* plaintext, uint8_t length, uint8_t* ciphertext);
void decrypt_CBC_sw(const uint8_t* key, const uint8_t* ciphertext, uint8_t length, uint8_t* plaintext);

int encrypt_CFB_sw(const uint8_t* key, const uint8_t* plaintext, uint8_t length, uint8_t* ciphertext);
void decrypt_CFB_sw(const uint8_t* key, const uint8_t* ciphertext, uint8_t length, uint8_t* plaintext);

int encrypt_CTR_sw(const uint8_t* key, const uint8_t* plaintext, uint8_t length, uint8_t* ciphertext);
void decrypt_CTR_sw(const uint8_t* key, const uint8_t* ciphertext, uint8_t length, uint8_t* plaintext);


#endif /* AES_SOFTWARE_H_ */
