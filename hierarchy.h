/*
 * hierarchy.h
 * Defines the data structure and functions for the organizational hierarchy.
 * This module builds and allows navigation of the company structure.
 *
 * AUTHOR: Fiyin Ogboye
 */

#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <stdio.h>
#include <stdlib.h>

#include "employee.h"   // <-- important

Employee *find_ceo(Employee *list, int count);
void      cleanup_hierarchy(Employee *list, int count);
void      build_hierarchy(Employee *list, int count);
void      print_org_chart(Employee *e, int depth);
void      print_chain_of_command(Employee *e);
Employee *find_by_id(Employee *list, int count, int id);
void      list_children(Employee *e);
Employee *go_to_manager(Employee *e);
Employee *go_to_child(Employee *e, int index);

#endif

