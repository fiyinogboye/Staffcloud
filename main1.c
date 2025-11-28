#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "employee.h"
#include "search.h"     
#include "hierarchy.h" 

// Small helper to clear leftover characters
static void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <organization_file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    Organization org;

    //Load all employees from file into org.list
    if (load_organization(filename, &org) != 0) {
        printf("Failed to load organization from file: %s\n", filename);
        return 1;
    }

    printf("Loaded organization from '%s'.\n", filename);
    print_organization(&org);

    //Build the hierarchy
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
        // Print all employees
        case 1: { 
            print_organization(&org);
            break;
        }

        // Search by ID
        case 2: { 
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

        // Search by name
        case 3: { 
            char name[64];
            printf("Enter part of first or last name: ");
            if (!fgets(name, sizeof(name), stdin)) {
                printf("Input error.\n");
                break;
            }
            
            name[strcspn(name, "\n")] = '\0';  
            searchEmployeesByName(&org, name);
            break;
        }
        // Search by department
        case 4: { 
            char dept[64];
            printf("Enter part of department name: ");
            if (!fgets(dept, sizeof(dept), stdin)) {
                printf("Input error.\n");
                break;
            }
            dept[strcspn(dept, "\n")] = '\0';  
            searchEmployeesByDepartment(&org, dept);
            break;
        }
        
        // Search by salary range
        case 5: { 
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

        // Org chart from CEO
        case 6: { 
            Employee *ceo = find_ceo(org.list, org.count);
            if (!ceo && org.count > 0) {
                ceo = &org.list[0];
            }
            if (ceo) {
                printf("\n=== Organization Chart ===\n");
                print_org_chart(ceo, 0);
            } else {
                printf("No CEO / employees found.\n");
            }
            break;
        }

        // Chain of command for an employee
        case 7: { 
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
        }
    }

    //cleans up hierarchy and organization memory
    cleanup_hierarchy(org.list, org.count);

    printf("Goodbye!\n");
    return 0;
}
