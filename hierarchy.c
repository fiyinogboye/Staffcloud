#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hierarchy.h"

Employee *find_ceo(Employee *list, int count) {
    for (int i = 0; i < count; i++) {
        if (list[i].manager_id == -1) {
            return &list[i];
        }
    }
    return NULL;  // if something is wrong
}

void build_hierarchy(Employee *list, int count) {

    // Step 1: initialize children arrays
    for (int i = 0; i < count; i++) {
        list[i].children = NULL;
        list[i].children_count = 0;
        list[i].manager = NULL;
    }

    // Step 2: link each employee to their manager
    for (int i = 0; i < count; i++) {
        if (list[i].manager_id == -1) continue;

        // find the manager
        for (int j = 0; j < count; j++) {
            if (list[j].id == list[i].manager_id) {

                list[i].manager = &list[j];

                // expand manager child list
                list[j].children_count++;
                list[j].children = realloc(
                    list[j].children,
                    list[j].children_count * sizeof(Employee *)
                );
                list[j].children[list[j].children_count - 1] = &list[i];
            }
        }
    }
}

void print_org_chart(Employee *root, int level) {
    if (!root) return;

    for (int i = 0; i < level; i++) printf("  ");
    printf("%s (ID %d)\n", root->name, root->id);

    for (int i = 0; i < root->children_count; i++) {
        print_org_chart(root->children[i], level + 1);
    }
}

void print_chain_of_command(Employee *e) {
    if (!e) return;
    printf("%s (ID %d)\n", e->name, e->id);
    if (e->manager) {
        print_chain_of_command(e->manager);
    }
}
