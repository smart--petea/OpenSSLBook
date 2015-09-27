#include "common.h"

void handle_error(const char *file, int lineno, const char *msg)
{
    fprintf(stderr, "** %s:%i %s\n", file, lineno, msg);
    ERR_print_errors_fp(stderr);
    exit(-1);
}

static pthread_mutex_t *lock_cs;
static long *lock_count;

void pthreads_thread_id(CRYPTO_THREADID *tid)
{
        CRYPTO_THREADID_set_numeric(tid, (unsigned long)pthread_self());
}

void pthreads_locking_callback(int mode, int type, const char *file, int line)
{
    if (mode & CRYPTO_LOCK)
    {
        pthread_mutex_lock(&(lock_cs[type]));
        lock_count[type]++;
    } else {
        pthread_mutex_unlock(&(lock_cs[type]));
    }
}

void THREAD_setup(void)
{
    int i;

    lock_cs = OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
    lock_count = OPENSSL_malloc(CRYPTO_num_locks() * sizeof(long));
    for (i = 0; i < CRYPTO_num_locks(); i++)
    {
        lock_count[i] = 0;
        pthread_mutex_init(&(lock_cs[i]), NULL);
    }

    CRYPTO_THREADID_set_callback(pthreads_thread_id);
    CRYPTO_set_locking_callback(pthreads_locking_callback);
}

void THREAD_cleanup(void)
{
    int i;

    CRYPTO_set_locking_callback(NULL);
    fprintf(stderr, "\ncleanup");
    for (i = 0; i < CRYPTO_num_locks(); i++)
    {
        pthread_mutex_destroy(&(lock_cs[i]));
        fprintf(stderr, "\n%8ld:%s", lock_count[i], CRYPTO_get_lock_name(i));
    }

    OPENSSL_free(lock_cs);
    OPENSSL_free(lock_count);

    fprintf(stderr, "\ndone cleanup");
}

static unsigned long id_function(void)
{
    return (unsigned long) pthread_self();
}

void init_OpenSSL(void)
{

    fprintf(stderr, "\ntry to init ssl library - ");
    if(!SSL_library_init())
    {
        fprintf(stderr, "failed");
        exit(-1);
    }
    fprintf(stderr, "inited");

    SSL_load_error_strings();

    fprintf(stderr, "\ntry to start THREAD_setup - ");
    THREAD_setup();
    fprintf(stderr, "started");
}
