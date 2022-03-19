#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <iomanip>

struct Student {
    int id;
    std::string name;
    int age;
};

bool isFileEmpty(std::ifstream& pFile) {
    return pFile.peek() == std::ifstream::traits_type::eof();
}

int maxElement(std::vector<int> v) {
    int maxElement = v[0];
    for(int i = 1; i < v.size(); i++) {
        maxElement = std::max(v[i],maxElement);
    }
    return maxElement;
}

/*
Comparison function to sort a Student struct vector by their id.
Reference:
https://stackoverflow.com/questions/4892680/sorting-a-vector-of-structs
*/
bool compareById(const Student &a, const Student &b) {
    return a.id < b.id;
}

/*
Read file to get a string 2D vector of all the data
*/
std::vector<std::vector<std::string>> getDataAsVector(std::string fileName) {
    using namespace std;
    vector<vector<string>> data = {};
    ifstream infile(fileName);

    if(isFileEmpty(infile)) return data; 
    while(infile) {
        string s;
        if(!getline(infile,s)) break;

        std::istringstream ss(s);
        std::vector<std::string> record;

        while(ss) {
            string s;
            if(!getline(ss,s,',')) break;
            record.push_back(s);
        }
        data.push_back(record);
    }

    return data;
}

/*
Write all student data into a file
Reference:
https://www.cplusplus.com/doc/tutorial/files/
*/
void passDataToFile(std::vector<Student> students, std::string fileName) {
    using namespace std;
    ofstream file (fileName);
    if(file.is_open()) {
        for(int i = 0; i < students.size();i++) {
            file << students[i].id << "," << students[i].name << "," <<students[i].age << '\n';            
        }
        file.close();
    }
}

/*
Generate student vector from the data as a 2D string vector
*/
std::vector<Student> getStudentVector(std::string fileName) {
    using namespace std;
    vector<Student> students;
    vector<vector<string>> data = getDataAsVector(fileName);
    
    for(auto d:data) {
        Student s;

        int id = stoi(d[0]);
        string name = d[1];
        int age = stoi(d[2]); 

        s.id = id;
        s.name = name;
        s.age = age;
        
        students.push_back(s);
    }

    return students;
}

std::vector<int> getAgeVector(std::vector<Student> students) {
    std::vector<int> ages = {};
    for(auto s:students) {
        ages.push_back(s.age);
    }
    return ages;
}

std::vector<std::string> getNameVector(std::vector<Student> students) {
    std::vector<std::string> names = {};
    for(auto s:students) {
        names.push_back(s.name);
    }
    return names;
}

std::vector<int> getIdVector(std::vector<Student> students) {
    std::vector<int> ids = {};
    for(auto s:students) {
        ids.push_back(s.id);
    }
    return ids;
}

/* 
Find free ids within the data
*/
std::vector<int> missingIds(std::vector<int> ids) {
    std::vector<int> missing = {};
    int maxId = maxElement(ids);
    std::vector<int> expected;
    for(int i = 1; i <= maxId; i++) expected.push_back(i);

    int prev = 0;
    for(int i = 0; i < ids.size(); i++) {
        if(ids[i] - prev > 1) {
            for(int j = prev+1; j < ids[i]; j++) {
                missing.push_back(j);
            }
        }
        prev = ids[i];
    }
    return missing;
}

/*
Add a student to the table
*/
void addStudent(std::vector<Student> students, std::string fileName) {
    using namespace std;
    Student s;

    cout << "Name: ";
    getline(cin >> ws, s.name);
    cout << "Age: ";
    cin >> s.age;
    
    //Assign the ID depending on the missing ones
    vector<int> ids = getIdVector(students);

    vector<int> missing;
    if(!ids.empty()) missing = missingIds(ids);

    if(missing.empty()) {
        s.id = ids.size() + 1;
        students.push_back(s);
        passDataToFile(students,fileName);
    } else {
        s.id = missing[0];
        students.push_back(s);
        sort(students.begin(), students.end(),compareById);
        passDataToFile(students,fileName);
    }
    
}

/*
Drop a student from the table given the ID
*/
void dropStudent(std::vector<Student> students, std::string fileName) {
    using namespace std;
    int id;
    cout << "ID: ";
    cin >> id;
    vector<int> ids = getIdVector(students);
    int studentPos = -1;

    for(int i = 0; i < students.size(); i++) {
        if(students[i].id == id) {
            studentPos = i;
            break;
        }
    }
    students.erase(students.begin() + studentPos);

    if(studentPos < 0) {
        cout << "ERROR: the student " << id << " does not exist within the table." << '\n';
    } else {
        passDataToFile(students,fileName);
        cout << "Deleted student " << id << "\n";
    }
}

void optionMenu() {
    using namespace std;
    cout << "\n\nChoose one of the following actions:\n";
    cout << "1. View students\n";
    cout << "2. Add student\n";
    cout << "3. Drop student\n";
    cout << "4. Exit\n";
}

void printTable(std::vector<Student> students) {
    using namespace std;
    cout << left << setw(10) << "ID" << setw(30) << "Name" << "Age\n\n";
    Student s;
    for(int i = 0; i < students.size(); i++) {
        s = students[i];
        cout << left << setw(10) << s.id << setw(30) << s.name << s.age << "\n";
    }
}
int main() {
    using namespace std;
    std::vector<Student> students;
    int option;
    string fileName = "table";
    while(true) {
        students = getStudentVector(fileName); //Always update the students data structure
        optionMenu();
        cout << "Option: ";
        cin >> option;

        switch (option) {
            case 1:
                printTable(students);
                break;
            case 2:
                addStudent(students,fileName);
                break;
            case 3:
                dropStudent(students,fileName);
                break;
            default:
                cout << "Option unavailable\n";
                break;
        }
    }
    printTable(students);
    return 0;
}