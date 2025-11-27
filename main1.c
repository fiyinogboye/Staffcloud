#include <stdio.h>
#include "employee.h"

int main(int argc, char *argv[]) {
    // We expect: ./program <file>
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    Organization org;

    // Load the organization from the file
    int rc = load_organization(filename, &org);
    if (rc != 0) {
        printf("Failed to load organization from file: %s\n", filename);
        return 1;
    }

    // Print all employees
    print_organization(&org);

    // Free the memory we allocated
    free_organization(&org);

    return 0;
}
