#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hierarchy.h"

// Helper to print "First Last"
static void print_employee_name(const Employee *e) {
    if (!e) return;
    printf("%s %s", e->first, e->last);
}

// Helper: grow a manager's children array and add one child
static void add_child(Employee *manager, Employee *child) {
    if (!manager || !child) return;

    int new_count = manager->child_count + 1;

    Employee **new_children = realloc(manager->children,
                                      new_count * sizeof(Employee *));
    if (!new_children) {
        fprintf(stderr, "Error: not enough memory in add_child.\n");
        return;
    }

    manager->children = new_children;
    manager->children[manager->child_count] = child;
    manager->child_count = new_count;
}

/*
 * Find the CEO.
 * Rule:
 *   1) First try: role == "CEO"
 *   2) If none found: any employee with manager_id == -1
 *   3) If still none and list not empty: fall back to list[0]
 */
Employee *find_ceo(Employee *list, int count) {
    if (!list || count <= 0) {
        return NULL;
    }

    // 1) Look for role == "CEO"
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].role, "CEO") == 0) {
            return &list[i];
        }
    }

    // 2) Look for manager_id == -1
    for (int i = 0; i < count; i++) {
        if (list[i].manager_id == -1) {
            return &list[i];
        }
    }

    // 3) Fallback: just return the first employee
    return &list[0];
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
    printf("\nMemory used for hierarchy (children arrays) has been cleaned up.\n");
}

/*
 * Build the hierarchy using David's loaded data.
 *       - Find their manager using manager_id
 *       - Set e->manager pointer
 *       - Add e to manager->children list
 */
void build_hierarchy(Employee *list, int count) {
    if (!list || count <= 0) return;

    // 1) Clear all links 
    for (int i = 0; i < count; i++) {
        list[i].manager     = NULL;
        list[i].children    = NULL;
        list[i].child_count = 0;
    }

    // 2) For each employee, hook them to their manager using manager_id
    for (int i = 0; i < count; i++) {
        Employee *e = &list[i];

        // Top-level person (CEO or top) has manager_id == -1
        if (e->manager_id == -1) {
            e->manager = NULL;
            continue;
        }

        // Find the manager in the list: someone whose id == e->manager_id
        Employee *mgr = NULL;
        for (int j = 0; j < count; j++) {
            if (list[j].id == e->manager_id) {
                mgr = &list[j];
                break;
            }
        }

        if (!mgr) {
            // manager_id didn't match anyone – just skip linking
            fprintf(stderr,
                    "Warning: no manager with id %d for employee %d\n",
                    e->manager_id, e->id);
            continue;
        }

        // Link this employee to their manager
        e->manager = mgr;

        // Add this employee as a child of the manager
        add_child(mgr, e);
    }
}

// prints the organizational chart 
void print_org_chart(Employee *e, int depth) {
    if (!e) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");   
    }

    printf("%s %s (%s, ID %d)\n",
           e->first, e->last, e->role, e->id);

    // Print children (direct reports)
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
        curr = curr->manager; 
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

// Prints the direct reports of an employee 
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
