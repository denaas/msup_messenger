#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/ec.h>


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



void Diffie_Hellman(size_t * secret_len){
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
	EVP_PKEY_keygen(kctx, &pkey1);
	EVP_PKEY_keygen(kctx, &pkey2);
	peerkey1 = pkey2;
	peerkey2 = pkey1;
	ctx1 = EVP_PKEY_CTX_new(pkey1, NULL);
	ctx2 = EVP_PKEY_CTX_new(pkey2, NULL);
	EVP_PKEY_derive_init(ctx1);
	EVP_PKEY_derive_init(ctx2);
	EVP_PKEY_derive_set_peer(ctx1, peerkey1);
	EVP_PKEY_derive_set_peer(ctx2, peerkey2);
	EVP_PKEY_derive(ctx1, NULL, secret_len);
	EVP_PKEY_derive(ctx2, NULL, secret_len);
	secret1 = (unsigned char * )OPENSSL_malloc(*secret_len);
	secret2 = (unsigned char * )OPENSSL_malloc(*secret_len);
	EVP_PKEY_derive(ctx1, secret1, secret_len);
	EVP_PKEY_derive(ctx2, secret2, secret_len);
	EVP_PKEY_CTX_free(ctx1);
	EVP_PKEY_free(peerkey1);
	EVP_PKEY_CTX_free(ctx2);
	EVP_PKEY_free(peerkey2);
	//EVP_PKEY_free(pkey);
	EVP_PKEY_CTX_free(kctx);
	EVP_PKEY_free(params);
	EVP_PKEY_CTX_free(pctx);
	for (size_t i = 0;  i < *(secret_len);  i++) {
        printf (" %02x", secret1[i]);
    }
    printf ("\n");
    	for (size_t i = 0;  i < *(secret_len);  i++) {
        printf (" %02x", secret2[i]);
    }
}

/*Функции key_gen и secret_key_gen и в клиенте и в сервере, обе создают свои ключи
Функция Diffie_Hellman2 их запускающая может находится или в клиенте или в сервере*/

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

int main() {
    FILE *in, *out;
    unsigned long key_len = 2048; // длина ключа в битах
    key_generation(key_len);
    size_t secret_len = (size_t) 2048;
    Diffie_Hellman(&secret_len);
    Diffie_Hellman2(&secret_len);
    return 0;
}


