#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <limits>

using namespace std;
namespace fs = std::filesystem;

const string DIRECTORY_PATH = "C:/Users/GPrice/CLionProjects/Project2/";

struct Course {
    string courseNumber; // The course number
    string courseTitle; // The title of the course
    vector<string> prerequisites; // List of prerequisite courses
};

vector<Course> courses; // Vector to store courses data

// Function to load data from a file into the data structure
void loadDataStructure(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Unable to open file: " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        Course course;
        size_t pos = line.find(",");
        course.courseNumber = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        course.courseTitle = line.substr(0, pos);
        line.erase(0, pos + 1);

        // Extracting prerequisites from the line
        while (!line.empty()) {
            pos = line.find(",");
            string prerequisite = line.substr(0, pos);
            course.prerequisites.push_back(prerequisite);
            if (pos == string::npos) break;
            line.erase(0, pos + 1);
        }

        courses.push_back(course);
    }

    file.close();
}

// Function to print the list of courses
void printCourseList() {
    // Sorting courses by course number
    sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
    });

    cout << "Here is a list of courses:" << endl;
    for (const auto& course : courses) {
        cout << course.courseNumber << ", " << course.courseTitle << endl;
    }
}

// Function to print details of a specific course
void printCourse(const string& courseNumber) {
    // Convert the course number to uppercase
    string upperCourseNumber = courseNumber;
    transform(upperCourseNumber.begin(), upperCourseNumber.end(), upperCourseNumber.begin(), ::toupper);

    auto it = find_if(courses.begin(), courses.end(), [&](const Course& c) {
        return c.courseNumber == upperCourseNumber;
    });

    if (it != courses.end()) {
        cout << it->courseNumber << ", " << it->courseTitle << endl;
        cout << "Prerequisites: ";
        if (it->prerequisites.empty()) {
            cout << "None" << endl;
        } else {
            for (const auto& prerequisite : it->prerequisites) {
                cout << prerequisite << ", ";
            }
            cout << endl;
        }
    } else {
        cout << "Course not found." << endl;
    }
}

int main() {
    string fileName;
    int choice;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;
    do {
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Available files in directory" << endl;
                cout << "Please select a file including the extension in lowercase:" << endl;
                // List files in the directory and display them as options
                for (const auto &entry: fs::directory_iterator(DIRECTORY_PATH)) {
                    cout << entry.path().filename() << endl;
                }
                cout << "Enter the file name: ";
                cin >> fileName;
                // Add the directory path to the filename
                fileName = DIRECTORY_PATH + fileName;
                loadDataStructure(fileName);
                break;
            case 2:
                printCourseList();
                break;
            case 3:
                cout << "What course do you want to know about? ";
                // Clear input buffer and discard leading whitespace characters
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                // Use getline to read the entire line, including whitespace
                getline(cin, courseNumber);
                // If the input is empty or contains only whitespace
                if (courseNumber.empty()) {
                    cout << "Invalid input. Please enter a valid course number." << endl;
                    break;
                }
                // Convert the user input to uppercase
                transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
                printCourse(courseNumber);
                break;

            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                cout << choice << " is not a valid option." << endl;
        }
    } while (choice != 9);

    return 0;
}
