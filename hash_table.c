#include <stdlib.h>
#include <stdio.h>

#include "hash_table.h"

int ARRAY_MULTIPLIER = 10;


// Robert Jenkins' 32 bit integer hash function (adapted)
// https://gist.github.com/badboy/6267743

uint32_t hash( uint32_t a, int size) {

   return a % size;
}


// Initialize the components of a hashtable.
// The size parameter is the expected number of elements to be inserted.
// This method returns an error code, 0 for success and -1 otherwise (e.g., if the parameter passed to the method is not null, if malloc fails, etc).
int allocate(hashtable** ht, int size) {

    if (*ht != NULL){
        printf("Hashtable pointer is not null.\n");
        return -1;
    }

    *ht = malloc(sizeof(hashtable));

    if (*ht == NULL){
        printf("Hashtable malloc failed.\n");
        return -1;
    }
    (*ht)->array = malloc(ARRAY_MULTIPLIER * size * sizeof(node*));
    if ((*ht)->array == NULL){
        printf("Array malloc failed.\n");
        return -1;
    }

    (*ht)->size = size * ARRAY_MULTIPLIER;


    return 0;
}


// This method inserts a key-value pair into the hash table.
// It returns an error code, 0 for success and -1 otherwise (e.g., if malloc is called and fails).
int put(hashtable* ht, keyType key, valType value) {
    uint32_t hashed_key = hash(key, ht->size);

    node* new_node = malloc(sizeof(node));

    if (new_node == NULL){
        printf("Node malloc failed.\n");
        return -1;
    }

    new_node->key = key;
    new_node->value = value;
    new_node->next = ht->array[hashed_key];

    ht->array[hashed_key] = new_node;
    return 0;
}

// This method retrieves entries with a matching key and stores the corresponding values in the
// values array. The size of the values array is given by the parameter
// num_values. If there are more matching entries than num_values, they are not
// stored in the values array to avoid a buffer overflow. The function returns
// the number of matching entries using the num_results pointer. If the value of num_results is greater than
// num_values, the caller can invoke this function again (with a larger buffer)
// to get values that it missed during the first call. 
// This method returns an error code, 0 for success and -1 otherwise (e.g., if the hashtable is not allocated).
int get(hashtable* ht, keyType key, valType *values, int num_values, int* num_results) {

    if (ht == NULL) {
        return -1;
    }

    uint32_t hashed_key = hash(key, ht->size);

    int key_matches = 0;
    node* current_node = ht->array[hashed_key];

    
    while (current_node != NULL) {
        if (key == current_node->key) {
            if (key_matches < num_values){
                values[key_matches] = current_node->value;
            }
            key_matches++;
        }
        current_node = current_node->next; 
    }
    *num_results = key_matches;

    return 0;
}

// This method erases all key-value pairs with a given key from the hash table.
// It returns an error code, 0 for success and -1 otherwise (e.g., if the hashtable is not allocated).
int erase(hashtable* ht, keyType key) {

    if (ht == NULL) {
        return -1;
    }

    uint32_t hashed_key = hash(key, ht->size);
    node** origin = &(ht->array[hashed_key]);
    node* current_node = ht->array[hashed_key];
    node* next_node;

    while(current_node != NULL){
        next_node = current_node->next;
        if (current_node->key == key) {
            *origin = next_node;
            free(current_node);
        }
        else {
            origin = &((*origin)->next);
        }
        current_node = next_node;
    }
    return 0;
}

// This method frees all memory occupied by the hash table.
// It returns an error code, 0 for success and -1 otherwise.
int deallocate(hashtable* ht) {
    if (ht == NULL) {
        return -1;
    }

    node* current_node;
    node* next_node;

    for (int i = 0; i < ht->size; ++i){
        current_node = ht->array[i];
        while (current_node != NULL){
            next_node = current_node->next;
            free(current_node);
            current_node = next_node;
        }
    }
    return 0;
}
