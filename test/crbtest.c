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

int main(int argc, char **argv) {
    printf("\n=== Cerberus Test Suite ===\n\n");

    printf(":: LinkedList ::\n");
    crb_test_llist();

    printf("\nAll done :-)\n");
    return 0;
}
