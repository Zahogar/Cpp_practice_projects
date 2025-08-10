#include <iostream>
#include <vector>
#include <string>

class student {
public:

    std::string name;
    std::vector<float> grades;

    student(std::string nom) {
        name = nom;
    }

    void add_grade(float grade) {
        grades.insert(grades.end(), grade);
    }

    float average() {
        
        float average = 0;
        for (float grade : grades) {
            average += grade;
        }
        average /= grades.size();
        return average;
    }

    char letter_grade(float grade) {
        if (grade > 20) {
            return 1; // this should not be possible
        }
        else {
            if (grade >= 18) return 'A';
            if (grade >= 16) return 'B';
            if (grade >= 14) return 'C';
            if (grade >= 12) return 'D';
            if (grade >= 0) return 'F';
            else {
                return 1; // this should not be possible
            }
        }
    }
};


int main() {
    
    int len;
    
    std::cout << "How many students ?\n";
    std::cin >> len;

    std::cout << "You have " << len << " students.\n";

    std::vector<student> class_of_students;

    for (int i = 0; i < len; i++) {
        
        std::string name;
        float grade;
        int number_of_grades;
        
        std::cout << "\nWhat is the name of the student ?\n";
        std::cin >> name;

        student con = student(name);
        
        std::cout << "\nHow many grades does " << con.name << " have ?\n";
        std::cin >> number_of_grades;

        for (int j = 0; j < number_of_grades; j++) {
            std::cout << "\nType a grade:\n";
            std::cin >> grade;
            
            con.add_grade(grade);
            
            std::cout << "Grade added\n";
        }

        class_of_students.insert(class_of_students.end(), con);
    }

    std::cout << "\n";

    for (student e : class_of_students) {

        float average = e.average();
        if (average > 20) {
            std::cout << "Error, average shouldn't be more than 20\n";
            break;
        }
        else {
            std::cout << e.name << " has an average of " << average << " which is a " << e.letter_grade(average) << "\n";
        }
    }

}