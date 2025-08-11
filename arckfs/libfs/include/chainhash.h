#ifndef SUFS_LIBFS_CHAINHASH_H_
#define SUFS_LIBFS_CHAINHASH_H_

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>

#include "../../include/libfs_config.h"
#include "compiler.h"
#include "types.h"

struct sufs_libfs_ch_item
{
        struct sufs_libfs_ch_item *next;

        char *key;
        unsigned long val;
        unsigned long val2;
};

struct sufs_libfs_ch_bucket
{
#if FIX_DRAM_PM_SYNC
        // Fix: If we use bucket lock for DRAM/PM synchronization, it should be rwlock.
        pthread_rwlock_t lock __mpalign__;
#else
        pthread_spinlock_t lock __mpalign__;
#endif
        bool dead_;


        struct sufs_libfs_ch_item *head;
};

struct sufs_libfs_chainhash
{
        u64 nbuckets_;
        struct sufs_libfs_ch_bucket * buckets_;


        u64 nbuckets_resize_;
        struct sufs_libfs_ch_bucket * buckets_resize_;

        atomic_long size;
        atomic_int  seq_lock;
        bool dead_;
};

// In the original chainhash.c, synchronization was done by directly calling `pthread_spin_lock()`.
// However, depending on the FIX_DRAM_PM_SYNC flag, the lock can be either a spin lock or an rwlock.
// Therefore, all instances of `pthread_spin_lock()` and `pthread_spin_unlock()` in chainhash.c have been
// replaced with the wrapper functions below, which dispatch to the appropriate locking function
// based on the flag. Since these are declared static inline, there is no performance overhead.
static inline void bucket_rdlock(struct sufs_libfs_ch_bucket* b) {
#if FIX_DRAM_PM_SYNC
        pthread_rwlock_rdlock(&b->lock);
#endif
}

static inline void bucket_wrlock(struct sufs_libfs_ch_bucket* b) {
#if FIX_DRAM_PM_SYNC
        pthread_rwlock_wrlock(&b->lock);
#else
        pthread_spin_lock(&b->lock);
#endif
}

static inline void bucket_unlock(struct sufs_libfs_ch_bucket* b) {
#if FIX_DRAM_PM_SYNC
        pthread_rwlock_unlock(&b->lock);
#else
        pthread_spin_unlock(&b->lock);
#endif
}

static inline bool sufs_libfs_chainhash_killed(
        struct sufs_libfs_chainhash *hash)
{
    return hash->dead_;
}

static inline void sufs_libfs_chainhash_fini(struct sufs_libfs_chainhash *hash)
{
    if (hash->buckets_)
        free(hash->buckets_);

    hash->dead_ = true;
    hash->buckets_ = NULL;
}

void sufs_libfs_chainhash_init(struct sufs_libfs_chainhash *hash, int index);

bool sufs_libfs_chainhash_lookup(struct sufs_libfs_chainhash *hash, char *key,
        int max_size, unsigned long *vptr1, unsigned long *vptr2);

bool sufs_libfs_chainhash_insert(struct sufs_libfs_chainhash *hash, char *key,
        int max_size, unsigned long val, unsigned long val2,
        struct sufs_libfs_ch_item ** item);

bool sufs_libfs_chainhash_remove(struct sufs_libfs_chainhash *hash, char *key,
        int max_size, unsigned long *val, unsigned long *val2);

bool sufs_libfs_chainhash_replace_from(
        struct sufs_libfs_chainhash *dst,
        char *kdst,
        unsigned long dst_exist,
        struct sufs_libfs_chainhash *src,
        char *ksrc,
        unsigned long vsrc,
        unsigned long vsrc2,
        int max_size,
        struct sufs_libfs_ch_item ** item);

bool sufs_libfs_chainhash_remove_and_kill(struct sufs_libfs_chainhash *hash);

void sufs_libfs_chainhash_forced_remove_and_kill(
        struct sufs_libfs_chainhash *hash);

bool sufs_libfs_chainhash_enumerate(struct sufs_libfs_chainhash *hash,
        int max_size, char *prev, char *out);

__ssize_t sufs_libfs_chainhash_getdents(struct sufs_libfs_chainhash *hash,
        int max_size, unsigned long * offset_ptr, void * buffer, size_t length);

struct sufs_libfs_ch_bucket *
sufs_libfs_get_buckets(struct sufs_libfs_chainhash * hash, char * key, int max_size);

#if FIX_DRAM_PM_SYNC
struct sufs_libfs_ch_bucket* sufs_libfs_bucket_write_lock(struct sufs_libfs_chainhash *hash,
        char *key, int max_size);

void sufs_libfs_bucket_unlock(struct sufs_libfs_chainhash *hash, struct sufs_libfs_ch_bucket* b, int max_size, bool resize);

bool sufs_libfs_bucket_insert(struct sufs_libfs_chainhash *hash, struct sufs_libfs_ch_bucket* b, char *key,
                int max_size, unsigned long val, unsigned long val2,
                struct sufs_libfs_ch_item ** item);

bool sufs_libfs_bucket_remove(struct sufs_libfs_chainhash *hash, struct sufs_libfs_ch_bucket *b, char *key,
    int max_size, unsigned long *val, unsigned long *val2);
#endif

#endif /* CHAINHASH_H_ */
