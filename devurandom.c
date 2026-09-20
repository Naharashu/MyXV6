
#include "types.h"
#include "defs.h"
#include "sleeplock.h"
#include "spinlock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "traps.h"
#include "x86.h"

static struct spinlock urnd_lock;

/*

CSPRNG based on http://cr.yp.to/chacha/chacha-20080128.pdf
Uses 16-byte key
Optimized for size

https://gist.githubusercontent.com/Emill/d8e8df7269f75b9485a2/raw/fcb4027ad7e6f79f40c93ed68b197a27730a06f5/ChaCha20-CSPRNG.c
*/

#include "libc/stdint.h"

// This is ASCII of "expand 16-byte k"
static const uint32_t chacha_constants[4] = {0x61707865, 0x3120646e, 0x79622d36, 0x6b206574};

static const uint8_t chacha_order[8][4] = {{0, 4, 8, 12},
                                           {1, 5, 9, 13},
                                           {2, 6, 10, 14},
                                           {3, 7, 11, 15},
                                           {0, 5, 10, 15},
                                           {1, 6, 11, 12},
                                           {2, 7, 8, 13},
                                           {3, 4, 9, 14}};

static uint32_t chacha_key[4];
static uint64_t chacha_counter __attribute__((__zero_init__));
static uint32_t chacha_random_output[16];
static uint8_t chacha_random_output_left __attribute__((__zero_init__));

static void chacha_quarterround(const uint8_t indices[4]) {
    uint32_t *s = chacha_random_output;
    int a = indices[0];
    int b = indices[1];
    int c = indices[2];
    int d = indices[3];
    s[a] += s[b];
    s[d] ^= s[a];
    s[d] = (s[d] << 16) | (s[d] >> 16);
    s[c] += s[d];
    s[b] ^= s[c];
    s[b] = (s[b] << 12) | (s[b] >> 20);
    s[a] += s[b];
    s[d] ^= s[a];
    s[d] = (s[d] << 8) | (s[d] >> 24);
    s[c] += s[d];
    s[b] ^= s[c];
    s[b] = (s[b] << 7) | (s[b] >> 25);
}

static void chacha_run(void) {
    uint32_t state[16];
    __builtin_memcpy(state, chacha_constants, sizeof(chacha_constants));
    __builtin_memcpy(state + 4, chacha_key, sizeof(chacha_key));
    __builtin_memcpy(state + 8, chacha_key, sizeof(chacha_key));
    __builtin_memset(state + 12, 0, 2 * sizeof(uint32_t));
    ++chacha_counter;
    __builtin_memcpy(state + 14, &chacha_counter, sizeof(chacha_counter));

    __builtin_memcpy(chacha_random_output, state, sizeof(state));

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 8; j++) {
            chacha_quarterround(chacha_order[j]);
        }
    }

    for (int i = 0; i < 16; i++) {
        chacha_random_output[i] += state[i];
    }
}

// Public functions

void csprng_seed(const uint8_t key[16]) {
    __builtin_memcpy(chacha_key, key, sizeof(chacha_key));
    chacha_counter = 0;
    chacha_random_output_left = 0;
}

uint32_t csprng_get_next_uint32(void) {
    acquire(&urnd_lock);
    if (chacha_random_output_left == 0) {
        chacha_run();
        chacha_random_output_left = 16;
    }
    uint32_t result = chacha_random_output[--chacha_random_output_left];
    release(&urnd_lock);
    return result;
}


int urndread(struct inode *ip, char *dst, int n) {
    
    int target = n;

    while (n >= 4) {
        if (myproc()->killed) {
            return -1;
        }
        uint r = csprng_get_next_uint32();

        dst[0] = r;
        dst[1] = r >> 8;
        dst[2] = r >> 16;
        dst[3] = r >> 24;

        dst += 4;
        n -= 4;
    }

    if (n > 0) {
        uint r = csprng_get_next_uint32();

        while (n > 0) {
            if (myproc()->killed) {
                return -1;
            }
            *dst++ = r;
            r >>= 8;
            n--;
        }
    }



    return target;
}

int urndwrite(struct inode *ip, char *buf, int n) {
    iunlock(ip);
    ilock(ip);
    return -1;
}

void urndominit(void) {
    initlock(&urnd_lock, "urandom");
    uint lo, hi;

    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    uint seed = hi ^ lo;
    unsigned char s1[4];
    __builtin_memcpy(s1, &seed, sizeof(seed));
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    seed -= hi ^ lo;
    unsigned char s2[4];
    __builtin_memcpy(s2, &seed, sizeof(seed));
    outb(0x43, 0x00);  // latch channel 0
    uint8_t pit_lo = inb(0x40);
    uint8_t pit_hi = inb(0x40);
    seed = (pit_hi << 8) | pit_lo;
    unsigned char s3[4];
    __builtin_memcpy(s3, &seed, sizeof(seed));
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    seed += hi ^ lo;
    unsigned char s4[4];
    __builtin_memcpy(s4, &seed, sizeof(seed));

    unsigned char sfinal[16];
    __builtin_memcpy(sfinal, s1, 4);
    __builtin_memcpy(sfinal + 4, s2, 4);
    __builtin_memcpy(sfinal + 8, s3, 4);
    __builtin_memcpy(sfinal + 12, s4, 4);

    csprng_seed(sfinal);
    devsw[URNDOM].write = urndwrite;
    devsw[URNDOM].read = urndread;
}
