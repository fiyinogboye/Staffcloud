#ifndef HIERARCHY_H
#define HIERARCHY_H

typedef struct Employee {
    int id;
    char name[64];
    char department[64];
    float salary;
    int manager_id;                // from file

    struct Employee *manager;      // linked upward
    struct Employee **children;    // dynamic child list
    int children_count;
} Employee;

// core construction
Employee *find_ceo(Employee *list, int count);
void build_hierarchy(Employee *list, int count);

// display
void print_org_chart(Employee *root, int level);

// chain-of-command
void print_chain_of_command(Employee *e);

#endif
