#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>


int do_crypt(char *outfile, unsigned char outbuf[])
{
	int outlen, tmplen;
	/* Bogus key and IV: we'd normally set these from
	 * another source.
	 */
	unsigned char key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	unsigned char iv[] = {1,2,3,4,5,6,7,8};
	char intext[] = "Some Crypto Text hahahaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	EVP_CIPHER_CTX *ctx;
	FILE *out;
	
	ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit(ctx, EVP_aes_128_ctr(), key, iv);
	
	if(!EVP_EncryptUpdate(ctx, outbuf, &outlen, intext, strlen(intext)))
	        {
	        /* Error */
	        return 0;
	        }
	printf("first interval: %d\n", outlen);
	/* Buffer passed to EVP_EncryptFinal() must be after data just
	 * encrypted to avoid overwriting it.
	 */
//	if(!EVP_EncryptFinal_ex(ctx, outbuf + outlen, &tmplen))
//	        {
//	        /* Error */
//	        return 0;
//	        }
//	outlen += tmplen;
	EVP_CIPHER_CTX_free(ctx);
	/* Need binary mode for fopen because encrypted data is
	 * binary data. Also cannot use strlen() on it because
	 * it won't be null terminated and may contain embedded
	 * nulls.
	 */
	out = fopen(outfile, "wb");
	fwrite(outbuf, 1, outlen, out);
	fclose(out);
	return 1;
}

int do_decrypt(const char* filename, unsigned char inbuf[]){
	unsigned char key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	unsigned char iv[] = {1,2,3,4,5,6,7,8};
	char outtext[1024];
	char intext[52];
	int outlen;
	EVP_CIPHER_CTX *ctx;
	FILE *in = fopen(filename, "a+");
	fread(intext, 1, 51, in);
	fclose(in);
	ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit(ctx, EVP_aes_128_ctr(), key, iv);

	if(!EVP_EncryptUpdate(ctx, outtext, &outlen, inbuf, strlen(inbuf)))
	{
		return 0;
	}
	outtext[outlen] = '\0';
	printf("decrypted text: %s\nlen: %d\n", outtext, outlen);
	EVP_CIPHER_CTX_free(ctx);
	return 1;
}

int main()
{
	unsigned char outbuf[52];
	do_crypt("./crypt.out", outbuf);
	outbuf[51] = '\0';
	printf("in main: %d\n", strlen(outbuf));
	do_decrypt("./crypt.out", outbuf);
}
