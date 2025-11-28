#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hierarchy.h"


// Helper to print "First Last" nicely
static void print_employee_name(const Employee *e) {
    printf("%s %s", e->first, e->last);
}

// Find the CEO: by convention, manager_id == -1
Employee *find_ceo(Employee *list, int count) {
    if (!list) return NULL;

    for (int i = 0; i < count; i++) {
        if (list[i].manager_id == -1) {
            return &list[i];
        }
    }
    return NULL;
}

// Free children arrays allocated in build_hierarchy
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
    printf("\n--- Memory for hierarchy children arrays has been cleaned up. ---\n");
}

// Build a simple hierarchy:
//  - find the CEO (role == "CEO")
//  - CEO gets manager_id = -1
//  - everyone else gets manager_id = CEO's id
//  - set manager / children accordingly
void build_hierarchy(Employee *list, int count) {
    if (!list || count <= 0) return;

    // 1) Find CEO by role
    Employee *ceo = NULL;
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].role, "CEO") == 0) {
            ceo = &list[i];
            break;
        }
    }

    if (!ceo) {
        fprintf(stderr, "Warning: no CEO found (role == \"CEO\"). Using first employee as CEO.\n");
        ceo = &list[0];
    }

    // 2) Initialize all hierarchy fields
    for (int i = 0; i < count; i++) {
        list[i].children    = NULL;
        list[i].child_count = 0;
        list[i].manager     = NULL;
        list[i].manager_id  = ceo->id;  // temporary; we'll fix CEO next
    }

    // CEO has no manager
    ceo->manager_id = -1;
    ceo->manager    = NULL;

    // 3) Second pass: build children arrays using manager pointers
    for (int i = 0; i < count; i++) {
        Employee *e = &list[i];

        // Skip CEO (no manager)
        if (e == ceo) continue;

        // For this simple version, everyone reports to CEO
        e->manager = ceo;

        // Grow CEO's children array with realloc
        ceo->child_count++;
        ceo->children = realloc(ceo->children,
                                ceo->child_count * sizeof(Employee *));
        if (!ceo->children) {
            fprintf(stderr, "Error: realloc failed in build_hierarchy.\n");
            ceo->child_count = 0;
            return;
        }

        ceo->children[ceo->child_count - 1] = e;
    }
}

// Recursively prints the organizational chart with indentation.
void print_org_chart(Employee *e, int depth) {
    if (!e) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");   // two spaces per depth level
    }

    printf("%s %s (%s, ID %d)\n",
           e->first, e->last, e->role, e->id);

    // Recurse over children
    for (int i = 0; i < e->child_count; i++) {
        print_org_chart(e->children[i], depth + 1);
    }
}

// Prints the reporting chain from an employee up to the CEO.
void print_chain_of_command(Employee *e) {
    if (!e) return;

    printf("Chain of command for ");
    print_employee_name(e);
    printf(":\n");

    Employee *curr = e;
    while (curr != NULL) {
        printf(" -> %s %s (%s)\n", curr->first, curr->last, curr->role);
        curr = curr->manager;  // move up
    }
}

// Searches for an employee by ID in a flat list.
Employee *find_by_id(Employee *list, int count, int id) {
    if (!list) return NULL;

    for (int i = 0; i < count; i++) {
        if (list[i].id == id) {
            return &list[i];
        }
    }
    return NULL;
}

// Prints the direct reports of an employee (uses the children array).
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

// Returns the manager of the given employee
Employee *go_to_manager(Employee *e) {
    if (!e) return NULL;
    return e->manager;
}

// Returns the child at the given index
Employee *go_to_child(Employee *e, int index) {
    if (!e) return NULL;
    if (index < 0 || index >= e->child_count) return NULL;
    return e->children[index];
}
