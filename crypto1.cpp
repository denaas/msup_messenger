#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstdio>
#include <stdio.h>

#define BUFSIZE 1024

using namespace std;

int crypt_text ( FILE * in, FILE * out, const unsigned char * key, const unsigned char * iv) {
    int outlen, inlen;
    unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    const EVP_CIPHER * cipher;
    cipher = EVP_aes_256_cbc();
    EVP_EncryptInit(&ctx, cipher, key, iv);

    for(;;) {
        inlen = fread(inbuf, 1, BUFSIZE, in);
        if(inlen <= 0) break;
        if(!EVP_EncryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen)) return 0;
        fwrite(outbuf, 1, outlen, out);
    }

    if(!EVP_EncryptFinal(&ctx, outbuf, &outlen)) return 0;
    fwrite(outbuf, 1, outlen, out);
    EVP_CIPHER_CTX_cleanup(&ctx);
    return 1;
}

int decrypt_text ( FILE * in, FILE * out, const unsigned char * key, const unsigned char * iv) {
    int outlen, inlen;
    unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    const EVP_CIPHER * cipher;
    cipher = EVP_aes_256_cbc();
    EVP_DecryptInit(&ctx, cipher, key, iv);

    for(;;) {
        inlen = fread(inbuf, 1, BUFSIZE, in);
        if(inlen <= 0) break;
        if(!EVP_DecryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen)) return 0;
        fwrite(outbuf, 1, outlen, out);
    }

    if(!EVP_DecryptFinal(&ctx, outbuf, &outlen)) return 0;
    fwrite(outbuf, 1, outlen, out);
    EVP_CIPHER_CTX_cleanup(&ctx);
    return 1;
}


int main(int argc, char** argv)
{
    OpenSSL_add_all_ciphers();
    FILE *in, *out;
    in = fopen("text.txt", "rb");
    out = fopen("out.txt", "wb");
    unsigned char key[16], iv[8];
    RAND_bytes(key, sizeof(key));
    RAND_bytes(iv, sizeof(iv));
    crypt_text(in, out, key, iv);
    fclose(in);
    fclose(out);
    in = fopen("out.txt", "rb");
    out = fopen("out2.txt", "wb");
    decrypt_text(in, out, key, iv);
    fclose(in);
    fclose(out);
    return 0;
}
