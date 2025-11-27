#include "search_analytics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Employee* findEmployeeById (Employee **employee, int count,int id){
	for (int i= 1; i<count; i++){
		if (employee[i] != NULL && employee[i] -> id ==id) {
			return employee[i];
		}

	}
	return NULL;
}

void displayEmployee( Employee *emp){
	if (emp == NULL){
		printf("No employee found.\n");
	}
	printf("ID: %d\n", emp -> id);
	printf("Name: %s\n", emp -> name);
	printf("Department: %s\n", emp -> department);
    	printf("Salary: $%.2f\n", emp -> salary);
    	printf("Manager ID: %d\n", emp -> manager_id);
}

int stringContains(const char *str, const char *substr) {
    if (str == NULL || substr == NULL) return 0;

    char strLower[NAME_LENGTH];
    char substrLower[NAME_LENGTH];

    //case-insensitive search
    for (int i = 0; str[i]; i++) {
        strLower[i] = tolower(str[i]);
    }
    strLower[strlen(str)] = '\0';

    for (int i = 0; substr[i]; i++) {
        substrLower[i] = tolower(substr[i]);
    }
    substrLower[strlen(substr)] = '\0';

    return strstr(strLower, substrLower) != NULL;
}


void searchEmployeesByName (Employee **employees, int count,const char *name ){
	int found =0;
	for (int i =0; i < count; i++){
		if (employees !=NULL && stringContains(employees[i] -> name, name)){
			printf ("%d\n match(s)", ++found);
			displayEmployee(employees[i]);
		}
	}
	printf("No employee with that name found");

			
}


void searchEmployeesByDepartment(Employee **employees, int count, const char *department) {
	int found = 0;

	for (int i = 0; i < count; i++) {
        	if (employees[i] != NULL && stringContains(employees[i]->department, department)) {
            		printf("Employee %d:\n", ++found);
            		displayEmployee(employees[i]);
        	}
    	}

    	if (found == 0) {
        	printf("No employees found in department '%s'\n", department);
    } 
}


void searchEmployeesBySalaryRange(Employee **employees, int count, double minSalary, double maxSalary) {

	int found = 0;

	for (int i = 0; i < count; i++) {
		if (employees[i] != NULL && employees[i]->salary >= minSalary && employees[i]->salary <= maxSalary) {
            		printf("Employee %d:\n", ++found);
            		displayEmployee(employees[i]);
        	}
    	}

    	if (found == 0) {
        	printf("No employees found in salary range $%.2f - $%.2f\n", minSalary, maxSalary);
    	} 
}
