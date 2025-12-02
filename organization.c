#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "employee.h"

static int count_lines(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return 0;
    }

    //buffer temporairly hols 256 elements
    int count = 0;
    char buffer[256];

    //fgets reads the frist line from fp into buffer

    //while loop ends if it reaches 255 characters since /0 occipies one arealdy
    //or reaches end of file, or hits \n
    //counts the number of lines (the number of workers)
    //fgets means read the first line, fp is what file to read from, size is how many 
    //characters to read, and buffer is where to store the read lines.
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        count++;
    }

    fclose(fp);
    return count;
}

int load_organization(const char *filename, Organization *org) {
    //
    org->list = NULL;
    org->count = 0;

    int line_count = count_lines(filename);

    FILE *fp = fopen(filename, "r");

    //emloyee is a struct. malloc is a block of memroy filled with the bytes  
    //*list is the actaully physical thing it points to. it points to the epmloee struct. 
    //list is malloc, a block of space that reserved  the bytes of all the emplyee info to fill later
    //line_count should be 24, size of employee shuld be around 174
    Employee *list = malloc(line_count * sizeof(Employee));

    int i;
    for (i = 0; i < line_count; i++) {
        // e is a pointer (creates a address) to &list[i] which has the address of list[i].
        // *e represnts the actual struct of empoyees ie salay, id, name
        Employee *e = &list[i];

        //frist loop, if it checks if the person is a ceo, epmplee, manager, or none and appands into role array

        if (i == 0) {
        // CEO: no department
            fscanf(fp, "%d %49s %49s %f",&e->id, e->first, e->last, &e->salary);
            e->manager_id = -1;   
            e->manager    = NULL;
            e->children   = NULL;
            e->child_count = 0;
            strcpy(e->role, "CEO");
            strcpy(e->department, "None");
        }

        else {
            // Managers + Employees: include department
            fscanf(fp, "%d %49s %49s %f %49s", &e->id, e->first, e->last, &e->salary, e->department);
            e->manager_id = -1;   
            e->manager    = NULL;
            e->children   = NULL;
            e->child_count = 0;
            if (i >= 1 && i <= 3) {
                strcpy(e->role, "Manager");
            } 
            else {
                strcpy(e->role, "Employee");
            }
        }
    }

    fclose(fp);

    //org accesses list, so we store list, that holds the adress of malloc into the lsit thats inside the organization struct
    //line_coutn is 24, so we store 24 in count thats located inside the organization struct
    org->list = list;
    org->count = line_count;

    return 0; 
}

void free_organization(Organization *org) {
    if (!org) return;

    //releases the memory of org so it resets to 0
    free(org->list);
    org->list = NULL;
    org->count = 0;
}

void print_organization(const Organization *org) {
    //if the adress of org or list inside struct org is empty
    if (!org || !org->list) {
        printf("Organization is empty.\n");
        return;
    }

    printf("Organization has %d employees:\n", org->count);

    for (int i = 0; i < org->count; i++) {
        // e is the same adress &org->list[i]
        // *e points to the contents of the list array
        // *e will be lists[i], the frist emplyee, 2nd, ... 24th. the lsit will have all d, salary, etc...
        const Employee *e = &org->list[i];
        // Print this employee's information on one line
        printf("ID: %d  Name: %s %s  Role: %s  Salary: %.2f  Dept: %s\n", e->id, e->first, e->last, e->role, e->salary, e->department);
    }
}
