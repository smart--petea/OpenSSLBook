#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/x509v3.h>
#include <pthread.h>

#define PORT "6001"
#define SERVER "splat.zork.org"
#define CLIENT "shell.zork.org"

#define int_error(msg) handle_error(__FILE__, __LINE__, msg)
void handle_error(const char *file, int lineno, const char *msg);

void init_OpenSSL(void);

int THREAD_setup(void);
int THREAD_cleanup(void);

