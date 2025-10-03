#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <memory>
#include <string>

using namespace std;

enum class year { FRESHMAN, SOPHOMORE, JUNIOR, SENIOR };

struct address {
    string street;
    string city;
};

class course;
class professor;

class student {
private:
    int id;
    string name;
    year yr;
    address addr;
    vector<weak_ptr<course>> enrolledcourses;
public:
    student(int id, const string& name, year yr, const address& addr)
        : id(id), name(name), yr(yr), addr(addr) {}

    int getid() const { return id; }
    string getname() const { return name; }
    year getyear() const { return yr; }
    address getaddress() const { return addr; }

    void addcourse(shared_ptr<course> c) {
        enrolledcourses.push_back(c);
    }

    void display() const;
};

class professor {
private:
    int id;
    string name;
    string department;
    address addr;
    vector<weak_ptr<course>> teachingcourses;
public:
    professor(int id, const string& name, const string& department, const address& addr)
        : id(id), name(name), department(department), addr(addr) {}

    int getid() const { return id; }
    string getname() const { return name; }
    string getdepartment() const { return department; }
    address getaddress() const { return addr; }

    void addcourse(shared_ptr<course> c) {
        teachingcourses.push_back(c);
    }

    void display() const;
};

class course : public enable_shared_from_this<course> {
private:
    int code;
    string title;
    int credits;
    weak_ptr<professor> prof;
    vector<weak_ptr<student>> enrolledstudents;
public:
    course(int code, const string& title, int credits)
        : code(code), title(title), credits(credits) {}

    int getcode() const { return code; }
    string gettitle() const { return title; }
    int getcredits() const { return credits; }
    const vector<weak_ptr<student>>& getenrolledstudents() const { return enrolledstudents; }

    void addstudent(shared_ptr<student> s) {
        enrolledstudents.push_back(s);
        s->addcourse(shared_from_this());
    }

    void setprofessor(shared_ptr<professor> p) {
        prof = p;
        p->addcourse(shared_from_this());
    }

    void display() const {
        cout << "Course Code: " << code << ", Title: " << title << ", Credits: " << credits << endl;
        if (auto p = prof.lock()) {
            cout << "Professor: " << p->getname() << endl;
        }
        cout << "Enrolled Students: ";
        for (auto& ws : enrolledstudents) {
            if (auto s = ws.lock()) {
                cout << s->getname() << " ";
            }
        }
        cout << endl;
    }
};

void student::display() const {
    cout << "Student ID: " << id << ", Name: " << name << ", Year: ";
    switch (yr) {
        case year::FRESHMAN: cout << "Freshman"; break;
        case year::SOPHOMORE: cout << "Sophomore"; break;
        case year::JUNIOR: cout << "Junior"; break;
        case year::SENIOR: cout << "Senior"; break;
    }
    cout << ", Address: " << addr.street << ", " << addr.city << endl;
    cout << "Enrolled Courses: ";
    for (const auto& wc : enrolledcourses) {
        if (auto c = wc.lock()) {
            cout << c->gettitle() << " ";
        }
    }
    cout << endl;
}

void professor::display() const {
    cout << "Professor ID: " << id << ", Name: " << name
         << ", Department: " << department
         << ", Address: " << addr.street << ", " << addr.city << endl;
    cout << "Teaching Courses: ";
    for (auto& wc : teachingcourses) {
        if (auto c = wc.lock()) {
            cout << c->getcode() << " ";
        }
    }
    cout << endl;
}

class university {
private:
    map<int, shared_ptr<student>> students;
    map<int, shared_ptr<professor>> professors;
    map<int, shared_ptr<course>> courses;
public:
    void addstudent(int id, const string& name, year yr, const address& addr) {
        students[id] = make_shared<student>(id, name, yr, addr);
    }

    void addprofessor(int id, const string& name, const string& department, const address& addr) {
        professors[id] = make_shared<professor>(id, name, department, addr);
    }

    void addcourse(int code, const string& title, int credits) {
        courses[code] = make_shared<course>(code, title, credits);
    }

    void assignprofessor(int coursecode, int professorid) {
        auto c = courses.find(coursecode);
        auto p = professors.find(professorid);
        if (c != courses.end() && p != professors.end()) {
            c->second->setprofessor(p->second);
        }
    }

    void enrollstudent(int studentid, int coursecode) {
        auto s = students.find(studentid);
        auto c = courses.find(coursecode);
        if (s != students.end() && c != courses.end()) {
            c->second->addstudent(s->second);
        }
    }

    void displaystudents() const {
        for (const auto& [id, s] : students) s->display();
    }

    void displayprofessors() const {
        for (const auto& [id, p] : professors) p->display();
    }

    void displaycourses() const {
        for (const auto& [code, c] : courses) c->display();
    }

    void savedata() const {
        ofstream studentfile("data/students.csv");
        ofstream professorfile("data/professors.csv");
        ofstream coursefile("data/courses.csv");
        ofstream enrollmentfile("data/enrollments.csv");

        for (const auto& [id, s] : students) {
            address a = s->getaddress();
            studentfile << s->getid() << "," << s->getname() << "," << static_cast<int>(s->getyear()) << "," << a.street << "," << a.city << "\n";
        }

        for (const auto& [id, p] : professors) {
            address a = p->getaddress();
            professorfile << p->getid() << "," << p->getname() << "," << p->getdepartment() << "," << a.street << "," << a.city << "\n";
        }

        for (const auto& [code, c] : courses) {
            coursefile << c->getcode() << "," << c->gettitle() << "," << c->getcredits() << "\n";
        }

        enrollmentfile << "student_id,course_id\n";
        for (const auto& [code, c] : courses) {
            for (const auto& ws : c->getenrolledstudents()) {
                if (auto s = ws.lock()) {
                    enrollmentfile << s->getid() << "," << c->getcode() << "\n";
                }
            }
        }
    }

    void loaddata() {
        ifstream studentfile("data/students.csv");
        ifstream professorfile("data/professors.csv");
        ifstream coursefile("data/courses.csv");
        ifstream enrollmentfile("data/enrollments.csv");

        string line;

        while (getline(studentfile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while (getline(ss, token, ',')) tokens.push_back(token);
            if (tokens.size() == 5) {
                int id = stoi(tokens[0]);
                string name = tokens[1];
                year yr = static_cast<year>(stoi(tokens[2]));
                address a = {tokens[3], tokens[4]};
                addstudent(id, name, yr, a);
            }
        }

        while (getline(professorfile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while (getline(ss, token, ',')) tokens.push_back(token);
            if (tokens.size() == 5) {
                int id = stoi(tokens[0]);
                string name = tokens[1];
                string department = tokens[2];
                address a = {tokens[3], tokens[4]};
                addprofessor(id, name, department, a);
            }
        }

        while (getline(coursefile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while (getline(ss, token, ',')) tokens.push_back(token);
            if (tokens.size() == 3) {
                int code = stoi(tokens[0]);
                string title = tokens[1];
                int credits = stoi(tokens[2]);
                addcourse(code, title, credits);
            }
        }

        if (getline(enrollmentfile, line)) { /* skip possible header */ }
        while (getline(enrollmentfile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            vector<string> tokens;
            string token;
            while (getline(ss, token, ',')) tokens.push_back(token);
            if (tokens.size() == 2) {
                int studentid = stoi(tokens[0]);
                int coursecode = stoi(tokens[1]);
                enrollstudent(studentid, coursecode);
            }
        }
    }
};

int main() {
    university uni;
    uni.loaddata();

    cout << "Students:" << endl;
    uni.displaystudents();
    cout << endl;

    cout << "Professors:" << endl;
    uni.displayprofessors();
    cout << endl;

    cout << "Courses:" << endl;
    uni.displaycourses();
    cout << endl;

    address addr1 = {"123 Main St", "CityA"};
    address addr2 = {"456 Elm St", "CityB"};

    uni.addstudent(4, "Alice Johnson", year::FRESHMAN, addr1);
    uni.addprofessor(4, "Dr. Emily Davis", "Physics", addr2);
    uni.addcourse(4, "Quantum Mechanics", 4);
    uni.assignprofessor(4, 4);
    uni.enrollstudent(4, 4);

    cout << "After adding new data:" << endl;
    cout << "Students:" << endl;
    uni.displaystudents();
    cout << endl;

    cout << "Professors:" << endl;
    uni.displayprofessors();
    cout << endl;

    cout << "Courses:" << endl;
    uni.displaycourses();
    cout << endl;

    uni.savedata();

    return 0;
}
