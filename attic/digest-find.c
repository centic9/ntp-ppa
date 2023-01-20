/*
 * digest.c - Hack to test various digest types.
 *
 * Fedora mentions blake2b and friends.  They are in the man page
 * and header files, but not available via OBJ_sn2n so I assume they
 * are not interesting and dropped them to reduce clutter.
 *
 * If the type column is blank, the OpenSSL package doesn't know
 * about that digest, maybe because it isn't spelled correctly.
 *
 * If the type column is non-blank but the length column is empty,
 * the library recognizes the type but doesn't support it.
 *
 * If the length column is filled in, that's the length of the digest.
 *
 */

#include <stdio.h>

#include <openssl/objects.h>
#if OPENSSL_VERSION_NUMBER > 0x20000000L
#include <openssl/ssl.h>
#endif
#include <openssl/evp.h>

#define UNUSED_ARG(arg)         ((void)(arg))


const char* digests[] = {
    "MD2", "MD4", "MD5",
    "SHA", "SHA1",
    "RMD160", "RIPEMD160",
    "SHA224", "SHA256", "SHA384", "SHA512",
    "MDC2", "GOST", "DSS1",
    "ChaCha20", "Poly1305",
    NULL };

unsigned char packet[100];
int
main (
	int argc,
	char *argv[]
	)
{
    UNUSED_ARG(argc);
    UNUSED_ARG(argv);

#if OPENSSL_VERSION_NUMBER > 0x20000000L
    SSL_CTX *ssl = SSL_CTX_new(TLS_client_method());
#endif

    unsigned int versionNumber = OPENSSL_VERSION_NUMBER;
    const char *versionText = OPENSSL_VERSION_TEXT;
    printf("OpenSSL xVersion is %x, %s\n", versionNumber, versionText);

    /* needed if OPENSSL_VERSION_NUMBER < 0x10100000L */
    OpenSSL_add_all_digests();


    printf("      name type length\n");
    for (int i = 0; NULL != digests[i]; i++) {
	unsigned char digest[EVP_MAX_MD_SIZE];
	unsigned int length = 0;
	EVP_MD_CTX *ctx;
	int keytype;
	const EVP_MD *md;
	keytype = OBJ_sn2nid(digests[i]);
	if (NID_undef == keytype) {
	    printf("%10s (no keytype)\n", digests[i]);
	    continue;
	}
	md = EVP_get_digestbynid(keytype);
	if (NULL == md) {
	    printf("%10s %4d (no digest)\n", digests[i], keytype);
	    continue;
	}
#if OPENSSL_VERSION_NUMBER > 0x20000000L
	if (1 && 0 == SSL_CTX_set_cipher_list(ssl, digests[i])) {
	    printf("%10s (no cipher_list)\n", digests[i]);
	    continue;
	}
#endif
	/* libntp/macencrypt.c has an ifdef for this */
	ctx = EVP_MD_CTX_create();
	EVP_MD_CTX_set_flags(ctx, EVP_MD_CTX_FLAG_NON_FIPS_ALLOW);
	EVP_DigestInit_ex(ctx, md, NULL);
	EVP_DigestUpdate(ctx, packet, sizeof(packet));
	EVP_DigestFinal_ex(ctx, digest, &length);
	EVP_MD_CTX_destroy(ctx);
	printf("%10s %4d %6u\n", digests[i], keytype, length);
    }

    return 0;
}
