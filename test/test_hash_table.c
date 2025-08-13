#include <stdio.h>
#include <string.h>
#include "hashTable.h"
#include "util.h"

typedef struct TestData
{
    int num;
    char* str;
} TestData;

uint64_t hash_test_data_int(void* ptr) {
    if (ptr == NULL) return 0;
    return hash_f(ptr, sizeof(int));
}

uint64_t hash_test_data_str(void* ptr) {
    if (ptr == NULL) return 0;
    return hash_f((char*) ptr + sizeof(int), strnlen(((TestData*) ptr)->str, 64));
}

int comp_list_td(void* ptr1, void* ptr2) {
    TestData* p_td1 = (TestData*) ptr1;
    TestData* p_td2 = (TestData*) ptr2;

    return (p_td1->num - p_td2->num);
}



void testInitFree() {

    HashTable* ht = init_hash_table(20, hash_test_data_int);

    AssertExit(ht != NULL);
    AssertExit(ht->capacity == 20);

    ht->events->free(ht);
    printf("Test \"Init and Free\" pass successful\n");
}

void testPushGet() {
    HashTable* ht = init_hash_table(16, hash_test_data_int);

    AssertExit(ht != NULL);

    TestData td_arr[5] = {
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "fore"},
        {5, "five"}
    };

    AssertExit( ht->events->push(ht, td_arr) == 1); 
    AssertExit( ht->events->push(ht, td_arr) == 0); 
    AssertExit( ht->events->push(ht, td_arr + 1) == 1); 
    AssertExit( ht->events->push(ht, td_arr + 2) == 1); 
    AssertExit( ht->events->push(ht, td_arr + 3) == 1); 
    AssertExit( ht->events->push(ht, td_arr + 4) == 1);

    TestData td3 = {3, NULL};

    TestData* t = ht->events->getKey(ht, &td3);

    AssertExit(t->num == 3);

    ht->events->free(ht);
    printf("Test \"Push and Get\" pass successful\n");
}

void testSetUpdate() {
    HashTable* ht = init_hash_table(16, hash_test_data_int);

    AssertExit(ht != NULL);

    TestData td_arr[5] = {
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "fore"},
        {5, "five"}
    };

    TestData td_t = {1, "Another"};

    AssertExit( ht->events->set(ht, td_arr) == td_arr); 
    AssertExit( ht->events->set(ht, &td_t) == td_arr); 
    AssertExit( ht->events->update(ht, td_arr + 1) == NULL); 
    AssertExit( ht->events->update(ht, td_arr) == &td_t); 


    ht->events->free(ht);
    printf("Test \"Set and Update\" pass successful\n");
}

void testPopRemove() {
    HashTable* ht = init_hash_table(16, hash_test_data_int);

    TestData td_arr[7] = {
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "fore"},
        {5, "five"},
        {6, "six"},
        {7, "seven"}
    };

    TestData td_t = {9, "Another"};

    AssertExit( ht->events->push(ht, td_arr) == 1);  
    AssertExit( ht->events->push(ht, td_arr + 1) == 1); 
    AssertExit( ht->events->push(ht, td_arr + 2) == 1); 
    AssertExit( ht->events->push(ht, td_arr + 3) == 1); 
    AssertExit( ht->events->push(ht, td_arr + 4) == 1);
    AssertExit( ht->events->push(ht, td_arr + 5) == 1);
    AssertExit( ht->events->push(ht, td_arr + 6) == 1);

    AssertExit( ht->events->size(ht) == 7);

    TestData* td_ptr = ht->events->pop(ht);

    AssertExit( td_ptr->num <= 7);

    TestData td_f = {9, NULL};

    AssertExit( ht->events->push(ht, &td_t) == 1);

    td_ptr = ht->events->removeKey(ht, &td_f);

    AssertExit( td_ptr->num == 9);

    ht->events->free(ht);
    printf("Test \"Pop and Remove\" pass successful\n");
}

int main() {
    printf("Start of test Hash Table!\n");

    testInitFree();
    testPushGet();
    testSetUpdate();
    testPopRemove();

    printf("End of test Hash Table!\n");
    return 0;
}