#include <iostream>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <cstdio>
#include <stdio.h>

using namespace std;


void key_generation (unsigned long key_len){
    RSA * key = NULL;
    FILE *priv_key_file = NULL, *pub_key_file = NULL;
    //const EVP_CIPHER *cipher = NULL;        //Надо ли шифровать секретный ключ ?
    priv_key_file = fopen("private_key.txt", "wb");
    pub_key_file = fopen("public_key.txt", "wb");
    key = RSA_generate_key(key_len, RSA_F4, NULL, NULL);
    //OpenSSL_add_all_ciphers();
    //cipher = EVP_get_cipherbyname("bf-ofb");
    //PEM_write_RSAPrivateKey(priv_key_file, rsa, cipher,NULL, 0, NULL, "hello");  //парольная фраза "hello", если будем исползовать шифрование
    PEM_write_RSAPrivateKey(priv_key_file, key, NULL,NULL, 0, NULL, NULL);
    PEM_write_RSAPublicKey(pub_key_file, key);
    RSA_free(key);
    fclose(priv_key_file);
    fclose(pub_key_file);
}

void rsa_encrypt (FILE * in, FILE * out) {
    RSA * pubKey = NULL;
    unsigned char *inbuf, *outbuf;
    unsigned int  inlen;
    unsigned int  outlen;
    FILE * pub_key_file = NULL;
    pub_key_file = fopen("public_key.txt", "rb");                       // Считываем открытый ключ
    pubKey = PEM_read_RSAPublicKey(pub_key_file, NULL, NULL, NULL);
    int key_size = RSA_size(pubKey);            // Определяем длину ключа
    inbuf = (unsigned char *)malloc(key_size);
    outbuf = (unsigned char *)malloc(key_size);
    while(1) {
        inlen = fread(inbuf, 1, key_size - 11, in);
        if(inlen <= 0) break;
        outlen = RSA_public_encrypt(inlen, inbuf, outbuf, pubKey, RSA_PKCS1_PADDING);
        if(outlen != RSA_size(pubKey)) exit(-1);
        fwrite(outbuf, 1, outlen, out);
    }
}

void rsa_decrypt (FILE * in, FILE * out) {
    RSA *privKey = NULL;
    unsigned char *inbuf, *outbuf;
    unsigned int  inlen;
    unsigned int  outlen;
    FILE *priv_key_file = NULL;
    priv_key_file = fopen("private_key.txt", "rb");
    privKey = PEM_read_RSAPrivateKey(priv_key_file, NULL, NULL, NULL);
    int key_size = RSA_size(privKey);
    inbuf = (unsigned char *)malloc(key_size);
    outbuf = (unsigned char *)malloc(key_size);
    while(1) {
        inlen = fread(inbuf, 1, key_size, in);
        if(inlen <= 0) break;
        outlen = RSA_private_decrypt(inlen, inbuf, outbuf, privKey, RSA_PKCS1_PADDING);
        if(outlen < 0) exit(0);
        fwrite(outbuf, 1, outlen, out);
    }
}

void authentication(){
    time_t timer;                   //Клиент
    bool check = true;
    timer = time(NULL);
    char * time_str = ctime (&timer);
    printf("TIME %s",  time_str);

    FILE * in, *out, *in_1,* in_2;       //Клиент
    in = fopen("time.txt", "wb");
    fwrite( time_str, 1, strlen(time_str), in);
    fclose(in);
    in = fopen("time.txt", "rb");
    out = fopen("out.txt", "wb");
    rsa_encrypt(in, out);
    fclose(in);
    fclose(out);

    in = fopen("out.txt", "rb");     // Сервер
    out = fopen("out2.txt", "wb");
    rsa_decrypt(in, out);
    fclose(in);
    fclose(out);

    in_1 = fopen("out2.txt", "rb");     // Клиент
    in_2 = fopen("time.txt", "rb");
    char c1,c2;
    c1 = fgetc(in_1);
    c2 = fgetc(in_2);
    while ( (c1 != EOF)  && (c2 != EOF)   ) {

        c1 = fgetc(in_1);
        c2 = fgetc(in_2);
        if (c1 != c2 )   check = false;
    }
    fclose(in_1);
    fclose(in_2);
}

int main() {
    FILE *in, *out;
    unsigned long key_len = 2048; // длина ключа в битах
    key_generation(key_len);

    in = fopen("text.txt", "rb");
    out = fopen("out.txt", "wb");
    rsa_encrypt(in, out);
    fclose(in);
    fclose(out);

    in = fopen("out.txt", "rb");
    out = fopen("out2.txt", "wb");
    rsa_decrypt(in, out);
    fclose(in);
    fclose(out);

    authentication();

    return 0;
}
