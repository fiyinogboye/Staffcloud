#include <stdio.h>
#include "hierarchy.h"

int main2() {
    // mini test: CEO -> Manager -> Employee
    Employee list[3] = {
        {1, "Alice", "CEO", 120000, -1},
        {2, "Bob", "Sales", 80000, 1},
        {3, "Carol", "Sales", 60000, 2}
    };

    build_hierarchy(list, 3);

    Employee *ceo = find_ceo(list, 3);
    print_org_chart(ceo, 0);

    printf("\nChain of command for Carol:\n");
    print_chain_of_command(&list[2]);

    return 0;
}
