#include "common.h"

void do_server_loop(BIO *conn)
{
    int err, nread;
    char buf[80];

    do
    {
        for(nread = 0; nread < sizeof(buf); nread += err)
        {
            err = BIO_read(conn, buf + nread, sizeof(buf) - nread);
            if(err <= 0)
                break;
        }

        fwrite(buf, 1, nread, stdout);
    }
    while (err > 0);
}

void* server_thread(void *arg) {
    BIO *client = (BIO *)arg;
    pthread_detach(pthread_self());

    fprintf(stderr, "Connection opened.\n");
    do_server_loop(client);
    fprintf(stderr, "Connection closed.\n");

    BIO_free(client);
    ERR_remove_thread_state(NULL);
    /*ERR_remove_thread_state() frees the error queue associated with thread tid.
     * if tid == NULL, the current thread will have its error queue removed.
     *
     * Since error queue data structures are allocated automatically for new threads,
     * they must be freed when threads are terminated in order to avoid memory leaks.
     *
     * ERR_remove_state is deprecated and has been replaced by ERR_remove_thread_state. Since
     * threads in OpenSSL are no longer identified by unsinged long values any argument to this function
     * is ignored. Calling ERR_remove_state is equivalent to ERR_remove_thread_state(NULL).
     */
}

int main(int argc, char *argv[])
{
    BIO *acc, *client;
    pthread_t tid;

    init_OpenSSL();

    fprintf(stderr, "\ntry BIO_new_accept - ");
    //BIO_new_accept = combine BIO_new() with BIO_set_accept_port()
    acc = BIO_new_accept(PORT);
    if(!acc)
        int_error("Error creating server socket");
    fprintf(stderr, "OK");

    fprintf(stderr, "\ntry BIO_do_accept - ");
    if(BIO_do_accept(acc) <= 0)
        int_error("Error binding server socket");
    fprintf(stderr, "OK");

    /*
     * BIO_do_accept serves two functions. WHEN it is first called, after the accepted
     * BIO has been setup, it will attempt to create an accept socket and bind an
     * address to it. Second and subsequend calls to BIO_do_accept() will wait in incoming
     * connection, or request a retry in non blocking mode
     */

    for(;;)
    {
        fprintf(stderr, "\nWaiting for new connection");
        if(BIO_do_accept(acc) <= 0)
            int_error("Error accepting connection");

        fprintf(stderr, "\nNew connection arrived");

        fprintf(stderr, "\nTry to get new connection - ");
        client = BIO_pop(acc);
        fprintf(stderr, "I got it");

        pthread_create(&tid, NULL, server_thread, (void *)client);
    }

    BIO_free(acc);
    return 0;
}
