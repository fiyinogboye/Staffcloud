/* StaffCloud main program.
 *
 * Loads an organization from a file and provides
 * a menu to explore employee data.
 *
 * AUTHOR: Team 39
 * DATE:  November 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "employee.h"
#include "search_analytics.h"     
#include "hierarchy.h" 

void print_full_help(void) {
    printf("========================================\n");
    printf(" StaffCloud Employee Management System\n");
    printf("========================================\n\n");

    printf("USAGE:\n");
    printf("./staffcloud <organization_file>\n");
    printf("./staffcloud --help\n");
    printf("./staffcloud -h\n\n");

    printf("<organization_file>:\n");
    printf(" - A text file containing one employee per line.\n");
    printf(" - Each line includes fields such as:\n");
    printf(" ID  FirstName  LastName  Salary  Department/Role\n");
    printf(" Example:\n");
    printf(" 1001 Alice Thompson 150000 CEO\n");
    printf(" 2001 Brian Lee 95000 Sales\n\n");

    printf("PROGRAM FLOW:\n");
    printf("1) The file is loaded into memory.\n");
    printf("2) The hierarchy is built (CEO, managers, employees).\n");
    printf("3) You are shown a menu of options to explore the data.\n\n");

    printf("MENU OPTIONS:\n");
    printf("1) Print all employees\n");
    printf("2) Search employee by ID\n");
    printf("3) Search employees by name\n");
    printf("4) Search employees by department\n");
    printf("5) Search employees by salary range\n");
    printf("6) Show full organization chart (from CEO)\n");
    printf("7) Show chain of command for an employee (by ID)\n");
    printf("8) Show highest and lowest paid employees\n");
    printf("9) Exit\n\n");

    printf("Use --help or -h any time to see this guide again.\n\n");
}

static void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {

    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <organization_file>\n", argv[0]);
        return 1;
    }

    const char *arg = argv[1];

    if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
        print_full_help();
        return 0;
    }

    const char *filename = argv[1];
    Organization org;

    if (load_organization(filename, &org) != 0) {
        printf("Failed to load organization from file: %s\n", filename);
        return 1;
    }

    printf("Loaded organization from '%s'.\n", filename);
    print_organization(&org);

    build_hierarchy(org.list, org.count);

    int choice = 0;
    while (1) {
        char input[32];

        printf("\n================ StaffCloud Menu ================\n");
        printf("1) Print all employees\n");
        printf("2) Search employee by ID\n");
        printf("3) Search employees by name\n");
        printf("4) Search employees by department\n");
        printf("5) Search employees by salary range\n");
        printf("6) Show full organization chart (from CEO)\n");
        printf("7) Show chain of command for an employee (by ID)\n");
        printf("8) Show highest and lowest paid employees\n");
        printf("9) Exit\n");
        printf("Enter choice (1-9, or --help): ");

        if (!fgets(input, sizeof(input), stdin)) {
            printf("Input error. Exiting.\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "--help") == 0 || strcmp(input, "-h") == 0) {
            print_full_help();
            continue; 
        }

        char *endptr = NULL;
        long val = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\0') {
            printf("Invalid input. Please enter 1-9 or --help.\n");
            continue;
        }
        choice = (int)val;

        if (choice == 9) {
            break;
        }

        switch (choice) {
        case 1: { 
            print_organization(&org);
            break;
        }

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

        case 8: { 
            findHighestAndLowestSalary(&org);
            break;
        }

        default:
            printf("Please choose a number between 1 and 9.\n");
            break;
        } 
    } 

    cleanup_hierarchy(org.list, org.count);
    free_organization(&org);

    printf("Goodbye!\n");
    return 0;
}
