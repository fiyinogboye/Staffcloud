#ifndef SEARCH_ANALYTICS_H
#define SEARCH_ANALYTICS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 50
#define DEPT_LENGTH 50


typedef struct Employee {
	int id;
	char name[NAME_LENGTH];
	char department[DEPT_LENGTH];
	double salary;
	int manager_id;
	struct Employee *next;
}Employee;

Employee* findEmployeeById(Employee **employees, int count, int id);
void searchEmployeesByName(Employee **employees, int count, const char *name);
void searchEmployeesByDepartment(Employee **employees, int count, const char *department);
void searchEmployeesBySalaryRange(Employee **employees, int count, double minSalary, double maxSalary);
void displayEmployee(Employee *emp);

#endif


