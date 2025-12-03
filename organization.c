/*********************************************************************
 * David Choi, choiy70, December 3rd
 * 
 * Functionality: The main function of this file is to serach for txt
 * files, store and print the contents in the files and free an 
 * organization’s employee data. 
 * 
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "employee.h"

/*********************************************************************
 * Function: count_lines
 * 
 * Functiionality: Counts the number of lines in a given text file
 * 
 * Parameters: 
 * filename: a C string representing the path to a text file.
 * 
 * Return: Returns the number of lines or returns 0 if file
 * could not be opened.
 ********************************************************************/

static int count_lines(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return 0;
    }

    int count = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        count++;
    }

    fclose(fp);
    return count;
}

/*********************************************************************
 * Function: load_organization
 * 
 * Parameters: 
 * Organization *org: a pointer to a struct that will hold the entire 
 * list of employees and the number of employees
 * 
 * Functiionality: Counts the number of lines in a given text file.
 * 
 * Return: Returns 0 when successful, and -1 if file errors occurred
 ********************************************************************/


int load_organization(const char *filename, Organization *org) {
    org->list = NULL;
    org->count = 0;
    

    int line_count = count_lines(filename);

    FILE *fp = fopen(filename, "r");
    Employee *list = malloc(line_count * sizeof(Employee));
    

    int i;
    for (i = 0; i < line_count; i++) {
        Employee *e = &list[i];

        if (i == 0) {
            fscanf(fp, "%d %49s %49s %f", &e->id, e->first, e->last, &e->salary);
            strcpy(e->role, "CEO");
            strcpy(e->department, "None");
        }

        else {
            fscanf(fp, "%d %49s %49s %f %49s", &e->id, e->first, e->last, &e->salary, e->department);

            if (i >= 1 && i <= 3) {
                strcpy(e->role, "Manager");
            } 
            else {
                strcpy(e->role, "Employee");
            }
        }
    }

    fclose(fp);

    int ceo_id = list[0].id;
    list[0].manager_id = -1;

    for (int i = 1; i < line_count; i++) {
        Employee *e = &list[i];

        if (strcmp(e->role, "Manager") == 0) {
            e->manager_id = ceo_id;
        } else if (strcmp(e->role, "Employee") == 0) {
            e->manager_id = -1;  

            for (int j = 1; j < line_count; j++) {
                Employee *m = &list[j];
                if (strcmp(m->role, "Manager") == 0 && strcasecmp(m->department, e->department) == 0) {
                    e->manager_id = m->id;
                    break;
                }
            }

            if (e->manager_id == -1) {
                e->manager_id = ceo_id;
            }
        }
    }

    org->list = list;
    org->count = line_count;

    return 0; 
}

/*********************************************************************
 * Function: free_organization
 * 
 * Functionality: Frees the dynamically allocated employee array 
 * inside the Organization struct.
 *
 * org - pointer to Organization 
 * 
 * Returns: nothing
 ************************************************************/
void free_organization(Organization *org) {
    if (!org) {
        return;
    }

    free(org->list);
    org->list = NULL;
    org->count = 0;
}

/************************************************************
 * Function: print_organization
 * 
 * Functionality: Prints a list of all employees, showing
 * their ID, name, role, salary, and department.
 *
 * Returns: Nothing
 ************************************************************/
void print_organization(const Organization *org) {
    if (!org || !org->list) {
        printf("Organization is empty.\n");
        return;
    }

    printf("Organization has %d employees:\n", org->count);

    for (int i = 0; i < org->count; i++) {
        const Employee *e = &org->list[i];
        printf("ID: %d  Name: %s %s  Role: %s  Salary: %.2f  Dept: %s\n", e->id, e->first, e->last, e->role, e->salary, e->department);
    }
}

