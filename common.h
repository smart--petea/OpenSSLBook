#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl/x509v3.h>
#include <pthread.h>

#define PORT "6001"
#define SERVER "127.0.0.1"
#define CLIENT "127.0.0.1"
#define CERTS_DIR "certs"

#define int_error(msg) handle_error(__FILE__, __LINE__, msg)
void handle_error(const char *file, int lineno, const char *msg);

void init_OpenSSL(void);

void THREAD_setup(void);
void THREAD_cleanup(void);

int seed_prng(int);
