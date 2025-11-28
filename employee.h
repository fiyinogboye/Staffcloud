#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#define NAME_LEN   50
#define DEPART_LEN 50

typedef struct Employee {
    int id;
    char first[NAME_LEN];
    char last[NAME_LEN];
    float salary;
    char department[DEPART_LEN];
    char role[16];

    int manager_id;              
    struct Employee *manager;    
    struct Employee **children;  
    int child_count; 

} Employee;

typedef struct {
    Employee *list;
    int count;
} Organization;

int load_organization(const char *filename, Organization *org);
void free_organization(Organization *org);
void print_organization(const Organization *org);

#endif
