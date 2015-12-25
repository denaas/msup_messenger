#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstdio>
#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>


#define BUFSIZE 1024

using namespace std;


void key_generation (unsigned long key_len){
    RSA * key = NULL;
    FILE *priv_key_file = NULL, *pub_key_file = NULL;
    priv_key_file = fopen("private_key.txt", "wb");
    pub_key_file = fopen("public_key.txt", "wb");
    key = RSA_generate_key(key_len, RSA_F4, NULL, NULL);
    PEM_write_RSAPrivateKey(priv_key_file, key, NULL,NULL, 0, NULL, NULL);
    PEM_write_RSAPublicKey(pub_key_file, key);
    RSA_free(key);
    fclose(priv_key_file);
    fclose(pub_key_file);
}

RSA * get_RSA_public_key (){
    RSA * pubKey = NULL;
    unsigned char *outbuf;
    FILE * pub_key_file = NULL;
    pub_key_file = fopen("public_key.txt", "rb");
    pubKey = PEM_read_RSAPublicKey(pub_key_file, NULL, NULL, NULL);

}

RSA * get_RSA_private_key (){
    RSA *privKey = NULL;
    unsigned char *outbuf;
    unsigned int  inlen;
    unsigned int  outlen;
    FILE *priv_key_file = NULL;
    priv_key_file = fopen("private_key.txt", "rb");
    privKey = PEM_read_RSAPrivateKey(priv_key_file, NULL, NULL, NULL);

}

unsigned char * random_key_gener (int len_key) {
    unsigned char * key;
    key = (unsigned char *)malloc(len_key* sizeof(char));
    RAND_bytes(key, len_key);
    return key;
}

