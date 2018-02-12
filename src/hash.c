#include <pitw/hash.h>
#include <pitw/mem.h>
#include <string.h>

#define DEFAULT_SIZE 16

struct cell {
    char *key;
    void *val;
};

static int cell_empty(struct cell *cell) {
    return !cell->key;
}

static int cell_has_key(struct cell *cell, char *key) {
    return !strcmp(cell->key, key);
}

struct pitw_hash {
    pitw_hash_fn hash_fn;
    struct cell *cells;
    size_t size;
    size_t used;
};

static int get_load(pitw_hash *hash) {
    return 100 * hash->used / hash->size;
}

/* the cell the key would use if no collisions */
static struct cell *get_ideal_cell(pitw_hash *hash, char *key) {
    return &hash->cells[hash->hash_fn(key) % hash->size];
}

/* advance pointer, wrapping if necessary */
static struct cell *next_cell(pitw_hash *hash, struct cell *cell) {
    return ++cell == hash->cells + hash->size ? hash->cells : cell;
}

/* the cell the key is actually in */
static struct cell *get_real_cell(pitw_hash *hash, char *key) {
    struct cell *cell = get_ideal_cell(hash, key);
    for (size_t i = 0; i < hash->size; ++i) {
        if (cell_empty(cell))
            return NULL;
        if (cell_has_key(cell, key))
            return cell;
        cell = next_cell(hash, cell);
    }
    return NULL;
}

/* the cell the key can be written to */
static struct cell *get_write_cell(pitw_hash *hash, char *key) {
    struct cell *cell = get_ideal_cell(hash, key);
    for (size_t i = 0; i < hash->size; ++i) {
        if (cell_empty(cell))
            return cell;
        if (cell_has_key(cell, key))
            return cell;
        cell = next_cell(hash, cell);
    }
    return NULL;
}

pitw_hash *pitw_hash_create(void) {
    return pitw_hash_create_custom(0, NULL);
}

static size_t default_hash_fn(char *key) {
    /* djb2 */
    size_t hash = 5381;
    while (*key)
        hash = hash * 33 + *key++;
    return hash;
}

pitw_hash *pitw_hash_create_custom(size_t size, pitw_hash_fn hash_fn) {
    pitw_hash *hash = pitw_mem_alloc(sizeof(*hash));
    hash->hash_fn = hash_fn ? hash_fn : default_hash_fn;
    hash->size = size > 0 ? size : DEFAULT_SIZE;
    hash->cells = pitw_mem_alloc_zero(hash->size * sizeof(hash->cells[0]));
    hash->used = 0;
    return hash;
}

void pitw_hash_free(pitw_hash *hash) {
    pitw_mem_free(hash->cells);
    pitw_mem_free(hash);
}

void *pitw_hash_get(pitw_hash *hash, char *key) {
    struct cell *cell = get_real_cell(hash, key);
    return cell ? cell->val : NULL;
}

int pitw_hash_contains(pitw_hash *hash, char *key) {
    struct cell *cell = get_real_cell(hash, key);
    return cell ? 1 : 0;
}

static void resize(pitw_hash *hash, size_t size) {
    struct cell *old = hash->cells;
    struct cell *end = hash->cells + hash->size;
    /* setup new cells */
    hash->cells = pitw_mem_alloc_zero(size * sizeof(hash->cells[0]));
    hash->used = 0;
    hash->size = size;
    /* rehash contents */
    for (struct cell *cell = old; cell != end; ++cell)
        pitw_hash_set(hash, cell->key, cell->val);
    /* release old cells */
    pitw_mem_free(old);
}

void pitw_hash_set(pitw_hash *hash, char *key, void *val) {
    struct cell *cell = get_write_cell(hash, key);
    if (!cell) {
        /* no available slots, resize and retry */
        resize(hash, hash->size * 2);
        pitw_hash_set(hash, key, val);
        return;
    }
    cell->val = val;
    if (cell_empty(cell)) {
        /* insert into empty cell */
        cell->key = key;
        hash->used++;
    }
}

static void fixup(pitw_hash *hash, struct cell *target) {
    struct cell *cell = next_cell(hash, target);
    for (size_t i = 1; i < hash->size; ++i) {
        if (cell_empty(cell))
            return;
        /* determine if this cell should be moved */
        struct cell *ideal = get_ideal_cell(hash, cell->key);
        size_t cell_diff = (cell - ideal) % hash->size;
        size_t target_diff = (target - ideal) % hash->size;
        if (cell_diff > target_diff) {
            /* moving the cell to the target makes it closer to the ideal */
            *target = *cell;
            /* erase this cell and try to fill it */
            cell->key = NULL;
            target = cell;
        }
        cell = next_cell(hash, cell);
    }
}

void pitw_hash_remove(pitw_hash *hash, char *key) {
    struct cell *cell = get_real_cell(hash, key);
    if (!cell)
        return;
    cell->key = NULL;
    hash->used--;
    /* may need to shift other cells if collisions */
    fixup(hash, cell);
}
