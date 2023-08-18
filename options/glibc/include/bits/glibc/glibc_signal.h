#ifndef MLIBC_GLIBC_SIGNAL_H
#define MLIBC_GLIBC_SIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _GNU_SOURCE

#include <abi-bits/signal.h>

typedef __sighandler sighandler_t;

#endif // _GNU_SOURCE

#ifndef __MLIBC_ABI_ONLY

int tgkill(int, int, int);

#endif /* !__MLIBC_ABI_ONLY */

#ifdef __cplusplus
}
#endif

#endif // MLIBC_GLIBC_SIGNAL_H
