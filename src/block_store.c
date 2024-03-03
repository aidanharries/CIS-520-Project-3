#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "block_store.h"
// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

/*
 * @struct block_store
 * @brief Structure representing a block storage system.
*/
typedef struct block_store 
{
    bitmap_t* bitmap;   // Bitmap to track used/free blocks
    uint8_t data[BLOCK_STORE_NUM_BYTES];    // Storage for blocks
} block_store_t;


/*
 * @struct block_store_create
 * @brief Creates and initializes a block store structure.
 * @return A pointer to the newly created block_store structure on sucess, or NULL on failure.
*/
block_store_t *block_store_create()
{
    //Allocate mem for block store struct and initialize all bits to zero
    block_store_t *block = (block_store_t *)calloc(1, sizeof(block_store_t));

    //Error check. Check that allocation worked
    if (block != NULL) {
        //Allocate and initilizae num of stored blocks in bitmap 
        block->bitmap = bitmap_create(BLOCK_STORE_NUM_BLOCKS);

        //Error check that bitmap created succesfully by checking if pointer == Null, else continue/skip 
        if (block->bitmap == NULL) {
            free(block); //free mem
            return NULL; //null on error
        }

        return block;
    }

    return NULL; //null on error
}

void block_store_destroy(block_store_t *const bs)
{
    UNUSED(bs);
}

size_t block_store_allocate(block_store_t *const bs)
{
    UNUSED(bs);
    return 0;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    UNUSED(bs);
    UNUSED(block_id);
    return false;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    UNUSED(bs);
    UNUSED(block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    UNUSED(bs);
    return 0;
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    UNUSED(bs);
    return 0;
}

size_t block_store_get_total_blocks()
{
    return 0;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

block_store_t *block_store_deserialize(const char *const filename)
{
    UNUSED(filename);
    return NULL;
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    UNUSED(bs);
    UNUSED(filename);
    return 0;
}