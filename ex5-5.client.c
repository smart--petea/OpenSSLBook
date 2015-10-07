#include "common.h"
#include <string.h>

#define CERTFILE ( CERTS_DIR "/client.pem" )

SSL_CTX *setup_client_ctx(void)
{
    SSL_CTX *ctx;

    ctx = SSL_CTX_new(SSLv23_method());
    if(SSL_CTX_use_certificate_chain_file(ctx, CERTFILE) != 1)
        int_error("Error loading certificate from file");

    if(SSL_CTX_use_PrivateKey_file(ctx, CERTFILE, SSL_FILETYPE_PEM) != 1)
        int_error("Error loading private key from file");

    return ctx;
}

int do_client_loop(SSL *ssl)
{
    int err, nwritten;
    char buf[80];

    for(;;)
    {
        if(!fgets(buf, sizeof(buf), stdin))
            break;

        for(nwritten = 0; nwritten < sizeof(buf); nwritten += err)
        {
            fprintf(stderr, "\nwriting %d", strlen(buf) - nwritten);
            err = SSL_write(ssl, buf + nwritten, strlen(buf) - nwritten);

            if(err <= 0)
                return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[])
{
    BIO *conn;
    SSL *ssl;
    SSL_CTX *ctx;

    init_OpenSSL();

    fprintf(stderr, "\nex5-5: \n");

    seed_prng(10);
    ctx = setup_client_ctx();

    conn = BIO_new_connect(SERVER ":" PORT); //create BIO and set it connect dates
    if(!conn)
        int_error("Error creating connection BIO");

    //1. create BIO
    //2. set BIO params
    //3. try to connect BIO
    //4 create sssl
    //5 move ssl on top of BIO
    if(BIO_do_connect(conn) <= 0)
        int_error("Error connection to remote machine");

    if(!(ssl = SSL_new(ctx)))
        int_error("Error creating an SSL context");
    SSL_set_bio(ssl, conn, conn);
    if(SSL_connect(ssl) <= 0)
        int_error("Error connecting SSL object");

    fprintf(stderr, "SSL Connection opened\n");
    if(do_client_loop(ssl))
        SSL_shutdown(ssl);
    else
        SSL_clear(ssl);
    fprintf(stderr, "SSL Connection closed\n");

    SSL_free(ssl);
    SSL_CTX_free(ctx);
    return 0;
}
