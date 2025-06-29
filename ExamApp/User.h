#pragma once
#include<iostream>
#include<string>
#include <functional> // for std::hash
#include <stdexcept> // for std::invalid_argument
#include <cstddef> // for size_t
#include<vector>

enum userType
{
	regular,
	admin
};

class User
{
	std::string username;
	std::size_t password; // hashed password
	std::vector<std::string> exams; // list of exams taken by the user

	userType type;

public:

	User(const std::string& username, const std::string& password, userType type = regular)
		: username(username), type(type)
	{
		if (username.empty() || password.empty())
		{
			throw std::invalid_argument("Username and password cannot be empty.");
		}
		this->password = std::hash<std::string>{}(password);
	}
	

	const std::string getUsername() const
	{
		return username;
	}
	const std::size_t getPassword() const
	{
		return password;
	}

	const userType& getType() const
	{
		return type;
	}

	void setUsername(const std::string& newUsername)
	{
		if (newUsername.empty())
		{
			throw std::invalid_argument("Username cannot be empty.");
		}
		username = newUsername;
	}

	void setPassword(const std::string& newPassword)
	{
		if (newPassword.empty())
		{
			throw std::invalid_argument("Password cannot be empty.");
		}
		password = std::hash<std::string>{}(newPassword);
	}
	void setType(userType newType)
	{
		type = newType;
	}

	friend std::ostream& operator<<(std::ostream& out, const User& user) 
	{
		out << user.username << " " << user.password << " " << user.type << " ";
		out << user.exams.size() << " ";
		for (const auto& exam : user.exams) {
			out << exam << " ";
		}
		out << "\n";
		return out;
	}

	friend std::istream& operator>>(std::istream& in, User& user)
	{
		int type;
		size_t numExams;
		in >> user.username >> user.password >> type >> numExams;
		user.type = static_cast<userType>(type);
		user.exams.resize(numExams);
		for (size_t i = 0; i < numExams; ++i)
		{
			in >> user.exams[i];
		}
		return in;
	}
};

class Admin : public User
{	
	public:
	Admin(const std::string& username, const std::string& password)
		: User(username, password, admin) {}
	
	void createExam();
	void deleteExam();
	void viewExams();
	void manageUsers();
	void viewResults();
};

