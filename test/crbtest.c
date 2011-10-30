/*
 * Copyright (c) 2011 Eddie Ringle <eddie@eringle.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "crbtest.h"

char *newStr(const char *_str) {
    char *ret = (char *)malloc(strlen(_str) + 1);
    strcpy(ret, _str);
    return ret;
}

int crb_test_llist()
{
    LList *llist;
    void *data;

    printf("Creating LList:\n");
    llist = crb_llist_create(NULL, false);
    assert(llist != NULL);
    printf("\tGOOD\n");

    printf("Testing various impossibilities:\n");
    assert(crb_llist_get(llist, (unsigned int)-1) == NULL);
    assert(crb_llist_get(llist, 1) == NULL);
    assert(crb_llist_get(llist, 0) == NULL);
    printf("\tGOOD.\n");

    printf("Inserting a few strings:\n");
    assert(crb_llist_insert(llist, newStr("one")) == 0);
    assert(crb_llist_insert(llist, newStr("two")) == 1);
    assert(crb_llist_insert(llist, newStr("three")) == 2);
    assert(crb_llist_insert(llist, newStr("four")) == 3);
    printf("\tGOOD.\n");

    printf("Did we really insert them:\n");
    assert(strcmp((char *)crb_llist_get(llist, 0), "one") == 0);
    assert(strcmp((char *)crb_llist_get(llist, 2), "three") == 0);
    assert(strcmp((char *)crb_llist_get(llist, 3), "four") == 0);
    assert(strcmp((char *)crb_llist_get(llist, 1), "two") == 0);
    printf("\tYUP.\n");

    printf("Removing a string:\n");
    data = crb_llist_remove(llist, 1);
    assert(data != NULL);
    free(data);
    printf("\tGOOD.\n");

    printf("Are the other strings intact:\n");
    assert(strcmp((char *)crb_llist_get(llist, 0), "one") == 0);
    assert(strcmp((char *)crb_llist_get(llist, 1), "three") == 0);
    assert(strcmp((char *)crb_llist_get(llist, 2), "four") == 0);
    printf("\tYUP.\n");

    printf("Clearing out the list:\n");
    data = NULL;
    while ((data = crb_llist_remove(llist, 0)) != NULL) {
        free(data);
    }
    printf("\tGOOD.\n");

    printf("Testing impossibilities again:\n");
    assert(crb_llist_get(llist, (unsigned int)-1) == NULL);
    assert(crb_llist_get(llist, 1) == NULL);
    assert(crb_llist_get(llist, 0) == NULL);
    printf("\tGOOD.\n");

    printf("Destroying LList:\n");
    crb_llist_destroy(&llist);
    assert(llist == NULL);
    printf("\tDONE.\n");

    return 0;
}

int crb_test_darray()
{
    DArray *darray;
    void *data;
    size_t pos;
    int i;

    printf("Creating DArray:\n");
    darray = crb_darray_create();
    assert(darray != NULL);
    printf("\tGOOD.\n");

    printf("Inserting some values:\n");
    pos = crb_darray_insert(darray, newStr("one"));
    assert(pos != -1);
    assert(crb_darray_get(darray, pos) != NULL);
    assert(strcmp(crb_darray_get(darray, pos), "one") == 0);
    pos = crb_darray_insert(darray, newStr("three"));
    assert(pos != -1);
    assert(crb_darray_get(darray, pos) != NULL);
    assert(strcmp(crb_darray_get(darray, pos), "three") == 0);
    pos = crb_darray_insert(darray, newStr("five"));
    assert(pos != -1);
    assert(crb_darray_get(darray, pos) != NULL);
    assert(strcmp(crb_darray_get(darray, pos), "five") == 0);
    pos = crb_darray_insert(darray, newStr("seven"));
    assert(pos != -1);
    assert(crb_darray_get(darray, pos) != NULL);
    assert(strcmp(crb_darray_get(darray, pos), "seven") == 0);
    printf("\tGOOD.\n");

    printf("Testing some impossibilities:\n");
    assert(darray->size == 4);
    assert(crb_darray_get(darray, (unsigned int) -1) == NULL);
    assert(crb_darray_get(darray, 3) != NULL);
    printf("\tGOOD.\n");

    printf("Deleting some values:\n");
    data = crb_darray_remove(darray, 0);
    assert(data != NULL);
    free(data);
    assert(darray->size == 3);
    assert(crb_darray_get(darray, 0) == NULL);
    assert(strcmp(crb_darray_get(darray, 1), "three") == 0);
    assert(strcmp(crb_darray_get(darray, 3), "seven") == 0);
    data = crb_darray_remove(darray, 3);
    assert(data != NULL);
    free(data);
    assert(darray->size == 2);
    assert(crb_darray_get(darray, 0) == NULL);
    assert(strcmp(crb_darray_get(darray, 1), "three") == 0);
    assert(strcmp(crb_darray_get(darray, 2), "five") == 0);
    assert(crb_darray_get(darray, 3) == NULL);
    printf("\tGOOD.\n");

    printf("Clearing DArray:\n");
    i = 0;
    while (i < darray->capacity) {
        data = crb_darray_remove(darray, i++);
        if (data != NULL) {
            free(data);
        }
    }
    assert(darray->size == 0);
    printf("\tGOOD.\n");

    printf("Destroying DArray:\n");
    crb_darray_destroy(&darray);
    assert(darray == NULL);
    printf("\tDONE.\n");

    return 0;
}

int crb_test_hashtable()
{
    HashTable *ht;
    char buf1[32];
    char buf2[32];
    const unsigned long max = 20;
    unsigned long i;
    void *data;

    printf("Creating HashTable:\n");
    ht = crb_hashtable_create(-1);
    assert(ht != NULL);
    printf("\tGOOD.\n");

    printf("Inserting pairs:\n");
    for (i = 0; i < max; i++) {
        sprintf(buf1, "%lu", i);
        sprintf(buf2, "%lu", max - i);
        assert(crb_hashtable_insert(ht, buf1, newStr(buf2)));
    }
    printf("\tGOOD.\n");

    printf("Finding every other pair:\n");
    for (i = 0; i < max; i += 2) {
        sprintf(buf1, "%lu", i);
        assert(crb_hashtable_find(ht, buf1) != NULL);
    }
    printf("\tGOOD.\n");

    printf("Deleting every other pair:\n");
    for (i = 0; i < max; i += 2) {
        sprintf(buf1, "%lu", i);
        data = crb_hashtable_remove(ht, buf1);
        assert(data != NULL);
        free(data);
        data = NULL;
    }
    printf("\tGOOD.\n");

    printf("Trying to find those deleted pairs:\n");
    for (i = 0; i < max; i += 2) {
        sprintf(buf1, "%lu", i);
        assert(crb_hashtable_find(ht, buf1) == NULL);
    }
    printf("\tGOOD.\n");

    printf("Matching the remaining pairs:\n");
    for (i = 1; i < max; i += 2) {
        sprintf(buf1, "%lu", i);
        sprintf(buf2, "%lu", max - i);
        assert(strcmp(crb_hashtable_find(ht, buf1), buf2) == 0);
    }
    printf("\tGOOD.\n");

    printf("Deleting the rest of the pairs:\n");
    for (i = 1; i < max; i += 2) {
        sprintf(buf1, "%lu", i);
        data = crb_hashtable_remove(ht, buf1);
        assert(data != NULL);
        free(data);
        data = NULL;
    }
    printf("\tGOOD.\n");

    printf("Make sure table is empty:\n");
    for (i = 0; i < max; i++) {
        sprintf(buf1, "%lu", i);
        assert(crb_hashtable_find(ht, buf1) == NULL);
    }
    printf("\tGOOD.\n");

    printf("Deleting HashTable:\n");
    crb_hashtable_destroy(&ht);
    assert(ht == NULL);
    printf("\tDONE.\n");

    return 0;
}

int main(int argc, char **argv)
{
    printf("\n=== Cerberus Test Suite ===\n\n");

    printf(":: LinkedList ::\n");
    crb_test_llist();

    printf("\n:: DArray ::\n");
    crb_test_darray();

    printf("\n:: HashTable ::\n");
    crb_test_hashtable();

    printf("\nAll done :-)\n");
    return 0;
}
