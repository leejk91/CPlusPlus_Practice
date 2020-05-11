/*
 *  Public Key layer for parsing key files and structures
 *
 *  Copyright (C) 2006-2014, ARM Limited, All Rights Reserved
 *
 *  This file is part of mbed TLS (https://polarssl.org)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#if !defined(POLARSSL_CONFIG_FILE)
#include "ps_config.h"
#else
#include POLARSSL_CONFIG_FILE
#endif

#if defined(POLARSSL_PK_PARSE_C)

#include "ps_pk.h"
#include "ps_asn1.h"
#include "ps_oid.h"

#if defined(POLARSSL_RSA_C)
#include "ps_rsa.h"
#endif
#if defined(POLARSSL_PEM_PARSE_C)
#include "ps_pem.h"
#endif

#include <stdlib.h>

#if defined(POLARSSL_FS_IO)
/* Implementation that should never be optimized out by the compiler */
static void polarssl_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = v; while( n-- ) *p++ = 0;
}

/*
 * Load all data from a file into a given buffer.
 */
int pk_load_file( const char *path, unsigned char *buf, size_t *n )
{
#ifdef DUKPT_NOT_IO
  *n = strlen(path);
  memcpy(buf, path, *n);
  buf[*n] = '\0';

  return ( 0 );
#else
  FILE *f;
  long size;

  if( ( f = fopen( path, "rb" ) ) == NULL)
    return ( POLARSSL_ERR_PK_FILE_IO_ERROR );

  fseek( f, 0, SEEK_END );
  if ( ( size = ftell( f ) ) == -1 )
  {
    fclose( f );
    return ( POLARSSL_ERR_PK_FILE_IO_ERROR );
  }
  fseek( f, 0, SEEK_SET );

  *n = (size_t) size;

  if ( *n + 1 == 0)
  {
    fclose( f );
    return ( POLARSSL_ERR_PK_MALLOC_FAILED );
  }

  if ( fread( buf, 1, *n, f) != *n)
  {
    fclose( f );
    return ( POLARSSL_ERR_PK_FILE_IO_ERROR );
  }
  fclose( f );
  buf[*n] = '\0';

  return( 0 );
#endif
  /*
    FILE *f;
    long size;

    if( ( f = fopen( path, "rb" ) ) == NULL )
        return( POLARSSL_ERR_PK_FILE_IO_ERROR );

    fseek( f, 0, SEEK_END );
    if( ( size = ftell( f ) ) == -1 )
    {
        fclose( f );
        return( POLARSSL_ERR_PK_FILE_IO_ERROR );
    }
    fseek( f, 0, SEEK_SET );

    *n = (size_t) size;

    if( *n + 1 == 0 ||
        ( *buf = (unsigned char *) polarssl_malloc( *n + 1 ) ) == NULL )
    {
        fclose( f );
        return( POLARSSL_ERR_PK_MALLOC_FAILED );
    }

    if( fread( *buf, 1, *n, f ) != *n )
    {
        fclose( f );
        polarssl_free( *buf );
        return( POLARSSL_ERR_PK_FILE_IO_ERROR );
    }

    fclose( f );

    (*buf)[*n] = '\0';

    return( 0 );
    */
}

/*
 * Load and parse a private key
 */
int pk_parse_keyfile( pk_context *ctx,
                      const char *path, const char *pwd )
{
    int ret;
    size_t n;
    static unsigned char buf[2048];

    if( ( ret = pk_load_file( path, buf, &n ) ) != 0 )
        return( ret );

    if( pwd == NULL )
        ret = pk_parse_key( ctx, buf, n, NULL, 0 );
    else
        ret = pk_parse_key( ctx, buf, n,
                (const unsigned char *) pwd, strlen( pwd ) );

    polarssl_zeroize( buf, n+1 );

    return( ret );
}

/*
 * Load and parse a public key
 */
int pk_parse_public_keyfile( pk_context *ctx, const char *path )
{
    int ret;
    size_t n;
    static unsigned char buf[1024];

    if( ( ret = pk_load_file( path, buf, &n ) ) != 0 )
        return( ret );

    ret = pk_parse_public_key( ctx, buf, n );

    polarssl_zeroize( buf, n+1 );

    return( ret );
}
#endif /* POLARSSL_FS_IO */

#if defined(POLARSSL_RSA_C)
/*
 *  RSAPublicKey ::= SEQUENCE {
 *      modulus           INTEGER,  -- n
 *      publicExponent    INTEGER   -- e
 *  }
 */
static int pk_get_rsapubkey( unsigned char **p,
                             const unsigned char *end,
                             rsa_context *rsa )
{
    int ret;
    size_t len;

    if( ( ret = asn1_get_tag( p, end, &len,
            ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
        return( POLARSSL_ERR_PK_INVALID_PUBKEY + ret );

    if( *p + len != end )
        return( POLARSSL_ERR_PK_INVALID_PUBKEY +
                POLARSSL_ERR_ASN1_LENGTH_MISMATCH );

    if( ( ret = asn1_get_mpi( p, end, &rsa->N ) ) != 0 ||
        ( ret = asn1_get_mpi( p, end, &rsa->E ) ) != 0 )
        return( POLARSSL_ERR_PK_INVALID_PUBKEY + ret );

    if( *p != end )
        return( POLARSSL_ERR_PK_INVALID_PUBKEY +
                POLARSSL_ERR_ASN1_LENGTH_MISMATCH );

    if( ( ret = rsa_check_pubkey( rsa ) ) != 0 )
        return( POLARSSL_ERR_PK_INVALID_PUBKEY );

    rsa->len = mpi_size( &rsa->N );

    return( 0 );
}
#endif /* POLARSSL_RSA_C */

/* Get a PK algorithm identifier
 *
 *  AlgorithmIdentifier  ::=  SEQUENCE  {
 *       algorithm               OBJECT IDENTIFIER,
 *       parameters              ANY DEFINED BY algorithm OPTIONAL  }
 */
static int pk_get_pk_alg( unsigned char **p,
                          const unsigned char *end,
                          pk_type_t *pk_alg, asn1_buf *params )
{
    int ret;
    asn1_buf alg_oid;

    memset( params, 0, sizeof(asn1_buf) );

    if( ( ret = asn1_get_alg( p, end, &alg_oid, params ) ) != 0 )
        return( POLARSSL_ERR_PK_INVALID_ALG + ret );

    if( oid_get_pk_alg( &alg_oid, pk_alg ) != 0 )
        return( POLARSSL_ERR_PK_UNKNOWN_PK_ALG );

    /*
     * No parameters with RSA (only for EC)
     */
    if( *pk_alg == POLARSSL_PK_RSA &&
            ( ( params->tag != ASN1_NULL && params->tag != 0 ) ||
                params->len != 0 ) )
    {
        return( POLARSSL_ERR_PK_INVALID_ALG );
    }

    return( 0 );
}

/*
 *  SubjectPublicKeyInfo  ::=  SEQUENCE  {
 *       algorithm            AlgorithmIdentifier,
 *       subjectPublicKey     BIT STRING }
 */
int pk_parse_subpubkey( unsigned char **p, const unsigned char *end,
                        pk_context *pk )
{
    int ret;
    size_t len;
    asn1_buf alg_params;
    pk_type_t pk_alg = POLARSSL_PK_NONE;
    const pk_info_t *pk_info;

    if( ( ret = asn1_get_tag( p, end, &len,
                    ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
    {
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT);
    }

    end = *p + len;

    if( ( ret = pk_get_pk_alg( p, end, &pk_alg, &alg_params ) ) != 0 )
        return( ret );

    if( ( ret = asn1_get_bitstring_null( p, end, &len ) ) != 0 )
        return( POLARSSL_ERR_PK_INVALID_PUBKEY + ret );

    if( *p + len != end )
        return( POLARSSL_ERR_PK_INVALID_PUBKEY +
                POLARSSL_ERR_ASN1_LENGTH_MISMATCH );

    if( ( pk_info = pk_info_from_type( pk_alg ) ) == NULL )
        return( POLARSSL_ERR_PK_UNKNOWN_PK_ALG );

    if( ( ret = pk_init_ctx( pk, pk_info ) ) != 0 )
        return( ret );

#if defined(POLARSSL_RSA_C)
    if( pk_alg == POLARSSL_PK_RSA )
    {
        ret = pk_get_rsapubkey( p, end, pk_rsa( *pk ) );
    } else
#endif /* POLARSSL_RSA_C */
        ret = POLARSSL_ERR_PK_UNKNOWN_PK_ALG;

    if( ret == 0 && *p != end )
        ret = POLARSSL_ERR_PK_INVALID_PUBKEY
              POLARSSL_ERR_ASN1_LENGTH_MISMATCH;

    if( ret != 0 )
        pk_free( pk );

    return( ret );
}

#if defined(POLARSSL_RSA_C)
/*
 * Parse a PKCS#1 encoded private RSA key
 */
static int pk_parse_key_pkcs1_der( rsa_context *rsa,
                                   const unsigned char *key,
                                   size_t keylen )
{
    int ret;
    size_t len;
    unsigned char *p, *end;

    p = (unsigned char *) key;
    end = p + keylen;

    /*
     * This function parses the RSAPrivateKey (PKCS#1)
     *
     *  RSAPrivateKey ::= SEQUENCE {
     *      version           Version,
     *      modulus           INTEGER,  -- n
     *      publicExponent    INTEGER,  -- e
     *      privateExponent   INTEGER,  -- d
     *      prime1            INTEGER,  -- p
     *      prime2            INTEGER,  -- q
     *      exponent1         INTEGER,  -- d mod (p-1)
     *      exponent2         INTEGER,  -- d mod (q-1)
     *      coefficient       INTEGER,  -- (inverse of q) mod p
     *      otherPrimeInfos   OtherPrimeInfos OPTIONAL
     *  }
     */
    if( ( ret = asn1_get_tag( &p, end, &len,
            ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
    {
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT);
    }

    end = p + len;

    if( ( ret = asn1_get_int( &p, end, &rsa->ver ) ) != 0 )
    {
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT + ret );
    }

    if( rsa->ver != 0 )
    {
        return( POLARSSL_ERR_PK_KEY_INVALID_VERSION );
    }

    if( ( ret = asn1_get_mpi( &p, end, &rsa->N  ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->E  ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->D  ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->P  ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->Q  ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->DP ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->DQ ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->QP ) ) != 0 )
    {
        rsa_free( rsa );
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT + ret );
    }

    rsa->len = mpi_size( &rsa->N );

    if( p != end )
    {
        rsa_free( rsa );
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT +
                POLARSSL_ERR_ASN1_LENGTH_MISMATCH );
    }

    if( ( ret = rsa_check_privkey( rsa ) ) != 0 )
    {
        rsa_free( rsa );
        return( ret );
    }

    return( 0 );
}
#endif /* POLARSSL_RSA_C */

/*
 * Parse an unencrypted PKCS#8 encoded private key
 */
static int pk_parse_key_pkcs8_unencrypted_der(
                                    pk_context *pk,
                                    const unsigned char* key,
                                    size_t keylen )
{
    int ret, version;
    size_t len;
    asn1_buf params;
    unsigned char *p = (unsigned char *) key;
    unsigned char *end = p + keylen;
    pk_type_t pk_alg = POLARSSL_PK_NONE;
    const pk_info_t *pk_info;

    /*
     * This function parses the PrivatKeyInfo object (PKCS#8 v1.2 = RFC 5208)
     *
     *    PrivateKeyInfo ::= SEQUENCE {
     *      version                   Version,
     *      privateKeyAlgorithm       PrivateKeyAlgorithmIdentifier,
     *      privateKey                PrivateKey,
     *      attributes           [0]  IMPLICIT Attributes OPTIONAL }
     *
     *    Version ::= INTEGER
     *    PrivateKeyAlgorithmIdentifier ::= AlgorithmIdentifier
     *    PrivateKey ::= OCTET STRING
     *
     *  The PrivateKey OCTET STRING is a SEC1 ECPrivateKey
     */

    if( ( ret = asn1_get_tag( &p, end, &len,
            ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
    {
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT + ret );
    }

    end = p + len;

    if( ( ret = asn1_get_int( &p, end, &version ) ) != 0 )
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT + ret );

    if( version != 0 )
        return( POLARSSL_ERR_PK_KEY_INVALID_VERSION + ret );

    if( ( ret = pk_get_pk_alg( &p, end, &pk_alg, &params ) ) != 0 )
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT + ret );

    if( ( ret = asn1_get_tag( &p, end, &len, ASN1_OCTET_STRING ) ) != 0 )
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT + ret );

    if( len < 1 )
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT +
                POLARSSL_ERR_ASN1_OUT_OF_DATA );

    if( ( pk_info = pk_info_from_type( pk_alg ) ) == NULL )
        return( POLARSSL_ERR_PK_UNKNOWN_PK_ALG );

    if( ( ret = pk_init_ctx( pk, pk_info ) ) != 0 )
        return( ret );

#if defined(POLARSSL_RSA_C)
    if( pk_alg == POLARSSL_PK_RSA )
    {
        if( ( ret = pk_parse_key_pkcs1_der( pk_rsa( *pk ), p, len ) ) != 0 )
        {
            pk_free( pk );
            return( ret );
        }
    } else
#endif /* POLARSSL_RSA_C */
        return( POLARSSL_ERR_PK_UNKNOWN_PK_ALG );

    return( 0 );
}

/*
 * Parse an encrypted PKCS#8 encoded private key
 */
static int pk_parse_key_pkcs8_encrypted_der(
                                    pk_context *pk,
                                    const unsigned char *key, size_t keylen,
                                    const unsigned char *pwd, size_t pwdlen )
{
    int ret, decrypted = 0;
    size_t len;
    unsigned char buf[2048];
    unsigned char *p, *end;
    asn1_buf pbe_alg_oid, pbe_params;

    memset( buf, 0, sizeof( buf ) );

    p = (unsigned char *) key;
    end = p + keylen;

    if( pwdlen == 0 )
        return( POLARSSL_ERR_PK_PASSWORD_REQUIRED );

    /*
     * This function parses the EncryptedPrivatKeyInfo object (PKCS#8)
     *
     *  EncryptedPrivateKeyInfo ::= SEQUENCE {
     *    encryptionAlgorithm  EncryptionAlgorithmIdentifier,
     *    encryptedData        EncryptedData
     *  }
     *
     *  EncryptionAlgorithmIdentifier ::= AlgorithmIdentifier
     *
     *  EncryptedData ::= OCTET STRING
     *
     *  The EncryptedData OCTET STRING is a PKCS#8 PrivateKeyInfo
     */
    if( ( ret = asn1_get_tag( &p, end, &len,
            ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
    {
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT + ret );
    }

    end = p + len;

    if( ( ret = asn1_get_alg( &p, end, &pbe_alg_oid, &pbe_params ) ) != 0 )
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT + ret );

    if( ( ret = asn1_get_tag( &p, end, &len, ASN1_OCTET_STRING ) ) != 0 )
        return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT + ret );

    if( len > sizeof( buf ) )
        return( POLARSSL_ERR_PK_BAD_INPUT_DATA );

    /*
     * Decrypt EncryptedData with appropriate PDE
     */
    {
        ((void) pwd);
    }

    if( decrypted == 0 )
        return( POLARSSL_ERR_PK_FEATURE_UNAVAILABLE );

    return( pk_parse_key_pkcs8_unencrypted_der( pk, buf, len ) );
}

/*
 * Parse a private key
 */
int pk_parse_key( pk_context *pk,
                  const unsigned char *key, size_t keylen,
                  const unsigned char *pwd, size_t pwdlen )
{
    int ret;
    const pk_info_t *pk_info;

#if defined(POLARSSL_PEM_PARSE_C)
    size_t len;
    pem_context pem;

    pem_init( &pem );

#if defined(POLARSSL_RSA_C)
    ret = pem_read_buffer( &pem,
                           "-----BEGIN RSA PRIVATE KEY-----",
                           "-----END RSA PRIVATE KEY-----",
                           key, pwd, pwdlen, &len );
    if( ret == 0 )
    {
        if( ( pk_info = pk_info_from_type( POLARSSL_PK_RSA ) ) == NULL )
            return( POLARSSL_ERR_PK_UNKNOWN_PK_ALG );

        if( ( ret = pk_init_ctx( pk, pk_info                    ) ) != 0 ||
            ( ret = pk_parse_key_pkcs1_der( pk_rsa( *pk ),
                                            pem.buf, pem.buflen ) ) != 0 )
        {
            pk_free( pk );
        }

        pem_free( &pem );
        return( ret );
    }
    else if( ret == POLARSSL_ERR_PEM_PASSWORD_MISMATCH )
        return( POLARSSL_ERR_PK_PASSWORD_MISMATCH );
    else if( ret == POLARSSL_ERR_PEM_PASSWORD_REQUIRED )
        return( POLARSSL_ERR_PK_PASSWORD_REQUIRED );
    else if( ret != POLARSSL_ERR_PEM_NO_HEADER_FOOTER_PRESENT )
        return( ret );
#endif /* POLARSSL_RSA_C */

    ret = pem_read_buffer( &pem,
                           "-----BEGIN PRIVATE KEY-----",
                           "-----END PRIVATE KEY-----",
                           key, NULL, 0, &len );
    if( ret == 0 )
    {
        if( ( ret = pk_parse_key_pkcs8_unencrypted_der( pk,
                                                pem.buf, pem.buflen ) ) != 0 )
        {
            pk_free( pk );
        }

        pem_free( &pem );
        return( ret );
    }
    else if( ret != POLARSSL_ERR_PEM_NO_HEADER_FOOTER_PRESENT )
        return( ret );

    ret = pem_read_buffer( &pem,
                           "-----BEGIN ENCRYPTED PRIVATE KEY-----",
                           "-----END ENCRYPTED PRIVATE KEY-----",
                           key, NULL, 0, &len );
    if( ret == 0 )
    {
        if( ( ret = pk_parse_key_pkcs8_encrypted_der( pk,
                                                      pem.buf, pem.buflen,
                                                      pwd, pwdlen ) ) != 0 )
        {
            pk_free( pk );
        }

        pem_free( &pem );
        return( ret );
    }
    else if( ret != POLARSSL_ERR_PEM_NO_HEADER_FOOTER_PRESENT )
        return( ret );
#else
    ((void) pwd);
    ((void) pwdlen);
#endif /* POLARSSL_PEM_PARSE_C */

    /*
    * At this point we only know it's not a PEM formatted key. Could be any
    * of the known DER encoded private key formats
    *
    * We try the different DER format parsers to see if one passes without
    * error
    */
    if( ( ret = pk_parse_key_pkcs8_encrypted_der( pk, key, keylen,
                                                  pwd, pwdlen ) ) == 0 )
    {
        return( 0 );
    }

    pk_free( pk );

    if( ret == POLARSSL_ERR_PK_PASSWORD_MISMATCH )
    {
        return( ret );
    }

    if( ( ret = pk_parse_key_pkcs8_unencrypted_der( pk, key, keylen ) ) == 0 )
        return( 0 );

    pk_free( pk );

#if defined(POLARSSL_RSA_C)
    if( ( pk_info = pk_info_from_type( POLARSSL_PK_RSA ) ) == NULL )
        return( POLARSSL_ERR_PK_UNKNOWN_PK_ALG );

    if( ( ret = pk_init_ctx( pk, pk_info                           ) ) != 0 ||
        ( ret = pk_parse_key_pkcs1_der( pk_rsa( *pk ), key, keylen ) ) == 0 )
    {
        return( 0 );
    }

   pk_free( pk );
#endif /* POLARSSL_RSA_C */

    return( POLARSSL_ERR_PK_KEY_INVALID_FORMAT );
}

/*
 * Parse a public key
 */
int pk_parse_public_key( pk_context *ctx,
                         const unsigned char *key, size_t keylen )
{
    int ret;
    unsigned char *p;
#if defined(POLARSSL_PEM_PARSE_C)
    size_t len;
    pem_context pem;

    pem_init( &pem );
    ret = pem_read_buffer( &pem,
            "-----BEGIN PUBLIC KEY-----",
            "-----END PUBLIC KEY-----",
            key, NULL, 0, &len );

    if( ret == 0 )
    {
        /*
         * Was PEM encoded
         */
        key = pem.buf;
        keylen = pem.buflen;
    }
    else if( ret != POLARSSL_ERR_PEM_NO_HEADER_FOOTER_PRESENT )
    {
        pem_free( &pem );
        return( ret );
    }
#endif /* POLARSSL_PEM_PARSE_C */
    p = (unsigned char *) key;

    ret = pk_parse_subpubkey( &p, p + keylen, ctx );

#if defined(POLARSSL_PEM_PARSE_C)
    pem_free( &pem );
#endif

    return( ret );
}

#endif /* POLARSSL_PK_PARSE_C */
