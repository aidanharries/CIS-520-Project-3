#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "block_store.h"
#include <string.h>
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
 * @function block_store_create
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

/*
 * @function block_store_destroy
 * @brief Deletes block store structure.
 * @param BS device.
*/
void block_store_destroy(block_store_t *const bs)
{
    //Check if block store is not empty
     if (bs != NULL) {
        bitmap_destroy(bs->bitmap); //Free the bitmap for the given block
        free(bs); //free mem
    }
}

/*
 * @function block_store_allocate
 * @brief Searches for a free block, marks it as in use, and returns the block's id
 * @param BS device.
 * @return Allocated block's id, SIZE_MAX on error
*/
size_t block_store_allocate(block_store_t *const bs)
{
    // Check if bs NULL
    if (bs == NULL || bs->bitmap == NULL) return SIZE_MAX;

    // iterate through block store, with i as the id
    for (size_t i = 0; i < block_store_get_total_blocks(); ++i) {
        // Check if the current block is within the reserved range and skip it if so
        if (i >= BITMAP_START_BLOCK && i < BITMAP_START_BLOCK + BITMAP_NUM_BLOCKS) {
            continue;
        }
        // Check if the current block is free        
        if (bitmap_test(bs->bitmap, i) == false) {
            // If free, set and return the ID
            bitmap_set(bs->bitmap, i);
            return i;
        }
    }

    //return SIZE_MAX in no availible/free block
    return SIZE_MAX;
}

/*
 * @function block_store_request
 * @brief Attempts to allocate a specific block by its ID.
 * @param bs A pointer to the block_store structure.
 * @param block_id The ID of the block to allocate.
 * @return True if the block was successfully allocated, False otherwise.
*/
bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    if (bs == NULL || bs->bitmap == NULL || block_id >= block_store_get_total_blocks()) return false;

    if (!bitmap_test(bs->bitmap, block_id)) { // Check if the block is free
        bitmap_set(bs->bitmap, block_id); // Mark it as used
        return true;
    }

    return false; // Block was already in use
}

/*
 * @function block_store_release
 * @brief Frees a specified block by marking it as available for use.
 * @param bs A pointer to the block_store structure.
 * @param block_id The ID of the block to be released.
*/
void block_store_release(block_store_t *const bs, const size_t block_id) 
{
    // Check if bs is valid and the provided block_id is within valid range
    if (bs != NULL && bs->bitmap != NULL && block_id < block_store_get_total_blocks()) {
        // Check if the block is currently allocated (marked as used)
        if (bitmap_test(bs->bitmap, block_id)) {
            // Mark the block as free in the bitmap
            bitmap_reset(bs->bitmap, block_id);
        }
    }
}

/*
 * @function block_store_get_used_blocks
 * @brief Counts the total number of used blocks in the block store.
 * @param bs A pointer to the block_store structure.
 * @return The total number of used blocks or SIZE_MAX on error.
*/
size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    if (bs == NULL || bs->bitmap == NULL) return SIZE_MAX;

    return bitmap_total_set(bs->bitmap) + BITMAP_NUM_BLOCKS;
}

/*
 * @function block_store_get_free_blocks
 * @brief Counts the total number of free blocks in the block store.
 * @param bs A pointer to the block_store structure.
 * @return The total number of free blocks or SIZE_MAX on error.
*/
size_t block_store_get_free_blocks(const block_store_t *const bs) {
    // Check if the provided block store pointer is valid
    if (bs == NULL || bs->bitmap == NULL) {
        return SIZE_MAX; // Return SIZE_MAX on error
    }

    // Get the total number of blocks in the bitmap
    size_t total_blocks = block_store_get_total_blocks();

    // Calculate the number of free blocks by subtracting the used blocks from the total blocks
    size_t used_blocks = block_store_get_used_blocks(bs);
    size_t free_blocks = total_blocks - used_blocks;

    return free_blocks;
}

/*
 * @function block_store_get_total_blocks
 * @return The total number of blocks available in the block store.
*/
size_t block_store_get_total_blocks()
{
    return BLOCK_STORE_NUM_BLOCKS;
}

/*
 * @function block_store_read
 * @brief Reads data from a specified block into a buffer.
 * @param bs A pointer to the block_store structure.
 * @param block_id The ID of the block to read from.
 * @param buffer The buffer the data should be read into.
 * @return The number of bytes read or 0 on error.
*/
size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer) {
    if (bs == NULL || bs->data == NULL || buffer == NULL || block_id >= block_store_get_total_blocks()) return 0;

    // Copy data from the specified block into the buffer
    memcpy(buffer, bs->data + (block_id * BLOCK_SIZE_BYTES), BLOCK_SIZE_BYTES);
    return BLOCK_SIZE_BYTES;
}

/*
 * @function block_store_write
 * @brief Writes data from a buffer to a specified block.
 * @param bs A pointer to the block_store structure.
 * @param block_id The ID of the block to write to.
 * @param buffer The buffer containing the data to be written.
 * @return The number of bytes written or 0 on error.
*/
size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer) {
    if (bs == NULL || bs->data == NULL || buffer == NULL || block_id >= block_store_get_total_blocks()) return 0;

    // Copy data from the buffer to the specified block
    memcpy(bs->data + (block_id * BLOCK_SIZE_BYTES), buffer, BLOCK_SIZE_BYTES);
    return BLOCK_SIZE_BYTES;
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