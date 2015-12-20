#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstdio>
#include <stdio.h>

#define BUFSIZE (1025*16)

void hash_text(FILE * in, FILE * out, const EVP_MD * md){  //для файла
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


int main(int argc, char** argv)
{
    OpenSSL_add_all_digests();
    FILE *in, *out;
    in = fopen("text.txt", "rb");
    out = fopen("out.txt", "wb");
    const EVP_MD * md;
    md = EVP_ripemd160();
    hash_text(in, out, md);
    fclose(in);
    fclose(out);
    return 0;
}
