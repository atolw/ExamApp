// ExamApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include"User.h"
#include"Exam.h"
#include"app.h"
int main()
{
    try {
        App app;
        app.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
    }
}