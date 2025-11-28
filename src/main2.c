/*
 * main2.c
 * Temporary test for the hierarchy
 * Used to verify build_hierarchy, navigation, and cleanup functions independently.
 *
 * AUTHOR: Fiyin Ogboye
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "employee.h"
#include "search.h"      // your SEARCH_ANALYTICS_H header
#include "hierarchy.h"   // hierarchy functions

// Small helper to clear leftover characters from stdin
static void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* throw away */ ;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <organization_file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    Organization org;

    // 1) Load all employees from file into org.list
    if (load_organization(filename, &org) != 0) {
        printf("Failed to load organization from file: %s\n", filename);
        return 1;
    }

    printf("Loaded organization from '%s'.\n", filename);
    print_organization(&org);

    // 2) Build the hierarchy (manager / children pointers)
    build_hierarchy(org.list, org.count);

    int choice = 0;
    while (1) {
        printf("\n================ StaffCloud Menu ================\n");
        printf("1) Print all employees\n");
        printf("2) Search employee by ID\n");
        printf("3) Search employees by name\n");
        printf("4) Search employees by department\n");
        printf("5) Search employees by salary range\n");
        printf("6) Show full organization chart (from CEO)\n");
        printf("7) Show chain of command for an employee (by ID)\n");
        printf("8) Exit\n");
        printf("Enter choice (1-8): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }
        clear_stdin();

        if (choice == 8) {
            break;
        }

        switch (choice) {
        case 1: { // Print all employees
            print_organization(&org);
            break;
        }

        case 2: { // Search by ID
            int id;
            printf("Enter employee ID: ");
            if (scanf("%d", &id) != 1) {
                printf("Invalid ID.\n");
                clear_stdin();
                break;
            }
            clear_stdin();

            Employee *e = findEmployeeById(&org, id);
            if (e) {
                printf("\nEmployee found:\n");
                displayEmployee(e);
            } else {
                printf("No employee with ID %d.\n", id);
            }
            break;
        }

        case 3: { // Search by name
            char name[64];
            printf("Enter part of first or last name: ");
            if (!fgets(name, sizeof(name), stdin)) {
                printf("Input error.\n");
                break;
            }
            name[strcspn(name, "\n")] = '\0';  // remove newline
            searchEmployeesByName(&org, name);
            break;
        }

        case 4: { // Search by department
            char dept[64];
            printf("Enter part of department name: ");
            if (!fgets(dept, sizeof(dept), stdin)) {
                printf("Input error.\n");
                break;
            }
            dept[strcspn(dept, "\n")] = '\0';  // remove newline
            searchEmployeesByDepartment(&org, dept);
            break;
        }

        case 5: { // Search by salary range
            float minS, maxS;
            printf("Enter minimum salary: ");
            if (scanf("%f", &minS) != 1) {
                printf("Invalid value.\n");
                clear_stdin();
                break;
            }
            printf("Enter maximum salary: ");
            if (scanf("%f", &maxS) != 1) {
                printf("Invalid value.\n");
                clear_stdin();
                break;
            }
            clear_stdin();

            searchEmployeesBySalaryRange(&org, minS, maxS);
            break;
        }

        case 6: { // Org chart from CEO
            Employee *ceo = find_ceo(org.list, org.count);
            if (!ceo && org.count > 0) {
                ceo = &org.list[0]; // fallback
            }
            if (ceo) {
                printf("\n=== Organization Chart ===\n");
                print_org_chart(ceo, 0);
            } else {
                printf("No CEO / employees found.\n");
            }
            break;
        }

        case 7: { // Chain of command for an employee
            int id;
            printf("Enter employee ID: ");
            if (scanf("%d", &id) != 1) {
                printf("Invalid ID.\n");
                clear_stdin();
                break;
            }
            clear_stdin();

            Employee *e = find_by_id(org.list, org.count, id);
            if (!e) {
                printf("No employee with ID %d.\n", id);
            } else {
                print_chain_of_command(e);
            }
            break;
        }

        default:
            printf("Please choose a number between 1 and 8.\n");
            break;
        } // end switch
    } // end while

    // 3) Clean up hierarchy + organization memory
    cleanup_hierarchy(org.list, org.count);
    free_organization(&org);

    printf("Goodbye!\n");
    return 0;
}
