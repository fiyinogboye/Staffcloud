#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hierarchy.h"

// Finds the CEO (the root of the hierarchy).
// The CEO is identified by having a manager_id of -1.
Employee *find_ceo(Employee *list, int count) {
    for (int i = 0; i < count; i++) {
        if (list[i].manager_id == -1) {
            return &list[i];
        }
    }
    return NULL; 
}

// Iterates through the list and calls free() on the 'children' arrays 
// that were allocated using realloc in build_hierarchy.
void cleanup_hierarchy(Employee *list, int count) {
    for (int i = 0; i < count; i++) {
        // Only managers/CEO (who have children) will have non-NULL memory to free
        if (list[i].children != NULL) {
            free(list[i].children);
            list[i].children = NULL; // Reset pointer after freeing
            list[i].child_count = 0;
        }
    }
    printf("\n--- Memory for all children arrays has been cleaned up . ---\n");
}

// Constructs the hierarchy by linking managers and subordinates.
void build_hierarchy(Employee *list, int count) {
    // PASS 1: Initialize pointers and set Upward (manager) links
    for (int i = 0; i < count; i++) {
        // Reset dynamic child array pointers
        list[i].children = NULL;
        list[i].child_count = 0;

        if (list[i].manager_id == -1) {
            list[i].manager = NULL;  // CEO (Root)
        } else {
            list[i].manager = NULL;
            // Search for the manager by ID
            for (int j = 0; j < count; j++) {
                if (list[j].id == list[i].manager_id) {
                    list[i].manager = &list[j]; // Set upward link
                    break;
                }
            }
            if (list[i].manager == NULL) {
                // Critical error handling: should not occur with clean data
                fprintf(stderr, "Error: Manager ID %d for employee %s not found in list.\n", 
                        list[i].manager_id, list[i].name);
            }
        }
    }
    
    // PASS 2: Set Downward (children) links using dynamic memory (realloc)
    // We use realloc here because we don't know the final number of reports a manager will have.
    for (int i = 0; i < count; i++) {
        if (list[i].manager != NULL) {
            Employee *boss = list[i].manager;

            // 1. Increment count and use realloc to expand the memory block
            boss->child_count++;
            boss->children = realloc(
                boss->children, 
                boss->child_count * sizeof(Employee *) // New size in bytes
            );
            
            // 2. Add the pointer to the current employee to the expanded array
            boss->children[boss->child_count - 1] = &list[i];
        }
    }
}

// Recursively prints the organizational chart with indentation.
void print_org_chart(Employee *e, int depth) {
    if (!e) return;

    // spacing using two spaces per depth level to show depth
    for (int i = 0; i < depth; i++)
        printf("  ");

    printf("%s (%s, ID %d)\n", e->name, e->role, e->id);

    // Recursively call for all children
    for (int i = 0; i < e->child_count; i++) {
        print_org_chart(e->children[i], depth + 1);
    }
}

// Prints the reporting chain from an employee up to the CEO.
void print_chain_of_command(Employee *e) {
    if (!e) return;

    printf("Chain of command for %s:\n", e->name);

    Employee *curr = e;
    while (curr != NULL) {
        printf(" -> %s (%s)\n", curr->name, curr->role);
        curr = curr->manager; // Move up the hierarchy using the manager pointer
    }
}

// Searches for an employee in the list by employee ID.
Employee* find_by_id(Employee *list, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (list[i].id == id)
            return &list[i];
    }
    return NULL;
}

// Prints the direct reports of an employee (uses the children array).
void list_children(Employee *e) {
    if (!e) return;

    if (e->child_count == 0) {
        printf("%s has no direct reports.\n", e->name);
        return;
    }

    printf("Direct reports of %s:\n", e->name);
    for (int i = 0; i < e->child_count; i++) {
        printf("  [%d] %s (ID %d)\n",
            i,
            e->children[i]->name,
            e->children[i]->id
        );
    }
}

// Returns the manager of the given employee (moves one level up).
Employee* go_to_manager(Employee *e) {
    if (!e) return NULL;
    return e->manager;
}

// Returns the child at the given index in the children array (moves one level down).
Employee* go_to_child(Employee *e, int index) {
    if (!e) return NULL;
    if (index < 0 || index >= e->child_count) return NULL;
    return e->children[index];
}