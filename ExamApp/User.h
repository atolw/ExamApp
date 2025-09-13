#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <cstddef>

enum userType
{
    regular,
    admin
};
/// <summary>
/// Структура для збереження результатів екзамену користувача. 
/// Потрібна для відстеження прогресу по кожному екзамену та для 
/// можливості продовження незавершених екзаменів.
/// </summary>
struct ExamResult {
    std::size_t examID = 0;
    std::size_t currentIndex = 0;   // індекс для збереження прогресу
    std::size_t correctAnswers = 0;
    bool completed = false;
};

/// <summary>
/// Базовий клас для користувача системи екзаменів. 
/// Зберігає інформацію про ім'я користувача, хеш пароля, тип користувача (регулярний або адміністратор).
/// Має методи для встановлення і отримання цих даних, а також для
/// призначення екзаменів та збереження результатів.
/// </summary>
class User
{
    std::string username;
	std::size_t passwordHash{}; // хеш пароля
    userType type;
	std::vector<std::size_t> assignedExams; // ліст ID призначених екзаменів
	std::vector<ExamResult> results; // прогрес по кожному призначеному екзамену

public:
    User() = default;

    User(const std::string& username_, const std::string& password_, userType type_ = regular)
        : username(username_), type(type_)
    {
        if (username.empty() || password_.empty())
        {
            throw std::invalid_argument("Username and password cannot be empty.");
        }
        passwordHash = std::hash<std::string>{}(password_);
    }

    const std::string& getUsername() const { return username; }
    std::size_t getPasswordHash() const { return passwordHash; }
    userType getType() const { return type; }

    void setUsername(const std::string& u) {
        if (u.empty()) throw std::invalid_argument("Username cannot be empty.");
        username = u;
    }
    void setPassword(const std::string& p) {
        if (p.empty()) throw std::invalid_argument("Password cannot be empty.");
        passwordHash = std::hash<std::string>{}(p);
    }
    void setType(userType t) { type = t; }

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

	// метод для пошуку результату екзамену по ID екзамену
    ExamResult* findResult(std::size_t examID)
    {
        for (auto& result : results) if (result.examID == examID) return &result;
        return nullptr;
    }

	// оператори для збереження/завантаження користувача у файл
    friend std::ostream& operator<<(std::ostream& out, const User& u)
    {
        out << u.username << "\n";
        out << u.passwordHash << "\n";
        out << static_cast<int>(u.type) << "\n";

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
        int t; in >> t; u.type = static_cast<userType>(t);

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
};
