#include <stdio.h>
#include <string.h>
#include "list.h"
#include "util.h"

typedef struct {
    int num;
    char* text;
} TestData;

int comp_test_data(void* ptr1, void* ptr2) {
    TestData* d1 = (TestData*) ptr1;
    TestData* d2 = (TestData*) ptr2;
    return memcmp(d1, d2, sizeof(int));
}

void print_test_data(void* ptr) {
    if (ptr == NULL) {
        printf("TestData: NULL\n");
        return  ;
    }
    TestData td = *(TestData*) ptr;
    printf("TestData: {%d, %s}\n", td.num, td.text);
}

void testInitFreeList() {

    List* list1 = init_list(NULL);

    AssertExit(list1 != NULL);

    List* list2 = init_list(NULL);

    AssertExit(list1->events == list2->events);

    list1->events->free(list1);
    list2->events->free(list2);
    printf("Test \"Init and Free\" pass successful\n");
}

void testPushGetList() {
    List* list = init_list(NULL);

    TestData td[5] = {
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "fore"},
        {5, "five"}
    };

    list->events->push_back(list, td);

    AssertExit( ((TestData*) list->events->front(list))->num == td[0].num);

    list->events->push_back(list, td + 1);
    list->events->push_back(list, td + 2);
    list->events->push_back(list, td + 3);

    AssertExit( ((TestData*) list->events->back(list))->num == td[3].num);

    list->events->push_front(list, td + 4);

    AssertExit( ((TestData*) list->events->front(list))->num == td[4].num);
    AssertExit( ((TestData*) list->events->index(list, 2))->num == td[1].num);
    AssertExit( list->events->size(list) == 5);

    TestData td_dop = {6, "six"};

    list->events->push_index(list, &td_dop, 1);

    AssertExit( list->events->size(list) == 6);
    AssertExit( ((TestData*) list->events->index(list, 1))->num == td_dop.num);
    AssertExit( ((TestData*) list->events->index(list, 2))->num == td[0].num);

    list->events->free(list);
    printf("Test \"Push and Get\" pass successful\n");
}

void testPopList() {
    List* list = init_list(NULL);

    TestData td[5] = {
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "fore"},
        {5, "five"}
    };

    AssertExit(list->events->pop_back(list) == NULL);

    list->events->push_back(list, td);
    list->events->push_back(list, td + 1);
    list->events->push_back(list, td + 2);
    list->events->push_back(list, td + 3);
    list->events->push_back(list, td + 4);

    AssertExit(list->events->size(list) == 5);

    TestData td_5 = *(TestData*) list->events->pop_back(list);

    AssertExit(td_5.num == 5);
    AssertExit(list->events->size(list) == 4);

    TestData td_1 = *(TestData*) list->events->pop_front(list);

    AssertExit(td_1.num == 1);
    AssertExit(list->events->size(list) == 3);

    TestData td_3 = *(TestData*) list->events->pop_index(list, 1);

    AssertExit(td_3.num == 3);
    AssertExit(list->events->size(list) == 2);

    list->events->pop_back(list);
    list->events->pop_back(list);

    AssertExit(list->events->size(list) == 0);

    list->events->free(list);
    printf("Test \"Pop\" pass successful\n");
}

int comp_test_data_char(void* ptr1, void* ptr2) {

    TestData* td1 = (TestData*) ptr1;
    TestData* td2 = (TestData*) ptr2;

    return strncmp(td1->text, td2->text, MIN(strlen(td1->text), strlen(td2->text)));
}
void testFindList() {
    List* list = init_list(comp_test_data);

    TestData td[5] = {
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "fore"},
        {5, "five"}
    };

    TestData f_td = {2, "five"};
    TestData fe_td = {25, "twenty five"};

    list->events->push_back(list, td);
    list->events->push_back(list, td + 1);
    list->events->push_back(list, td + 2);
    list->events->push_back(list, td + 3);
    list->events->push_back(list, td + 4);

    AssertExit(list->events->find(list, td, NULL) == 0);
    AssertExit(list->events->find(list, &f_td, NULL) == 1);
    AssertExit(list->events->find(list, &f_td, comp_test_data_char) == 4);
    AssertExit(list->events->find(list, &fe_td, NULL) == list->events->size(list));

    list->events->free(list);
    printf("Test \"Find\" pass successful\n");
}

int ucomp_test_data(void* ptr1, void* ptr2) {
    return comp_test_data(ptr2, ptr1);
}

void testSortIteratorList() {
    List* list = init_list(comp_test_data);

    TestData td[8] = {
        {3, "three"},
        {1, "one"},
        {6, "six"},
        {5, "five"},
        {2, "two"},
        {8, "eight"},
        {4, "fore"},
        {7, "seven"}
    };

    list->events->push_back(list, td);
    list->events->push_back(list, td + 1);
    list->events->push_back(list, td + 2);
    list->events->push_back(list, td + 3);
    list->events->push_back(list, td + 4);
    list->events->push_back(list, td + 5);
    list->events->push_back(list, td + 6);
    list->events->push_back(list, td + 7);

    list->events->sort(list, NULL);

    size_t index = 1;
    Iterator* iter = list->events->get_iterator(list);

    for_iterator(iter) {
        TestData temp_td = *(TestData*) iter->get_data(iter);
        AssertExit(temp_td.num == index);
        index++;
    }

    free(iter);

    index = 8;

    list->events->sort(list, ucomp_test_data);
    iter = list->events->get_iterator(list);

    for_iterator(iter) {
        TestData temp_td = *(TestData*) iter->get_data(iter);
        AssertExit(temp_td.num == index);
        index--;
    }

    free(iter);

    list->events->free(list);
    printf("Test \"Sort and Iter\" pass successful\n");
}

int test_list_main  (int argc, char** argt) {
    printf("Start test list\n");
    
    testInitFreeList();
    testPushGetList();
    testPopList();
    testFindList();
    testSortIteratorList();
    
    printf("End of test list\n");    
    return 0;
}
