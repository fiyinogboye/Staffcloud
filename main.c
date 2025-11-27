// test main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search_analytics.h"

// Function to create sample data for testing
void createSampleData(Employee ***employees, int *count) {
    *count = 10;
    *employees = malloc(*count * sizeof(Employee*));
    
    // Create sample employees with some similar names for testing
    (*employees)[0] = malloc(sizeof(Employee));
    *(*employees)[0] = (Employee){100, "Alice Johnson", "Executive", 500000, -1, NULL};
    
    (*employees)[1] = malloc(sizeof(Employee));
    *(*employees)[1] = (Employee){201, "Bob Smith", "Marketing", 120000, 100, NULL};
    
    (*employees)[2] = malloc(sizeof(Employee));
    *(*employees)[2] = (Employee){202, "Charlie Brown", "Sales", 115000, 100, NULL};
    
    (*employees)[3] = malloc(sizeof(Employee));
    *(*employees)[3] = (Employee){203, "Dana Wilson", "IT", 130000, 100, NULL};
    
    (*employees)[4] = malloc(sizeof(Employee));
    *(*employees)[4] = (Employee){301, "Eve Smith", "Marketing", 80000, 201, NULL};
    
    (*employees)[5] = malloc(sizeof(Employee));
    *(*employees)[5] = (Employee){302, "Frank Johnson", "Marketing", 75000, 201, NULL};
    
    (*employees)[6] = malloc(sizeof(Employee));
    *(*employees)[6] = (Employee){303, "Grace Brown", "Sales", 70000, 202, NULL};
    
    (*employees)[7] = malloc(sizeof(Employee));
    *(*employees)[7] = (Employee){304, "Paul Anderson", "IT", 90000, 203, NULL};
    
    (*employees)[8] = malloc(sizeof(Employee));
    *(*employees)[8] = (Employee){305, "Paula Johnson", "Sales", 85000, 202, NULL};
    
    (*employees)[9] = malloc(sizeof(Employee));
    *(*employees)[9] = (Employee){306, "David Smith", "IT", 95000, 203, NULL};
}

// Function to free memory
void cleanup(Employee **employees, int count) {
    for (int i = 0; i < count; i++) {
        free(employees[i]);
    }
    free(employees);
}

int main() {
    Employee **employees;
    int employeeCount;
    
    // Create sample data
    createSampleData(&employees, &employeeCount);
    
    int choice;
    do {
        printf("\n=== StaffCloud App - Search & Filter Menu ===\n");
        printf("1. Search Employee by ID\n");
        printf("2. Search Employees by Name (partial match)\n");
        printf("3. Search Employees by Department\n");
        printf("4. Search Employees by Salary Range\n");
        printf("5. Display All Employees\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: {
                int searchId;
                printf("Enter Employee ID to search: ");
                scanf("%d", &searchId);
                Employee *found = findEmployeeById(employees, employeeCount, searchId);
                displayEmployee(found);
                break;
            }
            case 2: {
                char name[NAME_LENGTH];
                printf("Enter name to search for: ");
                scanf(" %[^\n]", name); // Read string with spaces
                searchEmployeesByName(employees, employeeCount, name);
                break;
            }
            case 3: {
                char department[DEPT_LENGTH];
                printf("Enter department to search for: ");
                scanf(" %[^\n]", department);
                searchEmployeesByDepartment(employees, employeeCount, department);
                break;
            }
            case 4: {
                double minSalary, maxSalary;
                printf("Enter minimum salary: ");
                scanf("%lf", &minSalary);
                printf("Enter maximum salary: ");
                scanf("%lf", &maxSalary);
                searchEmployeesBySalaryRange(employees, employeeCount, minSalary, maxSalary);
                break;
            }
            case 5: {
                printf("\n=== All Employees ===\n");
                for (int i = 0; i < employeeCount; i++) {
                    if (employees[i] != NULL) {
                        displayEmployee(employees[i]);
                    }
                }
                break;
            }
            case 0:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
    
    // Cleanup memory
    cleanup(employees, employeeCount);
    
    return 0;
}
