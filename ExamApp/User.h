#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <cstddef>

/// <summary>
/// Структура для управління результатами тесту користувача. 
/// Зберігає ID тесту, поточний індекс питання та кількість 
/// правильних відповідей для відновлення прогресу користувача.
/// </summary>
struct ExamResult {
    std::size_t examID = 0;
    std::size_t currentIndex = 0;   // Індекс для продовження тесту
    std::size_t correctAnswers = 0;
    bool completed = false;
};

class App;

/// <summary>
/// Базовий клас для користувачів системи. 
/// Кожен користувач має своє ім'я, пароль, та призначені тести (засобами або адміністратора).
/// Цей клас є абстрактним і призначений для успадкування у підкласи, і кожен
/// підклас матиме свою реалізацію для виконання та управління завданнями.
/// </summary>
class User
{
    std::string username;
	std::size_t passwordHash{}; // Хеш паролю
	std::vector<std::size_t> assignedExams; // ID призначених тестів
	std::vector<ExamResult> results; // Результати від завдання прройденого тесту

public:
    User() = default;

    User(const std::string& username_, const std::string& password_)
		: username(username_), passwordHash(std::hash<std::string>{}(password_)), assignedExams(), results()
    {
        if (username.empty() || password_.empty())
        {
            throw std::invalid_argument("Username and password cannot be empty.");
        }
    }

    virtual ~User() = default;

    const std::string& getUsername() const { return username; }
    std::size_t getPasswordHash() const { return passwordHash; }
    

    void setUsername(const std::string& u) {
        if (u.empty()) throw std::invalid_argument("Username cannot be empty.");
        username = u;
    }
    void setPassword(const std::string& p) {
        if (p.empty()) throw std::invalid_argument("Password cannot be empty.");
        passwordHash = std::hash<std::string>{}(p);
    }
   

    void assignExam(std::size_t examID)
    {
        assignedExams.push_back(examID);
        ExamResult result;
        result.examID = examID;
        results.push_back(result);
    }

    const std::vector<std::size_t>& getAssignedExams() const { return assignedExams; }
    std::vector<ExamResult>& getResults() { return results; }
    const std::vector<ExamResult>& getResults() const { return results; }

	// Метод для пошуку результату тесту за ID тесту
    ExamResult* findResult(std::size_t examID)
    {
        for (auto& result : results) if (result.examID == examID) return &result;
        return nullptr;
    }

	// Оператори для серіалізації/десеріалізації до файлу
    friend std::ostream& operator<<(std::ostream& out, const User& u)
    {
        out << u.username << "\n";
        out << u.passwordHash << "\n";
      

        out << u.assignedExams.size() << "\n";
        for (auto id : u.assignedExams) out << id << "\n";

        out << u.results.size() << "\n";
        for (const auto& r : u.results)
        {
            out << r.examID << " " << r.currentIndex << " " << r.correctAnswers << " " << r.completed << "\n";
        }
        return out;
    }

    friend std::istream& operator>>(std::istream& in, User& u)
    {
        std::getline(in >> std::ws, u.username);
        in >> u.passwordHash;
        

        std::size_t assignedCount;
        in >> assignedCount;
        u.assignedExams.clear();
        for (std::size_t i = 0; i < assignedCount; ++i)
        {
            std::size_t id; in >> id; u.assignedExams.push_back(id);
        }

        std::size_t resultsCount;
        in >> resultsCount;
        u.results.clear();
        for (std::size_t i = 0; i < resultsCount; ++i)
        {
            ExamResult r;
            in >> r.examID >> r.currentIndex >> r.correctAnswers >> r.completed;
            u.results.push_back(r);
        }

        std::string musor; std::getline(in, musor);
        return in;
    }

	virtual void menu(App *app) = 0; // Чистий віртуальний метод для реалізації меню користувачем
};

class Admin : public User
{
public:
    using User::User;
    void menu(App *app) override;
};

class RegularUser : public User
{
public:
	using User::User;
	void menu(App* app) override;
};