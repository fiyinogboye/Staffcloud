#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "employee.h" 

static int stringContains(const char *str, const char *substr) {
    if (str == NULL || substr == NULL) return 0;

    char strLower[128];
    char substrLower[128];

    size_t i = 0;
    for (; str[i] && i < sizeof(strLower) - 1; i++) {
        strLower[i] = (char)tolower((unsigned char)str[i]);
    }
    strLower[i] = '\0';

    for (i = 0; substr[i] && i < sizeof(substrLower) - 1; i++) {
        substrLower[i] = (char)tolower((unsigned char)substr[i]);
    }
    substrLower[i] = '\0';

    return strstr(strLower, substrLower) != NULL;
}
Employee* findEmployeeById(const Organization *org, int id) {
    if (!org || !org->list) return NULL;

    for (int i = 0; i < org->count; i++) {
        if (org->list[i].id == id) {
            return &org->list[i];
        }
    }
    return NULL;
}


void displayEmployee(const Employee *emp) {
    if (emp == NULL) {
        printf("No employee found.\n");
        return;
    }

    printf("ID: %d\n", emp -> id);
    printf("Name: %s, %s\n", emp -> first, emp -> last);
    printf("Department: %s\n", emp -> department);
    printf("Salary: $%.2f\n", emp -> salary);
    printf("Role: %s\n", emp -> role);
}

void searchEmployeesByName(const Organization *org, const char *name) {
    if (!org || !org->list) {
        printf("Organization is empty.\n");
        return;
    }

    int found = 0;

    for (int i = 0; i < org->count; i++) {
        const Employee *e = &org->list[i];

        if (stringContains(e->first, name) || stringContains(e->last, name)) {
            printf("Match %d:\n", ++found);
            displayEmployee(e);
        }
    }

    if (found == 0) {
        printf("No employees with name containing '%s' were found.\n", name);
    }
}

void searchEmployeesByDepartment(const Organization *org, const char *department) {
    if (!org || !org->list) {
        printf("Organization is empty.\n");
        return;
    }

    int found = 0;

    for (int i = 0; i < org->count; i++) {
        const Employee *e = &org->list[i];

        if (stringContains(e->department, department)) {
            printf("Employee %d:\n", ++found);
            displayEmployee(e);
        }
    }

    if (found == 0) {
        printf("No employees found in the department '%s'.\n", department);
    }
}

void searchEmployeesBySalaryRange(const Organization *org, float minSalary, float maxSalary) {
    if (!org || !org->list) {
        printf("Organization is empty.\n");
        return;
    }

    int found = 0;

    for (int i = 0; i < org->count; i++) {
        const Employee *e = &org->list[i];

        if (e->salary >= minSalary && e->salary <= maxSalary) {
            printf("Employee %d:\n", ++found);
            displayEmployee(e);
        }
    }

    if (found == 0) {
        printf("No employees found in salary range %.2f - %.2f\n",
               minSalary, maxSalary);
    }
}

void findHighestAndLowestSalary(const Organization *org) {
    if (!org || !org->list || org->count == 0) {
        printf("Organization is empty.\n");
        return;
    }

    const Employee *minEmp = &org->list[0];
    const Employee *maxEmp = &org->list[0];


    for (int i = 1; i < org->count; i++) {
        const Employee *e = &org->list[i];

        if (e->salary < minEmp->salary) {
            minEmp = e;
        }
        if (e->salary > maxEmp->salary) {
            maxEmp = e;
        }
    }

    printf("\nLowest paid employee:\n");
    displayEmployee(minEmp);

    printf("\nHighest paid employee:\n");
    displayEmployee(maxEmp);
}
