#ifndef SEARCH_ANALYTICS_H
#define SEARCH_ANALYTICS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "employee.h"
#define NAME_LENGTH NAME_LEN
#define DEPT_LENGTH DEPART_LEN

Employee *findEmployeeById(Organization *org, int id);

void displayEmployee(const Employee *emp);

void searchEmployeesByName(const Organization *org, const char *name);
void searchEmployeesByDepartment(const Organization *org, const char *department);
void searchEmployeesBySalaryRange(const Organization *org, float minSalary, float maxSalary);

#endif


