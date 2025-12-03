**StaffCloud **

StaffCloud is a modular, console-based Employee Management System written in C.  
It allows HR personnel and office managers to load, view, and analyze employee data for multiple organizations directly from the terminal. 

**Features**

- Load data for multiple organizations from text files
- Parse and store:
  - 1 CEO
  - 3 managers (departments)
  - 20 employees per organization
- View full organizational hierarchy (CEO → managers → employees)
- Search employees by:
  - ID
  - Department
  - Name
- Sort employees by:
  - ID
  - Name
  - Salary
  - Department
- Basic analytics:
  - Find highest and lowest salary in an organization


**Technology**

- Language: C  
- Interface: Console / terminal (Windows, macOS, Linux)

**File Format**

Each organization is described in a `.txt` file with the following order:

1. 1 CEO  
2. 3 Managers  
3. 20 other Employees  

These are loaded into dynamically allocated structs and arrays to avoid unnecessary fixed-size allocations. 


**How to Run**
make
./staffcloud 'sampleorg.txt'
./staffcloud --help - to see usage options

