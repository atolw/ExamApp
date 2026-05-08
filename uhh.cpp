#include "User.h"
#include "app.h"

void Admin::menu(App* app)
{
	while (true)
	{
		std::cout << "\n[ADMIN MENU]\n1) Add exam\n2) View exams\n3) Add user\n4) Assign exam to user\n5) Save & logout\nChoose: ";
		int c; std::cin >> c;
		if (c == 1) app->adminAddExam();
		else if (c == 2) app->adminViewExams();
		else if (c == 3) app->adminAddUser();
		else if (c == 4) app->adminAssignExam();
		else if (c == 5) { app->save(); break; }
		else std::cout << "Invalid\n";
	}
}

void RegularUser::menu(App* app)
{
	while (true)
	{
		std::cout << "\n[USER MENU]\n1) View assigned exams\n2) Take/resume exam\n3) Save & logout\nChoose: ";
		int c; std::cin >> c;
		if (c == 1) app->userViewAssignedExams(this);
		else if (c == 2) app->takeOrResumeExam(this);
		else if (c == 3) { app->save(); break; }
		else std::cout << "Invalid\n";
	}
}