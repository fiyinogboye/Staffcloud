#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hierarchy.h"

static void print_employee_name(const Employee *e) {
    printf("%s %s", e->first, e->last);
}

static void add_child(Employee *manager, Employee *child) {
    manager->child_count++;
    Employee **tmp = realloc(manager->children, manager->child_count * sizeof(Employee *));
    if (!tmp) {
        fprintf(stderr, "Error: realloc failed in add_child.\n");
        exit(1);
    }
    manager->children = tmp;
    manager->children[manager->child_count - 1] = child;
}

Employee *find_ceo(Employee *list, int count) {
    if (!list) return NULL;

    for (int i = 0; i < count; i++) {
        if (list[i].manager_id == -1) {
            return &list[i];
        }
    }
    return NULL;
}

void cleanup_hierarchy(Employee *list, int count) {
    if (!list) return;

    for (int i = 0; i < count; i++) {
        if (list[i].children != NULL) {
            free(list[i].children);
            list[i].children = NULL;
        }
        list[i].child_count = 0;
        list[i].manager = NULL;
    }
}

void build_hierarchy(Employee *list, int count) {
    if (!list || count <= 0) return;

    for (int i = 0; i < count; i++) {
        list[i].manager     = NULL;
        list[i].children    = NULL;
        list[i].child_count = 0;
    }

    for (int i = 0; i < count; i++) {
        Employee *e = &list[i];

        if (e->manager_id == -1) {
            continue;
        }

        Employee *mgr = NULL;
        for (int j = 0; j < count; j++) {
            if (list[j].id == e->manager_id) {
                mgr = &list[j];
                break;
            }
        }

        if (!mgr) {
            fprintf(stderr, "Warning: manager with id %d not found for %s %s\n", e->manager_id, e->first, e->last);
            continue;
        }

        e->manager = mgr;
        add_child(mgr, e);
    }
}

void print_org_chart(Employee *e, int depth) {
    if (!e) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");   
    }

    printf("%s %s (%s, ID %d)\n",
           e->first, e->last, e->role, e->id);

    for (int i = 0; i < e->child_count; i++) {
        print_org_chart(e->children[i], depth + 1);
    }
}

void print_chain_of_command(Employee *e) {
    if (!e) return;

    printf("Chain of command for ");
    print_employee_name(e);
    printf(":\n");

    Employee *curr = e;
    while (curr != NULL) {
        printf(" -> %s %s (%s)\n", curr->first, curr->last, curr->role);
        curr = curr->manager; 
    }
}

Employee *find_by_id(Employee *list, int count, int id) {
    if (!list) return NULL;

    for (int i = 0; i < count; i++) {
        if (list[i].id == id) {
            return &list[i];
        }
    }
    return NULL;
}

void list_children(Employee *e) {
    if (!e) return;

    if (e->child_count == 0) {
        printf("%s %s has no direct reports.\n", e->first, e->last);
        return;
    }

    printf("Direct reports of %s %s:\n", e->first, e->last);
    for (int i = 0; i < e->child_count; i++) {
        Employee *child = e->children[i];
        printf("  [%d] %s %s (ID %d, Role %s)\n",
               i, child->first, child->last, child->id, child->role);
    }
}

Employee *go_to_manager(Employee *e) {
    if (!e) return NULL;
    return e->manager;
}

Employee *go_to_child(Employee *e, int index) {
    if (!e) return NULL;
    if (index < 0 || index >= e->child_count) return NULL;
    return e->children[index];
}
