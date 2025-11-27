/*
 * hierarchy.h
 * Defines the data structure and functions for the organizational hierarchy.
 * This module builds and allows navigation of the company structure.
 *
 * AUTHOR: Fiyin Ogboye
 */

#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Employee {
    int id;
    const char *name;
    const char *role;
    double salary;
    int manager_id; // ID used to link employees when loading from file

    struct Employee *manager;      // Upward link: Pointer to the direct manager
    
    // Dynamic Memory: Array of pointers to subordinates (required for constraint)
    struct Employee **children;    
    int child_count;               // Current number of direct reports
} Employee;

Employee *find_ceo(Employee *list, int count);

// Links managers (upward pointers) and subordinates.
void build_hierarchy(Employee *list, int count);

// Frees the memory allocated for the 'children' arrays .
void cleanup_hierarchy(Employee *list, int count);

// Recursively prints the organizational chart with indentation.
void print_org_chart(Employee *e, int depth);

// Prints the reporting chain from a given employee up to the CEO.
void print_chain_of_command(Employee *e);

// Searches the employee list by ID.
Employee* find_by_id(Employee *list, int count, int id);

// Prints the direct reports of a specific manager.
void list_children(Employee *e);

// Returns the manager of the given employee (moves one level up).
Employee* go_to_manager(Employee *e);

// Returns the child at the given index (used for navigating down the reports).
Employee* go_to_child(Employee *e, int index);

#endif