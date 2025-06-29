#pragma once

#include <string>
#include <stdexcept> 
#include <cstddef> 
#include<vector>
struct Question
{
	std::string questionText;
	std::vector<std::string> options; // multiple choice options
	std::size_t correctAnswerIndex; // index of the correct answer in options
	Question(const std::string& text, const std::vector<std::string>& opts, std::size_t correctIndex)
		: questionText(text), options(opts), correctAnswerIndex(correctIndex)
	{
		if (text.empty() || opts.empty() || correctIndex >= opts.size())
		{
			throw std::invalid_argument("Invalid question parameters.");
		}
	}

	friend std::ostream& operator<<(std::ostream& out, const Question& question)
	{
		out << question.questionText << "\n"
			<< question.options.size() << "\n";
		for (const auto& option : question.options)
		{
			out << option << "\n";
		}
		out << question.correctAnswerIndex << "\n";
		return out;
	}

	friend std::istream& operator<<(std::istream& in, Question& question)
	{
		std::getline(in >> std::ws, question.questionText);

		std::size_t optionsCount;
		in >> optionsCount;

		in.ignore();
		question.options.resize(optionsCount);
		for (std::size_t i = 0; i < optionsCount; ++i)
		{
			std::getline(in, question.options[i]);
		}

		in >> question.correctAnswerIndex;

		if (question.correctAnswerIndex >= question.options.size())
		{
			throw std::invalid_argument("Correct answer index out of range.");
		}
		return in;
	
	}

	void addOption(const std::string& option)
	{
		if (option.empty())
		{
			throw std::invalid_argument("Option cannot be empty.");
		}
		options.push_back(option);
	}
	bool isCorrectAnswer(std::size_t answerIndex) const
	{
		if (answerIndex >= options.size())
		{
			throw std::out_of_range("Answer index out of range.");
		}
		return answerIndex == correctAnswerIndex;
	}
};


class Exam
{
	std::string examName;
	std::size_t examID; // unique identifier for the exam
	std::string subject;
	std::vector<Question> questions; // list of questions in the exam
protected:
	bool isCompleted;

public:
	Exam(const std::string& name, std::size_t id, const std::string& subj)
		: examName(name), examID(id), subject(subj), isCompleted(false)
	{
		if (name.empty() || id == 0 || subj.empty())
		{
			throw std::invalid_argument("Invalid exam parameters.");
		}
	}

	friend std::ostream& operator<<(std::ostream& out, const Exam& exam)
	{
		out << exam.examName << "\n";
		out << exam.examID << "\n"; 
		out << exam.subject << "\n";
		out << exam.isCompleted << "\n";
		out << exam.questions.size() << "\n";
		for (const auto& question : exam.questions) {
			out << question;
		}
		return out;
	}

	friend std::istream& operator>>(std::istream& in, Exam& exam)
	{
		std::getline(in >> std::ws, exam.examName);
		in >> exam.examID >> std::ws;
		std::getline(in, exam.subject);
		in >> exam.isCompleted;

		std::size_t questionsCount;
		in >> questionsCount;

		in.ignore();
		exam.questions.resize(questionsCount);
		for (std::size_t i = 0; i < questionsCount; ++i)
		{
			in >> exam.questions[i];
		}
		return in;

	}


	void addQuestion(const Question& question)
	{
		questions.push_back(question);
	}
	const std::string& getExamName() const
	{
		return examName;
	}
	std::size_t getExamID() const
	{
		return examID;
	}
	const std::string& getSubject() const
	{
		return subject;
	}
	const std::vector<Question>& getQuestions() const
	{
		return questions;
	}
	void setExamName(const std::string& name)
	{
		if (name.empty())
		{
			throw std::invalid_argument("Exam name cannot be empty.");
		}
		examName = name;
	}
	void setisCompleted(bool completed)
	{
		isCompleted = completed;
	}
	void markCompleted()
	{
		this->isCompleted = true;
	}
	void runExam() 
	{
		std::size_t correctAnswers = 0;
		if (questions.empty())
		{
			throw std::runtime_error("No questions available for the exam.");
		}
		std::cout << "Running exam: " << examName << " on subject: " << subject << "\n";
		for (const auto& question : questions)
		{
			if (isCompleted)
			{
				throw std::runtime_error("Exam has already been completed.");
			}
			
			std::cout << question.questionText << "\n";
			for (std::size_t i = 0; i < question.options.size(); ++i)
			{
				std::cout << i << ": " << question.options[i] << "\n";
			}
			std::cout << "Please select the correct answer (0-" << question.options.size() - 1 << "): ";
			int answerIndex;
			std::cin >> answerIndex;
			if(answerIndex < 0 || answerIndex >= question.options.size())
			{
				throw std::invalid_argument("Invalid answer index.");
			}
			if (question.isCorrectAnswer(answerIndex))
			{
				correctAnswers++;
			}
			std::cout << "Answer has been selected.\n";
		}
		isCompleted = true;
		std::cout << "Exam completed. Total correct answers: " << correctAnswers << "\n";
	
	}
	void removeQuestion(std::size_t index)
	{
		if (index >= questions.size())
		{
			throw std::out_of_range("Question index out of range.");
		}
		questions.erase(questions.begin() + index);
	}
	
};

