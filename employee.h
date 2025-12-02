#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#define NAME_LEN   50
#define DEPART_LEN 50

// Name the struct so we can use "struct Employee *" inside it
typedef struct Employee {
    int id;
    char first[NAME_LEN];
    char last[NAME_LEN];
    float salary;
    char department[DEPART_LEN];
    char role[16];

    /* -------- hierarchy-related fields -------- */
    int manager_id;              // -1 for CEO, otherwise id of manager
    struct Employee *manager;    // pointer to direct manager
    struct Employee **children;  // dynamic array of pointers to direct reports
    int child_count;             // number of children
    /* ------------------------------------------ */

} Employee;

typedef struct {
    Employee *list;
    int count;
} Organization;

int load_organization(const char *filename, Organization *org);
void free_organization(Organization *org);
void print_organization(const Organization *org);

#endif
