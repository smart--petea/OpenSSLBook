#include "common.h"
#define CERTFILE ( CERTS_DIR "/server.pem" )

SSL_CTX *setup_server_ctx(void)
{
    SSL_CTX *ctx;

    ctx = SSL_CTX_new(SSLv23_method());
    if(SSL_CTX_use_certificate_chain_file(ctx, CERTFILE) != 1)
        int_error("Error loadin certificate from file");
    if(SSL_CTX_use_PrivateKey_file(ctx, CERTFILE, SSL_FILETYPE_PEM) != 1)
        int_error("Error loading private key from file");

    return ctx;
}

int do_server_loop(SSL *ssl)
{
    int err, nread;
    char buf[80];

    do
    {
        for(nread = 0; nread < sizeof(buf); nread += err)
        {
            err = SSL_read(ssl, buf + nread, sizeof(buf) - nread);
            if(err <= 0)
                break;
        }
        fwrite(buf, 1, nread, stdout);
    }
    while (err > 0);
    return (SSL_get_shutdown(ssl) & SSL_RECEIVED_SHUTDOWN) ? 1 : 0;
}

void* server_thread(void *arg)
{
    SSL *ssl = (SSL *)arg;
    pthread_detach(pthread_self());

    if(SSL_accept(ssl) <= 0)
        int_error("Error accepting SSL connection");

    fprintf(stderr, "SSL Connection opened\n");
    if(do_server_loop(ssl))
        SSL_shutdown(ssl);
    else
        SSL_clear(ssl);
    fprintf(stderr, "SSL Connection closed\n");
    SSL_free(ssl);

    return NULL;
}

int main(int argc, char *argv[])
{
    BIO *acc, *client;
    SSL *ssl;
    SSL_CTX *ctx;
    pthread_t tid;

    init_OpenSSL();
    fprintf(stderr, "\nex5-6 server:\n");

    seed_prng(10);
    ctx = setup_server_ctx();

    acc = BIO_new_accept(PORT);
    if(!acc)
        int_error("Error creating server socket");
    else
        fprintf(stderr, "\nBio created on port %s", PORT);

    if(BIO_do_accept(acc) <= 0)
        int_error("Error binding server socket");
    else
        fprintf(stderr, "\nBio listening on port %s", PORT);

    for(;;)
    {
        if(BIO_do_accept(acc) <= 0 )
            int_error("Error accepting connection");
        else
            fprintf(stderr, "\nBio connected");

        client = BIO_pop(acc);
        if(!(ssl = SSL_new(ctx)))
            int_error("Error creating SSL context");
        else
            fprintf(stderr, "\nSSL context created");

        SSL_set_bio(ssl, client, client);
        pthread_create(&tid, NULL, server_thread, ssl);
    }

    SSL_CTX_free(ctx);
    BIO_free(acc);

    return 0;
}
