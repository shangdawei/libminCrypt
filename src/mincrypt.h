/*
 *  mincrypt.h: Minimalistic encryption system headers
 *
 *  Copyright (c) 2010-2012, Michal Novotny <mignov@gmail.com>
 *  All rights reserved.
 *
 *  See COPYING for the license of this software
 *
 */

#ifndef MINCRYPT_H
#define MINCRYPT_H

// Comment following line to enable debugging
#define DISABLE_DEBUG

#define MINCRYPT_BANNER			"minCrypt encryption system"
#define BUFFER_SIZE			(1 << 17)			/* Make 128 kB to be default buffer size */
#define BUFFER_SIZE_BASE64		(((BUFFER_SIZE + 2) / 3) * 4)
#define O_LARGEFILE			0x0200000
#define SIGNATURE			"MCF"
#define DEFAULT_SALT_VAL		SIGNATURE
#define DEFAULT_VECTOR_MULT		0x20
#define DEFAULT_NEW_PASSWORD_LENGTH	8

#define ENCODING_TYPE_BASE		0x10
#define ENCODING_TYPE_BINARY		ENCODING_TYPE_BASE
#define ENCODING_TYPE_BASE64		ENCODING_TYPE_BASE + 1

/* Flags for DH-like encryption */
#define	MINCRYPT_FLAG_DHKEY_COMMON_P	0x0000001
#define	MINCRYPT_FLAG_DHKEY_COMMON_G	0x0000002
#define	MINCRYPT_FLAG_DHKEY_COMMON	(MINCRYPT_FLAG_DHKEY_COMMON_P | MINCRYPT_FLAG_DHKEY_COMMON_G)
#define	MINCRYPT_FLAG_DHKEY_PUBLIC	0x0000004
#define	MINCRYPT_FLAG_DHKEY_PRIVATE	0x0000008
#define	MINCRYPT_FLAG_DHVAL_RECEIVER	0x0000001
#define	MINCRYPT_FLAG_DHVAL_SENDER	0x0000002

//#define USE_LARGE_FILE

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#ifdef WINDOWS
#undef O_LARGEFILE
#define	O_LARGEFILE			0
#define	strtok_r(s,d,p)			strtok(s,d)
#endif

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <inttypes.h>
#include <malloc.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <sys/time.h>

typedef struct tTokenizer {
	char **tokens;
	int numTokens;
} tTokenizer;

tTokenizer tokenize(char *string);
void free_tokens(tTokenizer t);

#define UINT32STR(var, val)	\
	var[0] = (val >> 24) & 0xff;	\
	var[1] = (val >> 16) & 0xff;	\
	var[2] = (val >>  8) & 0xff;	\
	var[3] = (val      ) & 0xff;

#define UINT64STR(var, val)	\
	var[0] = (val >> 56) & 0xff;	\
	var[1] = (val >> 48) & 0xff;	\
	var[2] = (val >> 40) & 0xff;	\
	var[3] = (val >> 32) & 0xff;	\
	var[4] = (val >> 24) & 0xff;	\
	var[5] = (val >> 16) & 0xff;	\
	var[6] = (val >>  8) & 0xff;	\
	var[7] = (val      ) & 0xff;

#define BYTESTR(var, val)	\
	var[0] =  val;

#define WORDSTR(var, val)	\
	var[0] = (val >> 8) & 0xff;	\
	var[1] = (val     ) & 0xff;

#define GETBYTE(var)    (var[0])
#define GETWORD(var)    ((var[0] << 8) + (var[1]))
#define GETUINT32(var)	(uint32_t)(((uint32_t)var[0] << 24) + ((uint32_t)var[1] << 16) + ((uint32_t)var[2] << 8) + ((uint32_t)var[3]))
#define GETUINT64(var)	(uint64_t)(((uint64_t)var[0] << 56) + ((uint64_t)var[1] << 48) + ((uint64_t)var[2] << 40) + \
			((uint64_t)var[3] << 32) + ((uint64_t)var[4] << 24) + ((uint64_t)var[5] << 16)  + \
			((uint64_t)var[6] << 8) + (uint64_t)var[7])

typedef struct tPrimes {
	int num;
	uint64_t start;
	uint64_t end;
	uint64_t *numbers;
} tPrimes;

extern char *gQuartet;

#define	BINARY_OPERATION_OR	0x00
#define	BINARY_OPERATION_AND	0x01
#define	BINARY_OPERATION_XOR	0x02

#define	BIT_SET			0x01
#define	BIT_UNSET		0x00

#define	GET_NEAREST_BIGGER	0x00
#define GET_NEAREST_SMALLER	0x01

#define	APPROACH_SYMMETRIC	0x00
#define	APPROACH_ASYMMETRIC	0x01

/* Public functions */
void mincrypt_set_password(char *salt, char *password, int vector_multiplier);
int mincrypt_set_encoding_type(int type);
void mincrypt_dump_vectors(char *dump_file);
int mincrypt_read_key_file(char *keyfile, int *oIsPrivate);
void mincrypt_cleanup(void);
unsigned char *mincrypt_encrypt(unsigned char *block, size_t size, int id, size_t *new_size);
unsigned char *mincrypt_decrypt(unsigned char *block, size_t size, int id, size_t *new_size, int *read_size);
int mincrypt_encrypt_file(char *filename1, char *filename2, char *salt, char *password, int vector_multiplier);
int mincrypt_decrypt_file(char *filename1, char *filename2, char *salt, char *password, int vector_multiplier);
int mincrypt_generate_keys(int bits, char *salt, char *password, char *key_private, char *key_public);
long mincrypt_get_version(void);
int mincrypt_set_simple_mode(int enable);
unsigned char *mincrypt_convert_to_four_system(unsigned char *data, int len);
unsigned char *mincrypt_convert_from_four_system(unsigned char *data, int len);
int mincrypt_set_four_system_quartet(char *quartet);
char *mincrypt_get_four_system_quartet(void);
unsigned char *mincrypt_base64_encode(const char *in, size_t *size);
unsigned char *mincrypt_base64_decode(const char *in, size_t *size);

/* Function prototypes */
uint32_t crc32_block(unsigned char *block, uint32_t length, uint64_t initVal);
unsigned char *base64_encode(const char *in, size_t *size);
unsigned char *base64_decode(const char *in, size_t *size);
char *dec_to_hex(int dec);
uint64_t bits_to_num(char *bits, int num);
char *num_to_bits(uint64_t code, int *out_bits);
int check_is_prime_number_since(uint64_t start, uint64_t number);
int check_is_prime_number(uint64_t number);
tPrimes generate_primes_in_range(uint64_t start, uint64_t end);
tPrimes generate_primes_in_bit_range(int start, int end);
void free_primes(tPrimes p);
char *apply_binary_operation(char *tbits, char *kbits, int operation);
char *align_bits(char *bits, int num);
int get_number_of_bits_set(char *bits, int flags);
uint64_t find_nearest_prime_number(uint64_t number, int flags);
uint64_t pow_and_mod(uint64_t n, uint64_t e, uint64_t mod);
uint64_t get_decryption_value(uint64_t p, uint64_t q, uint64_t e, uint64_t *on);
int get_random_values(uint64_t seed, int size,uint64_t *p, uint64_t *q, uint64_t *oe, uint64_t *od, uint64_t *on, int flags);
unsigned int asymmetric_decrypt(unsigned int c, int d, int n);
unsigned int asymmetric_encrypt(unsigned int c, int e, int n);
uint64_t asymmetric_encrypt_u64(uint64_t c, uint64_t e, uint64_t n);
uint64_t asymmetric_decrypt_u64(uint64_t c, uint64_t d, uint64_t n);
tPrimes get_prime_elements(uint64_t number);
unsigned char *four_numbering_system_encode(unsigned char *data, int len);
unsigned char *four_numbering_system_decode(unsigned char *data, int len);
int four_numbering_system_test(unsigned char *data, int len);
void four_numbering_system_set_quartet(char *quartet);
char *four_numbering_system_get_quartet(void);
char *mincrypt_encrypt_minimal(char *input, unsigned char *key, unsigned char *salt);
char *mincrypt_decrypt_minimal(char *input, unsigned char *key, unsigned char *salt);

/* Diffie-Hellman related stuff */
typedef struct tAKDCommon {
	uint64_t p;
	uint64_t g;
} tAKDCommon;

typedef struct tRndValues {
	int num;
	uint64_t *vals;
} tRndValues;

typedef struct tAKDKeyPair {
	int num;
	uint64_t *vPublic;
	uint64_t *vPrivate;
	tAKDCommon *common;
} tAKDKeyPair;

typedef struct tTokenizerU64 {
	int numVals;
	uint64_t *vals;
} tTokenizerU64;

typedef struct tAKDParams {
	int type;
	int step;
	int count;
	char *filename;
} tAKDParams;

typedef struct tAKDData {
	int step;
	int direction;
	int num;
	uint64_t *vals;

	// Files before operation
	char *bfilename_common;
	unsigned long bfilename_common_size;
	char *bfilename_private;
	unsigned long bfilename_private_size;
	char *bfilename_public;
	unsigned long bfilename_public_size;

	// Files after operation
	char *afilename_common;
	unsigned long afilename_common_size;
	char *afilename_private;
	unsigned long afilename_private_size;
	char *afilename_public;
	unsigned long afilename_public_size;
} tAKDData;

tAKDKeyPair AKD_KEYPAIR_EMPTY;
tAKDParams  AKD_PARAMS_EMPTY;
tAKDData    AKD_DATA_EMPTY;

void mincrypt_init(void);
void akd_mincrypt_init(void);
tAKDParams akd_parse_value(char *val);
tAKDCommon akd_generate_pg_values(int val);
tRndValues generate_random_values(int num, uint64_t max);
tAKDKeyPair akd_generate_keypair(int num, tAKDCommon *common);
int akd_write_shared(int fd, tAKDKeyPair kp, int flags);
int akd_write_file(char *filename, tAKDKeyPair kp, int flags);
int akd_get_number_of_elements(char *filename);
tAKDKeyPair akd_read_file(char *filename, tAKDKeyPair kp);
void akd_keypair_dump(tAKDKeyPair kp);
tTokenizer tokenize_by(char *string, char *by);
int akd_get_number_of_elements(char *filename);
void akd_keypair_free(tAKDKeyPair kp);
void akd_process_data_dump(tAKDData data);
void akd_process_data_free(tAKDData data);
tAKDData akd_process_data(tAKDParams akd_params);
unsigned long get_file_size(char *fn);
unsigned char *uint64_to_binary(uint64_t n, int strip);
unsigned char *uint64_to_bytes(uint64_t n, int len);
void akd_process_data_dump_keys(tAKDData data);
unsigned long long get_microtime(void);

#endif
