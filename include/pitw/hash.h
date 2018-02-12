#ifndef PITW_HASH_H
#define PITW_HASH_H

#include <pitw/config.h>
#include <stddef.h>

/**
 * @file
 * A hash table with string keys.
 * Collisions are handled with linear probing. Poor distribution of hash codes
 * will cause the hash table to degenerate into a linear scan of an array.
 */

/**
 * A hash table.
 */
typedef struct pitw_hash pitw_hash;

/**
 * A hash function.
 * This function must not modify the key.
 * @param key The key to hash.
 * @return The hash code for the key.
 */
typedef size_t (*pitw_hash_fn)(char *key);

/**
 * Creates a new hash table with default parameters.
 * @return An empty hash table.
 */
PITW_API pitw_hash *pitw_hash_create(void);

/**
 * Creates a new hash table.
 * @param size The starting size of the hash table.
 * @param hash_fn The hash function.
 * @return An empty hash table.
 */
PITW_API pitw_hash *pitw_hash_create_custom(size_t size, pitw_hash_fn hash_fn);

/**
 * Frees a hash table.
 * @param hash The hash table to free.
 */
PITW_API void pitw_hash_free(pitw_hash *hash);

/**
 * Returns the value associated with the key in the hash table.
 * If a key is associated with NULL, then #pitw_hash_contains should be used.
 * @param hash The hash table.
 * @param key The key.
 * @return The associated value, or NULL if the key is not present.
 */
PITW_API void *pitw_hash_get(pitw_hash *hash, char *key);

/**
 * Associates a key with a value in a hash table.
 * Any existing value will be overwritten.
 * Inserting a new key to the hash table may cause resizing.
 * @param hash The hash table.
 * @param key The key.
 * @param val The value.
 */
PITW_API void pitw_hash_set(pitw_hash *hash, char *key, void *val);

/**
 * Removes a key from a hash table.
 * If the key is not present, nothing happens.
 * @param hash The hash table.
 * @param key The key.
 */
PITW_API void pitw_hash_remove(pitw_hash *hash, char *key);

/**
 * Checks if the key is in the hash table.
 * This function allows clients to detect keys associated with NULL.
 * @param hash The hash table.
 * @param key The key.
 * @return 1 if the key is present, else 0.
 */
PITW_API int pitw_hash_contains(pitw_hash *hash, char *key);

#endif /* PITW_HASH_H */
