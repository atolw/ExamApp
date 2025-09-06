#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>

#include "User.h"
#include "Exam.h"

class App {
    std::vector<Exam> exams;
    std::vector<User> users;
    std::size_t nextExamID = 1;

    Exam* findExamByID(std::size_t id) {
        for (auto &exam : exams) if (exam.getExamID() == id) return &exam;
        return nullptr;
    }

    User* findUserByName(const std::string& uname) {
        for (auto &user : users) if (user.getUsername() == uname) return &user;
        return nullptr;
    }

	// створення питання з консолі
    Question createQuestionFromInput() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string text;
        std::cout << "Enter question text: ";
        std::getline(std::cin, text);

        std::vector<std::string> options;
        for (int i = 0; i < 4; ++i) {
            std::string option;
            std::cout << "Option " << i << ": ";
            std::getline(std::cin, option);
            options.push_back(option);
        }

        std::size_t correct;
        std::cout << "Enter index of correct option (0-3): ";
        std::cin >> correct;
        if (correct >= options.size()) {
            throw std::invalid_argument("Correct option out of range.");
        }
        return Question(text, options, correct);
    }

    void adminAddExam() {
        system("cls");
        std::string title, subject;
        std::cout << "Enter exam title: ";
        std::cin >> std::ws;
        std::getline(std::cin, title);
        std::cout << "Enter subject: ";
        std::getline(std::cin, subject);

        Exam e(title, nextExamID++, subject);

        while (true) {
            std::cout << "Add a question? (1 - yes, 0 - no): ";
            int c; std::cin >> c;
            if (!c) break;
            try {
                Question q = createQuestionFromInput();
                e.addQuestion(q);
            } catch (const std::exception& ex) {
                std::cout << "Error: " << ex.what() << "\n";
            }
        }
        exams.push_back(e);
        std::cout << "Exam added with ID " << e.getExamID() << "\n";
    }

    void adminViewExams() {
        system("cls");
        if (exams.empty()) { std::cout << "No exams.\n"; return; }
        for (const auto& e : exams) {
            std::cout << "ID: " << e.getExamID() << " | " << e.getExamName() << " (" << e.getSubject() << ") Questions: " << e.getQuestions().size() << "\n";
        }
    }

    void adminAddUser() {
        system("cls");
        std::string username, pass;
        std::cout << "New username: ";
        std::cin >> username;
        if (findUserByName(username)) { std::cout << "User already exists.\n"; return; }
        std::cout << "Password: ";
        std::cin >> pass;

		userType t = (users.empty() ? admin : regular); // перший користувач - адмін
        users.emplace_back(username, pass, t);
        std::cout << "User created. Type: " << (t == admin ? "admin" : "regular") << "\n";
    }

    void adminAssignExam() {
        system("cls");
        adminViewExams();
        std::cout << "Enter exam ID to assign: ";
        std::size_t id; std::cin >> id;
        Exam* e = findExamByID(id);
        if (!e) { std::cout << "No such exam.\n"; return; }

        std::cout << "Enter username to assign to: ";
        std::string uname; std::cin >> uname;
        User* u = findUserByName(uname);
        if (!u) { std::cout << "No such user.\n"; return; }

        u->assignExam(id);
        std::cout << "Assigned exam " << id << " to " << uname << "\n";
    }

    void userViewAssignedExams(User& u) {
        system("cls");
        const auto& assigned = u.getAssignedExams();
        if (assigned.empty()) { std::cout << "No assigned exams.\n"; system("pause"); return; }
        for (auto id : assigned) {
            Exam* e = findExamByID(id);
            if (e) std::cout << "ID: " << id << " - " << e->getExamName() << "\n";
            else std::cout << "ID: " << id << " - (missing exam)\n";
        }
		
    }

    void takeOrResumeExam(User& u) {
        system("cls");
        userViewAssignedExams(u);
        std::cout << "Enter exam ID to take/resume: ";
        std::size_t id; std::cin >> id;
        Exam* exam = findExamByID(id);
        if (!exam) { std::cout << "No such exam.\n"; system("pause"); return; }

        ExamResult* result = u.findResult(id);
        if (!result) {
            std::cout << "You are not assigned this exam.\n";
            system("pause");
            return;
        }
        if (result->completed) {
            std::cout << "This exam is already completed. Score: " << result->correctAnswers << " / " << exam->getQuestions().size() << "\n";
            system("pause");
            return;
        }

        auto& questions = exam->getQuestions();
        for (std::size_t i = result->currentIndex; i < questions.size(); ++i) {
            const auto& question = questions[i];
            std::cout << "Question #" << i+1 << ": " << question.questionText << "\n";
            for (std::size_t option = 0; option < question.options.size(); ++option) {
                std::cout << "  " << option << ": " << question.options[option] << "\n";
            }
            std::cout << "Enter option index (or -1 to pause): ";
            int chosen; std::cin >> chosen;
            if (chosen == -1) {
                result->currentIndex = i;
                std::cout << "Progress saved. You can resume later.\n";
                return;
            }
            if (chosen < 0 || static_cast<std::size_t>(chosen) >= question.options.size()) {
                std::cout << "Invalid choice. Try again.\n";
                --i; // repeat same question
                continue;
            }
            if (question.isCorrectAnswer(static_cast<std::size_t>(chosen))) result->correctAnswers++;
            result->currentIndex = i + 1;
        }
        result->completed = true;
        std::cout << "Exam finished! Score: " << result->correctAnswers << " / " << questions.size() << "\n";
        system("pause");
    }

public:
    App() = default;

    void save(const std::string& usersFile = "users.dat", const std::string& examsFile = "exams.dat") {
        std::ofstream uf(usersFile);
        if (!uf) throw std::runtime_error("Cannot open users file for writing");
        uf << users.size() << "\n";
        for (const auto& u : users) uf << u;

        std::ofstream ef(examsFile);
        if (!ef) throw std::runtime_error("Cannot open exams file for writing");
        ef << exams.size() << "\n";
        for (const auto& e : exams) ef << e;

        ef << nextExamID << "\n";
        std::cout << "Saved.\n";
    }

    void load(const std::string& usersFile = "users.dat", const std::string& examsFile = "exams.dat") {
        std::ifstream uf(usersFile);
        if (uf) {
            std::size_t ucount; uf >> ucount;
            users.clear();
            users.resize(ucount);
            for (std::size_t i = 0; i < ucount; ++i) uf >> users[i];
        } else {
           
        }

        std::ifstream ef(examsFile);
        if (ef) {
            std::size_t ecount; ef >> ecount;
            exams.clear();
            exams.resize(ecount);
            for (std::size_t i = 0; i < ecount; ++i) ef >> exams[i];
          
            if (!(ef >> nextExamID)) {
                
                nextExamID = 1;
                for (const auto& e : exams) if (e.getExamID() >= nextExamID) nextExamID = e.getExamID() + 1;
            }
        }
        std::cout << "Loaded.\n";
    }
    void run() {
        load();
        while (true) {
            std::cout << "\n1) Register\n2) Login\n3) Save & Exit\nChoose: ";
            int choice; std::cin >> choice;
            if (choice == 1) {
                std::string username, pass;
                std::cout << "Username: "; std::cin >> username;
                if (findUserByName(username)) { std::cout << "Exists\n"; continue; }
                std::cout << "Password: "; std::cin >> pass;
                userType t = users.empty() ? admin : regular;
                users.emplace_back(username, pass, t);
                std::cout << "Registered. " << (t==admin ? "You are admin (first user)." : "") << "\n";
            } else if (choice == 2) {
                std::string username, pass;
                std::cout << "Username: "; std::cin >> username;
                std::cout << "Password: "; std::cin >> pass;
                std::size_t h = std::hash<std::string>{}(pass);
                User* u = findUserByName(username);
                if (!u || u->getPasswordHash() != h) { std::cout << "Bad credentials\n"; continue; }

                if (u->getType() == admin) {
					// меню адміна
                    while (true) {
                        
                        std::cout << "\n[ADMIN MENU]\n1) Add exam\n2) View exams\n3) Add user\n4) Assign exam to user\n5) Save & logout\nChoose: ";
                        int c; std::cin >> c;
                        if (c == 1) adminAddExam();
                        else if (c == 2) adminViewExams();
                        else if (c == 3) adminAddUser();
                        else if (c == 4) adminAssignExam();
                        else if (c == 5) { save(); break; }
                        else std::cout << "Invalid\n";
                    }
                } else {
					// меню користувача
                    while (true) {
                        system("cls");
                        std::cout << "\n[USER MENU]\n1) View assigned exams\n2) Take/resume exam\n3) Save & logout\nChoose: ";
                        int c; std::cin >> c;
                        if (c == 1) userViewAssignedExams(*u);
                        else if (c == 2) takeOrResumeExam(*u);
                        else if (c == 3) { save(); break; }
                        else std::cout << "Invalid\n";
                    }
                }
            } else if (choice == 3) {
                save();
                break;
            } else {
                std::cout << "Invalid\n";
            }
        }
    }
};
