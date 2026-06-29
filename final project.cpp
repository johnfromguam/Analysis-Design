#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course) {
        if (course.courseNumber == node->course.courseNumber) {
            node->course = course;
        } else if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            } else {
                addNode(node->left, course);
            }
        } else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            } else {
                addNode(node->right, course);
            }
        }
    }

    void inOrder(Node* node) const {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.title << endl;
            inOrder(node->right);
        }
    }

    void destroy(Node* node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

public:
    BinarySearchTree() {
        root = nullptr;
    }

    ~BinarySearchTree() {
        destroy(root);
    }

    void Insert(Course course) {
        if (root == nullptr) {
            root = new Node(course);
        } else {
            addNode(root, course);
        }
    }

    Course Search(string courseNumber) const {
        Node* current = root;

        while (current != nullptr) {
            if (current->course.courseNumber == courseNumber) {
                return current->course;
            }

            if (courseNumber < current->course.courseNumber) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        Course course;
        return course;
    }

    void InOrder() const {
        inOrder(root);
    }
};

string trim(const string& value) {
    size_t first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) {
        return "";
    }

    size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

string toUpperCase(string value) {
    transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
        return static_cast<char>(toupper(character));
    });

    return value;
}

bool loadCourses(const string& fileName, BinarySearchTree& courseTree) {
    ifstream courseFile(fileName);

    if (!courseFile.is_open()) {
        cout << "Error: Could not open file " << fileName << "." << endl;
        return false;
    }

    string line;

    while (getline(courseFile, line)) {
        if (trim(line).empty()) {
            continue;
        }

        stringstream lineStream(line);
        string field;
        vector<string> fields;

        while (getline(lineStream, field, ',')) {
            fields.push_back(trim(field));
        }

        if (fields.size() < 2 || fields.at(0).empty() || fields.at(1).empty()) {
            cout << "Error: Invalid course record skipped: " << line << endl;
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(fields.at(0));
        course.title = fields.at(1);

        for (size_t i = 2; i < fields.size(); ++i) {
            if (!fields.at(i).empty()) {
                course.prerequisites.push_back(toUpperCase(fields.at(i)));
            }
        }

        courseTree.Insert(course);
    }

    return true;
}

void printCourse(const Course& course) {
    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.title << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    } else {
        cout << "Prerequisites: ";

        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites.at(i);

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}

void displayMenu() {
    cout << endl;
    cout << "Menu:" << endl;
    cout << "  1. Load Data Structure" << endl;
    cout << "  2. Print Course List" << endl;
    cout << "  3. Print Course" << endl;
    cout << "  9. Exit" << endl;
    cout << "Enter choice: ";
}

int main() {
    BinarySearchTree courseTree;
    bool dataLoaded = false;
    int choice = 0;

    cout << "Welcome to the ABCU Advising Program." << endl;

    while (choice != 9) {
        displayMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a menu number." << endl;
            continue;
        }

        switch (choice) {
        case 1: {
            string fileName;
            cout << "Enter course data file name: ";
            cin >> fileName;

            if (loadCourses(fileName, courseTree)) {
                dataLoaded = true;
                cout << "Course data loaded successfully." << endl;
            }

            break;
        }
        case 2:
            if (!dataLoaded) {
                cout << "Please load the data structure first." << endl;
            } else {
                cout << endl;
                cout << "Here is a sample schedule:" << endl;
                courseTree.InOrder();
            }

            break;
        case 3:
            if (!dataLoaded) {
                cout << "Please load the data structure first." << endl;
            } else {
                string courseNumber;
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                printCourse(courseTree.Search(toUpperCase(courseNumber)));
            }

            break;
        case 9:
            cout << "Thank you for using the ABCU Advising Program." << endl;
            break;
        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}
