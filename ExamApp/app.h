#pragma once

#include <string>
#include <stdexcept>
#include <cstddef>
#include <vector>
#include <iostream>
#include <functional> // for std::hash
#include<fstream>

#include"Exam.h"
#include"User.h"

class app
{

	std::vector<Exam> exams; // list of exams in the app
	std::vector<User> users; // list of users in the app
	bool isAdmin; // flag to check if the user is an admin
protected:
	void addExam(const Exam& exam)
		{
			exams.push_back(exam);
		}
		void addUser(const User& user)
		{
			users.push_back(user);
		}
		void setAdmin(bool adminStatus)
		{
			isAdmin = adminStatus;
		}
		bool getAdminStatus() const
		{
			return isAdmin;
		}
		const std::vector<Exam>& getExams() const
		{
			return exams;
		}
		const std::vector<User>& getUsers() const
		{
			return users;
		}

public:

	app() : isAdmin(false) {}
	void save()
	{
		std::ofstream userFile("users.dat");
		if (!userFile) {
			throw std::runtime_error("Could not open users file for writing");
		}
		userFile << users.size() << "\n";
		for (const auto& user : users)
		{
			userFile << user;
		}

		std::ofstream examFile("exams.dat");
		if (!examFile) {
			throw std::runtime_error("Could not open exams file for writing");
		}

		examFile << exams.size() << "\n";
		for (const auto& exam : exams) {
			examFile << exam;
		}

		std::cout << "Data saved successfully!\n";
	}
	void menu(bool isAdmin)
	{
		system("cls"); // clear the console
		std::cout << "Welcome to the Exam App!\n";
		if (isAdmin)
		{
			std::cout << "1. Add Exam\n";
			std::cout << "2. Add User\n";
			std::cout << "3. View Exams\n";
			std::cout << "4. View Users\n";
			std::cout << "0. Exit\n";
		}
		else
		{
			std::cout << "1. View Exams\n";
			std::cout << "2. Take Exam\n";
			std::cout << "0. Exit\n";
		}
		std::cout << "Please select an option: ";
		int choice;
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			if (isAdmin)
			{
				// Logic to add exam
			}
			else
			{
				// Logic to view exams
			}
			break;
		case 2:
			if (isAdmin)
			{
				// Logic to add user
			}
			else
			{
				// Logic to take exam
			}
			break;
		case 3:
			if (isAdmin)
			{
				// Logic to view exams or users
			}
			else
			{
				exit(0);
			}
			break;
		case 4:
			if (isAdmin)
			{
				exit(0);
			}
			else
			{
				exit(0);
			}
		case 0:
			save();
			break;
		default:
			std::cout << "Invalid choice, please try again.\n";
			menu(isAdmin);
			break;
		}
		
	}
	void login()
	{
		std::string username, password;
		std::size_t hashedPassword;
		std::cout << "Enter username: ";
		std::cin >> username;
		std::cout << "Enter password: ";
		std::cin >> password;

		if (username.empty() || password.empty())
		{
			throw std::invalid_argument("Username and password cannot be empty.");
		}

		hashedPassword = std::hash<std::string>{}(password);

		bool found = false;
		for (const auto& user : users)
		{
			if (user.getUsername() == username && user.getPassword() == hashedPassword)
			{
				std::cout << "Login successful!\n";
				setAdmin(user.getType() == admin);
				menu(isAdmin);
				found = true;
				break;
			}
		}

		if (!found)
		{
			std::cout << "Invalid username or password. Please try again.\n";
			login();
		}
	}
	
	void registerUser()
	{
		std::string username, password;
		std::cout << "Enter username: ";
		std::cin >> username;
		std::cout << "Enter password: ";
		std::cin >> password;
		userType type = regular; // default user type
		if (username.empty() || password.empty())
		{
			throw std::invalid_argument("Username and password cannot be empty.");
		}
		if (users.empty())
		{
			 type = admin;
		}
		User newUser(username, password, type);
		addUser(newUser);
		std::cout << "User registered successfully!\n";
		system("cls"); // clear the console
	}
};