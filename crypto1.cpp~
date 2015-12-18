#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstdio>
#include <stdio.h>

#define BUFSIZE 1024

using namespace std;

int crypt_text ( FILE * in, FILE * out, const unsigned char * key) {
    unsigned char iv[8] = {0,0,0,0,0,0,0,0};
    int outlen, inlen;
    unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    const EVP_CIPHER * cipher;
    //cipher = EVP_rc2_cbc();
    cipher = EVP_aes_256_ctr();
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

int decrypt_text ( FILE * in, FILE * out, const unsigned char * key) {
    unsigned char iv[8] = {0,0,0,0,0,0,0,0};
    int outlen, inlen;
    unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    const EVP_CIPHER * cipher;
    //cipher = EVP_rc2_cbc();
    cipher = EVP_aes_256_ctr();
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
    in = fopen("text.txt", "r");
    out = fopen("out.txt", "w");
    unsigned char key[16];
    RAND_bytes(key, sizeof(key));
    crypt_text(in, out, key);

     //ofstream fout("out.txt");
  //  fout << "надо исправить?";
   // fout.close();
  //  system("pause");

    return 0;
}
