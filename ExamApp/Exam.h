#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <cstddef>
#include <iostream>

/// <summary>
/// Структура для збереження питання екзамену. 
/// </summary>
struct Question
{
    std::string questionText;
    std::vector<std::string> options;
    std::size_t correctAnswerIndex{};

    Question() = default;

    Question(const std::string& text, const std::vector<std::string>& opts, std::size_t correctIndex)
        : questionText(text), options(opts), correctAnswerIndex(correctIndex)
    {
        if (text.empty() || opts.empty() || correctIndex >= opts.size()) {
            throw std::invalid_argument("Invalid question parameters.");
        }
    }

    bool isCorrectAnswer(std::size_t answerIndex) const {
        if (answerIndex >= options.size()) throw std::out_of_range("Answer index out of range.");
        return answerIndex == correctAnswerIndex;
    }

    friend std::ostream& operator<<(std::ostream& out, const Question& q) {
        out << q.questionText << "\n";
        out << q.options.size() << "\n";
        for (const auto& option : q.options) out << option << "\n";
        out << q.correctAnswerIndex << "\n";
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Question& q) {
        std::getline(in >> std::ws, q.questionText);
        std::size_t optCount; in >> optCount;
        in.ignore();
        q.options.resize(optCount);
        for (std::size_t i = 0; i < optCount; ++i) {
            std::getline(in, q.options[i]);
        }
        in >> q.correctAnswerIndex;
        std::string musor; std::getline(in, musor);
        if (q.correctAnswerIndex >= q.options.size()) throw std::invalid_argument("Correct index out of range.");
        return in;
    }
};
/// <summary>
/// Клас для збереження екзамену. 
/// </summary>
class Exam
{
    std::string examName;
    std::size_t examID{ 0 };
    std::string subject;
    std::vector<Question> questions;
public:
    Exam() = default;
    Exam(const std::string& name, std::size_t id, const std::string& subj)
        : examName(name), examID(id), subject(subj)
    {
        if (name.empty() || id == 0 || subj.empty()) throw std::invalid_argument("Invalid exam parameters.");
    }

    const std::string& getExamName() const { return examName; }
    std::size_t getExamID() const { return examID; }
    const std::string& getSubject() const { return subject; }
    const std::vector<Question>& getQuestions() const { return questions; }

    void setExamName(const std::string& n) { if (n.empty()) throw std::invalid_argument("Name empty"); examName = n; }
    void setSubject(const std::string& s) { if (s.empty()) throw std::invalid_argument("Subject empty"); subject = s; }
    void setExamID(std::size_t id) { examID = id; }

    void addQuestion(const Question& q) { questions.push_back(q); }
    void removeQuestion(std::size_t idx) {
        if (idx >= questions.size()) throw std::out_of_range("Question index out of range");
        questions.erase(questions.begin() + idx);
    }

    friend std::ostream& operator<<(std::ostream& out, const Exam& e) {
        out << e.examName << "\n";
        out << e.examID << "\n";
        out << e.subject << "\n";
        out << e.questions.size() << "\n";
        for (const auto& question : e.questions) out << question;
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Exam& e) {
        std::getline(in >> std::ws, e.examName);
        in >> e.examID;
        in.ignore();
        std::getline(in, e.subject);
        std::size_t qcount; in >> qcount;
        in.ignore();
        e.questions.clear();
        e.questions.resize(qcount);
        for (std::size_t i = 0; i < qcount; ++i) in >> e.questions[i];
        return in;
    }
};
