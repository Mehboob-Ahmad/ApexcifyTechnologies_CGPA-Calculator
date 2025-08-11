#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

class savable {
public:
    virtual void save() = 0;
};

class grades : public savable {
protected:
    string name;
    int marks;
public:
    grades(string n) : name(n), marks(0) {}

    void setMarks() {
        cout << "Enter marks for " << name << ": ";
        cin >> marks;
        while (marks < 0 || marks > 100) {
            cout << "Invalid marks. Please enter a value between 0 and 100: ";
            cin >> marks;
        }
        cout << "Grade: " << getGrade() << endl;
    }

    string getGrade() const {
        if (marks >= 85) return "A";
        else if (marks >= 80) return "A-";
        else if (marks >= 75) return "B+";
        else if (marks >= 70) return "B";
        else if (marks >= 65) return "B-";
        else if (marks >= 60) return "C+";
        else if (marks >= 55) return "C";
        else if (marks >= 50) return "C-";
        else if (marks >= 45) return "D+";
        else if (marks >= 40) return "D";
        else if (marks >= 35) return "D-";
        else return "F";
    }

    int getMarksValue() const { return marks; }
    string getCourseName() const { return name; }

    void save() override {
        cout << "Saving grades for " << name << ": Marks = " << marks << ", Grade = " << getGrade() << endl;
    }
};

class credits : public savable {
protected:
    string courseName;
    int creditHours;
public:
    credits(string c) : courseName(c), creditHours(0) {}

    void setCredits() {
        cout << "Enter credit hours for " << courseName << ": ";
        cin >> creditHours;
        while (creditHours <= 0) {
            cout << "Invalid credit hours. Please enter a positive value: ";
            cin >> creditHours;
        }
    }

    int getCreditHoursValue() const { return creditHours; }
    string getCourseName() const { return courseName; }

    void save() override {
        cout << "Saving credits for " << courseName << ": Credit Hours = " << creditHours << endl;
    }
};

class CGPA : public savable {
protected:
    string studentName;
    vector<grades> gradeList;
    vector<credits> creditList;
    int totalCredits;
    float totalGradePoints;
public:
    CGPA(string name) : studentName(name), totalCredits(0), totalGradePoints(0.0) {}

    void addCourse(string courseName) {
        gradeList.push_back(grades(courseName));
        creditList.push_back(credits(courseName));
    }

    void setAllCredits() {
        for (auto& c : creditList) {
            c.setCredits();
        }
    }

    void setAllMarks() {
        for (size_t i = 0; i < gradeList.size(); i++) {
            gradeList[i].setMarks();
            float gradePoint = calculateGradePoint(gradeList[i].getMarksValue());
            totalGradePoints += gradePoint * creditList[i].getCreditHoursValue();
            totalCredits += creditList[i].getCreditHoursValue();
        }
    }

    float calculateCGPA() const {
        if (totalCredits == 0) return 0.0;
        return totalGradePoints / totalCredits;
    }

    void save() override {
        ofstream file("student_data.txt", ios::app);
        file << fixed << setprecision(2);
        file << "Student: " << studentName << endl;
        for (size_t i = 0; i < gradeList.size(); i++) {
            file << "Course: " << creditList[i].getCourseName()
                << " | Credits: " << creditList[i].getCreditHoursValue()
                << " | Marks: " << gradeList[i].getMarksValue()
                << " | Grade: " << gradeList[i].getGrade() << endl;
        }
        file << "CGPA: " << calculateCGPA() << endl;
        file << "----------------------" << endl;
        file.close();

        cout << fixed << setprecision(2);
        cout << "Saving CGPA for " << studentName << ": CGPA = " << calculateCGPA() << endl;
    }

private:
    float calculateGradePoint(int marks) {
        if (marks >= 85) return 4.0;
        else if (marks >= 80) return 3.7;
        else if (marks >= 75) return 3.3;
        else if (marks >= 70) return 3.0;
        else if (marks >= 65) return 2.7;
        else if (marks >= 60) return 2.3;
        else if (marks >= 55) return 2.0;
        else if (marks >= 50) return 1.7;
        else if (marks >= 45) return 1.3;
        else if (marks >= 40) return 1.0;
        else if (marks >= 35) return 0.7;
        else return 0.0;
    }
};

int main() {
    cout << "Welcome to the CGPA Calculator!" << endl;
    string studentName;
    cout << "Enter student name: ";
    getline(cin, studentName);

    CGPA cgpaCalculator(studentName);

    int numCourses;
    cout << "Enter number of courses: ";
    cin >> numCourses;
    cin.ignore();

    for (int i = 0; i < numCourses; ++i) {
        string courseName;
        cout << "Enter course name: ";
        getline(cin, courseName);
        cgpaCalculator.addCourse(courseName);
    }

    cout << endl << "--- Enter Credit Hours for Each Course ---" << endl;
    cgpaCalculator.setAllCredits();

    cout << endl << "--- Enter Marks for Each Course ---" << endl;
    cgpaCalculator.setAllMarks();

    cgpaCalculator.save();

    cout << fixed << setprecision(2);
    cout << "Final CGPA: " << cgpaCalculator.calculateCGPA() << endl;

    cout << endl << "Data saved in 'student_data.txt' in the same directory as the program." << endl;

    return 0;
}
