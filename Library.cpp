#include <iostream>
#include <openssl/rand.h>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/ec.h>


#define BUFSIZE 1024

using namespace std;

struct file_names {
    char * publ_name;
    char * priv_name;
};

struct file_names key_generation (unsigned long key_len,char * password){
    RSA * key = NULL;
    struct file_names Name;
    FILE *priv_key_file = NULL, *pub_key_file = NULL;
    const EVP_CIPHER *cipher = NULL;
    priv_key_file = fopen("private_key.txt", "wb");
    pub_key_file = fopen("public_key.txt", "wb");
    key = RSA_generate_key(key_len, RSA_F4, NULL, NULL);
    OpenSSL_add_all_ciphers();
    cipher = EVP_get_cipherbyname("bf-ofb");
    PEM_write_RSAPrivateKey(priv_key_file, key, cipher,NULL, 0, NULL, password);
    PEM_write_RSAPrivateKey(priv_key_file, key, NULL,NULL, 0, NULL, NULL);
    PEM_write_RSAPublicKey(pub_key_file, key);
    RSA_free(key);
    fclose(priv_key_file);
    fclose(pub_key_file);
    Name.publ_name = (char *)malloc(strlen("public_key.txt")* sizeof(char));
    Name.priv_name = (char *)malloc(strlen("private_key.txt")* sizeof(char));
    strcpy (Name.priv_name,"private_key.txt");
    strcpy (Name.publ_name,"public_key.txt");
    return Name;

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

EVP_PKEY * key_gen( EVP_PKEY_CTX * kctx){
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_keygen_init(kctx);
    EVP_PKEY_keygen(kctx, &pkey);
    return pkey;
}

unsigned char * secret_key_gen(EVP_PKEY *pkey, EVP_PKEY * peerkey, size_t * secret_len){
    EVP_PKEY_CTX *ctx;
    unsigned char *secret;
    ctx = EVP_PKEY_CTX_new(pkey, NULL);
    EVP_PKEY_derive_init(ctx);
    EVP_PKEY_derive_set_peer(ctx, peerkey);
    EVP_PKEY_derive(ctx, NULL, secret_len);
    secret = (unsigned char * ) OPENSSL_malloc(*secret_len);
    EVP_PKEY_derive(ctx, secret, secret_len);
    EVP_PKEY_CTX_free(ctx);
    return secret;
}

void server (unsigned char * secret, unsigned char * iv) {
    unsigned char * sec_key = random_key_gener(1024);
     FILE *in, *out;
    out = fopen("out.txt", "wb");
    fwrite(sec_key, 1, 1024, out);
    fclose(out);
    in = fopen("out.txt", "rb");
    out = fopen("out2.txt", "wb");
    crypt_text(in, out, secret, iv);
    fclose(in);
    fclose(out);
}

void client (unsigned char * secret, unsigned char * iv) {
     FILE *in, *out;
    in = fopen("out2.txt", "rb");
    out = fopen("out3.txt", "wb");
    decrypt_text(in, out, secret, iv);
    fclose(in);
    fclose(out);
}

void Diffie_Hellman2(size_t * secret_len){
    EVP_PKEY_CTX *pctx, *kctx;
	EVP_PKEY_CTX *ctx1, *ctx2;
	unsigned char *secret1, *secret2;
	EVP_PKEY *pkey1 = NULL, *peerkey1,*pkey2 = NULL, *peerkey2, *params = NULL;
	pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
	EVP_PKEY_paramgen_init(pctx);
	EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, NID_X9_62_prime256v1);
	EVP_PKEY_paramgen(pctx, &params);
	kctx = EVP_PKEY_CTX_new(params, NULL);
	EVP_PKEY_keygen_init(kctx);
    pkey1 = key_gen(kctx);
    pkey2 = key_gen(kctx);
    peerkey1 = pkey2;
	peerkey2 = pkey1;
    secret1 = (unsigned char * )OPENSSL_malloc(*secret_len);
    secret2 = (unsigned char * )OPENSSL_malloc(*secret_len);
    secret1 = secret_key_gen(pkey1, peerkey1, secret_len);
    secret2 = secret_key_gen(pkey2, peerkey2, secret_len);
    unsigned char iv[8];
    RAND_bytes(iv, sizeof(iv));
    server (secret1, iv);
    client (secret1, iv);
	EVP_PKEY_free(peerkey1);
	EVP_PKEY_free(peerkey2);
    EVP_PKEY_CTX_free(kctx);
	EVP_PKEY_free(params);
	EVP_PKEY_CTX_free(pctx);
	printf ("\n SECOND \n");
		for (size_t i = 0;  i < *(secret_len);  i++) {
        printf (" %02x", secret1[i]);
    }
    printf ("\n");
    	for (size_t i = 0;  i < *(secret_len);  i++) {
        printf (" %02x", secret2[i]);
    }
}

/*
Алгоритм Диффи-Хэлмана, порядок немного другой но суть та же
Сначала вычисляется общий секретный клч, далее сервер генерирует какой то секрет ключ и зашифровывает общим секретным ключом
Далее клиент зная общий секретный лючи может получить из файла сгенерированный сервером ключ.
*/
