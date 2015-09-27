#include "common.h"

void handle_error(const char *file, int lineno, const char *msg)
{
    fprintf(stderr, "** %s:%i %s\n", file, lineno, msg);
    ERR_print_errors_fp(stderr);
    exit(-1);
}

static pthread_mutex_t* mutex_buf = NULL;

static void locking_function(int mode, int n, const char* file, int line)
{
    if(mode && CRYPTO_LOCK)
        pthread_mutex_lock(&mutex_buf[n]);
    else
        pthread_mutex_unlock(&mutex_buf[n]);
}

static unsigned long id_function(void)
{
    return (unsigned long) pthread_self();
}

int THREAD_setup(void)
{
    int i;

    fprintf(stderr, "\nTry to alloc memory for mutex_buf - ");
    mutex_buf = (pthread_mutex_t *) malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
    if(!mutex_buf) {
        fprintf(stderr, "not setted");
        return 0;
    }
    fprintf(stderr, "setted");

    for(i = 0; i < CRYPTO_num_locks(); i++)
        pthread_mutex_init(&mutex_buf[i], NULL);

    //fprintf(stderr, "\nTry to set id_function - ");
    //CRYPTO_THREADID_set_callback(id_function);
    //fprintf(stderr, "setted");

    //fprintf(stderr, "\nTry to set locking_function - ");
    //CRYPTO_set_locking_callback(locking_function);
    //fprintf(stderr, "setted");

    return 1;
}

int THREAD_cleanup(void)
{
    int i;
    if(!mutex_buf)
        return 0;
    CRYPTO_set_id_callback(NULL);
    CRYPTO_set_locking_callback(NULL);
    for(i = 0; i < CRYPTO_num_locks(); i++)
        pthread_mutex_destroy(&mutex_buf[i]);

    free(mutex_buf);
    mutex_buf = NULL;
    return 1;
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

    if( !THREAD_setup() )
    {
        fprintf(stderr, "\n thread setup failed");
        exit(-1);
    }


    fprintf(stderr, "\ninit_OpenSSL end\n");
}
