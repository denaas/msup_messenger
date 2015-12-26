#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

using namespace std;

char * stringcopy(char *s1, const char *s2){
	char *s = s1;
	while ((*s++ = *s2++) != 0);
	return (s1);
}

char * rewrite(int i, char *stream){
	char * buf;
	buf = (char *)malloc(BUFSIZE);
	for (int j = 0; stream[j] != '\0'; j++){
		if (((j + 1) % BUFSIZE) == 0) {
			char * resbuf = (char *) malloc((j + 1) * 2);
			buf = stringcopy(resbuf, buf);
        }
		buf[j] = stream[i + j];
	}
}

int sread(char *buf, int scount, char *stream){
	int i = 0;
	for (i = 0; i < scount; i++){
		buf[i] = stream[i];
		if (stream[i] == '\0')
			break;
	}
	if (i == scount)
		stream = rewrite(i, stream);
	return i;
}

int swrite(const char *buf, int scount, char *stream){
	int i = 0;
	for (i = 0; i < scount; i++){
		stream[i] = buf[i];
		if (buf[i] == '\0')
			break;
	}
	if (i == scount)
		stream = rewrite(i, stream);
	return i;
}

int crypt_text(char * in, char * out, const unsigned char * key, const unsigned char * iv) {
	int outlen, inlen;
	unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	const EVP_CIPHER * cipher;
	cipher = EVP_aes_256_cbc();
	EVP_EncryptInit(&ctx, cipher, key, iv);

	for (;;) {
		inlen = sread((char *)inbuf, BUFSIZE, in);
		if (inlen <= 0) break;
		if (!EVP_EncryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen)) return 0;
		swrite((char *)outbuf, outlen, out);
	}

	if (!EVP_EncryptFinal(&ctx, outbuf, &outlen)) return 0;
	swrite((char *)outbuf, outlen, out);
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 1;
}

int decrypt_text ( char * in, char * out, const unsigned char * key, const unsigned char * iv) {
    int outlen, inlen;
    unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    const EVP_CIPHER * cipher;
    cipher = EVP_aes_256_cbc();
    EVP_DecryptInit(&ctx, cipher, key, iv);

    for(;;) {
        inlen = sread((char *)inbuf, BUFSIZE, in);
        if(inlen <= 0) break;
        if(!EVP_DecryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen)) return 0;
        swrite((char *)outbuf, outlen, out);
    }

    if(!EVP_DecryptFinal(&ctx, outbuf, &outlen)) return 0;
    swrite((char *)outbuf, outlen, out);
    EVP_CIPHER_CTX_cleanup(&ctx);
    return 1;
}


