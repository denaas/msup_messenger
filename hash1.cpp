#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

unsigned int BUFSIZE = 1024;
using namespace std;

void hash_text(FILE * in, FILE * out, const EVP_MD * md){  // хэш от файла
    EVP_MD_CTX mdctx;
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;
    unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
    EVP_DigestInit(&mdctx, md);
    for(;;) {
        int i = fread(inbuf, 1, BUFSIZE, in);
        if(i <= 0) break;
        EVP_DigestUpdate(&mdctx, outbuf, (unsigned long)i);
        EVP_DigestFinal(&mdctx, md_value, &md_len);
        fwrite(md_value, 1, md_len, out);
    }
    EVP_MD_CTX_cleanup(&mdctx);
}

void hmac_text(FILE * in, FILE * out, const EVP_MD * md, const unsigned char * key_2){  // hmac от файла
    HMAC_CTX ctx;
    unsigned char result[20]; //тк 160 бит
    unsigned int  len;
    unsigned char inbuf[BUFSIZE];
    char * key = (char *)key_2;
    HMAC_CTX_init(&ctx);
    HMAC_Init(&ctx, key, strlen(key), md);
    fread(inbuf, 1, BUFSIZE, in);
    HMAC_Update(&ctx, inbuf, BUFSIZE);
    HMAC_Final(&ctx, result, &len);
    fwrite(result, 1, len, out);
    HMAC_CTX_cleanup(&ctx);
}

void connect(FILE * in_1, FILE * in_2, FILE * out){   //чтобы удобнее было отлаживать создан отдельны файл. Вообще hmac должен был бы записываться в конец изначальных данных.
    unsigned char result[160];
    unsigned char inbuf[BUFSIZE];
    fread(inbuf, 1, BUFSIZE, in_1);
    fwrite(inbuf, 1, BUFSIZE, out);
    fread(result, 1, 20, in_2);
    fwrite(result, 1, 20 , out);
}

bool check_hmac(FILE * in, const EVP_MD * md, const unsigned char * key_2){  //  проверка hmac от файла, возвращает 1 если хэш совпал, 0 иначе
    HMAC_CTX ctx;
    bool check = true;
    unsigned char result[20], hmac1[20];
    unsigned int  len;
    unsigned char inbuf[BUFSIZE];
    char * key = (char *)key_2;
    HMAC_CTX_init(&ctx);
    HMAC_Init(&ctx, key, strlen(key), md);
    fread(inbuf, 1, BUFSIZE, in);
    fread(hmac1, 1, 20, in);
    HMAC_Update(&ctx, inbuf, BUFSIZE);
    HMAC_Final(&ctx, result, &len);
    for (int i = 0;  i < len;  i++) {
        if (result [i] != hmac1 [i]) check = false;
    }
    HMAC_CTX_cleanup(&ctx);
    return check;
}

int main(int argc, char** argv)
{
    OpenSSL_add_all_digests();
    FILE *in, *out;
    FILE *in_1, *in_2;
    struct stat fi;
    bool Correct = false;
    const EVP_MD * md;
    unsigned char key[16];

    stat("text.txt", &fi);
    BUFSIZE = fi.st_size;
    RAND_bytes(key, sizeof(key));

    in = fopen("text.txt", "rb");
    out = fopen("out.txt", "wb");
    md = EVP_ripemd160();
    hash_text(in, out, md);
    fclose(in);
    fclose(out);

    in = fopen("text.txt", "rb");
    out = fopen("out2.txt", "wb");
    hmac_text(in, out, md, key);
    fclose(in);
    fclose(out);

    in_1 = fopen("text.txt", "rb");
    in_2 = fopen("out2.txt", "rb");
    out = fopen("out3.txt", "wb");
    connect(in_1, in_2, out);
    fclose(in_1);
    fclose(in_2);
    fclose(out);

    in = fopen("out3.txt", "rb");
    Correct = check_hmac(in, md, key);
    fclose(in);

    return 0;
}
