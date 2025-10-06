University Management System
A simple C++ program for managing university data - students, professors, and courses. Built this for a project and thought it might be useful for others learning about object-oriented programming and file I/O.

What It Does
This program lets you keep track of:
Students (with their info, addresses, and enrolled courses)
Professors (with their departments and teaching assignments)
Courses (with enrolled students and assigned professors)

Features
Student Management: Store student ID, name, year (freshman through senior), and address
Professor Management: Track professor details including department and address
Course Management: Maintain course codes, titles, and credit hours
Enrollment System: Link students to courses and professors to the courses they teach
Data Persistence: Automatically saves and loads data from CSV files
Smart Memory Management: Uses smart pointers (shared_ptr and weak_ptr) to avoid memory leaks

Project Structure
.
├── main.cpp
└── data/
    ├── students.csv
    ├── professors.csv
    ├── courses.csv
    └── enrollments.csv
Make sure the data/ directory exists before running the program, or you'll get file errors.
 
 How to Build and Run
bashg++ -std=c++17 main.cpp -o university
./university
(You'll need a C++17 compatible compiler)
Quick Example
The main function shows how to use it:
cppuniversity uni;
uni.loaddata();  // Load existing data

// Add new entries
uni.addstudent(4, "Alice Johnson", year::FRESHMAN, {"123 Main St", "CityA"});
uni.addprofessor(4, "Dr. Emily Davis", "Physics", {"456 Elm St", "CityB"});
uni.addcourse(4, "Quantum Mechanics", 4);

// Link them together
uni.assignprofessor(4, 4);  // Assign professor to course
uni.enrollstudent(4, 4);    // Enroll student in course

uni.savedata();  // Save everything back to files
CSV Format
students.csv
id,name,year_number,street,city
1,John Doe,0,123 Oak St,Springfield
professors.csv
id,name,department,street,city
1,Dr. Smith,Computer Science,456 Pine St,Springfield
courses.csv
code,title,credits
101,Data Structures,3
enrollments.csv
student_id,course_id
1,101
