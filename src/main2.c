/*
 * main2.c
 * Temporary test for the hierarchy
 * Used to verify build_hierarchy, navigation, and cleanup functions independently.
 *
 * AUTHOR: Fiyin Ogboye
 */
#include <stdio.h>
#include "hierarchy.h"

int main() {
    
    // NOTE: We assume the full team's Employee struct definition has 10 fields 
    // due to fields added by other team members (like David or Ifeyinwa).
    // We add two extra initializers (NULL, 0) to ensure the compiler is satisfied.
    Employee list[5] = {
        // [1] ID  [2] Name  [3] Role  [4] Salary  [5] MgrID [6] MgrPtr [7] ChildPtr [8] ChildCount [9] ExtraPtr [10] ExtraInt
        {100, "CEO Alice", "CEO", 250000.0, -1, NULL, NULL, 0, NULL, 0},
        {201, "Manager Bob", "Sales Mgr", 120000.0, 100, NULL, NULL, 0, NULL, 0},
        {202, "Manager Carol", "HR Mgr", 110000.0, 100, NULL, NULL, 0, NULL, 0},
        {301, "Employee Dave", "Sales", 75000.0, 201, NULL, NULL, 0, NULL, 0},
        {302, "Employee Eve", "HR", 72000.0, 202, NULL, NULL, 0, NULL, 0}
    };

    printf("--- StaffCloud Hierarchy Test (Fiyin Ogboye) ---\n");
    
    // 1. Build the hierarchy links (sets manager pointers and dynamic children arrays)
    build_hierarchy(list, 5);

    Employee *ceo = find_ceo(list, 5);

    if (ceo) {
        printf("\n--- Organizational Chart ---\n");
        print_org_chart(ceo, 0);

        // 2. Test Navigation and Chain of Command
        printf("\n--- Test: Chain of Command for Employee Dave ---\n");
        Employee *dave = find_by_id(list, 5, 301);
        if (dave) {
            print_chain_of_command(dave);
        }
        
        printf("\n--- Test: Direct Reports of CEO Alice ---\n");
        list_children(ceo);

    } else {
        printf("\nERROR: CEO not found in the list. Hierarchy build failed.\n");
        // Clean up even if the build failed
        cleanup_hierarchy(list, 5);
        return 1;
    }

    // 4. REQUIRED STEP: Clean up all dynamically allocated memory
    cleanup_hierarchy(list, 5);

    return 0;
}