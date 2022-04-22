/* Caroline Rinks
 * Lab #9
 *
 * Implementation of person, faculty, and student class methods
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <algorithm>
#include <map>
using namespace std;

#include "Person.h"

person::person() { 
	name = ""; 
	type = UNKNOWN;
	category = UNKNOWN;
}

person::~person() {}

faculty::faculty(string &_name, type_enum &_type, type_enum &_category, vector<string> &list){
	name = _name;
	type = _type;
	category = _category;
	
	add_course(list);
}

faculty::~faculty() {}

void faculty::add_course(vector<string> &courses){
	courselist = courses;
	sort(courselist.begin(), courselist.end());		//sort list of courses
}

void faculty::print_personinfo(){
	cout << right << setw(11) << "Name:";
	if (type == FACULTY){
		cout << " Dr.";
	}
	cout << ' ' << name << '\n';

	int i = category;
	switch(i){
		case ASST_PROF:
			cout << setw(11) << "Category:" << ' ' << "Assistant Professor" << '\n';
			break;

		case ASSOC_PROF:
			cout << setw(11) << "Category:" << ' ' << "Associate Professor" << '\n';
			break;

		case FULL_PROF:
			cout << setw(11) << "Category:" << ' ' << "Full Professor" << '\n';
			break;
	}
}

void faculty::print_courseinfo(){
	for (int i = 0; i < (int)courselist.size(); i++){
		cout << right << setw(11) << "Course:" << ' ' << courselist[i] << '\n';
	}
	cout << '\n';
}

student::student(string &_name, type_enum &_type, type_enum &_category, vector<string> &list){
	name = _name;
	type = _type;
	category = _category;
	
	add_course(list);
}

student::~student() {}

void student::add_course(vector<string> &courses){
	for (int i = 0; i < (int)courses.size()-1; i += 2){
		courselist.push_back(courses[i]);			//add course name to course list
		course_grade[courses[i]] = courses[i+1];	//map course name to course grade
	}
	sort(courselist.begin(), courselist.end());		//sort list of courses
}

void student::print_personinfo(){
	cout << right << setw(11) << "Name:";
	if (type == FACULTY){
		cout << " Dr.";
	}
	cout << ' ' << name << '\n';

	int i = category;
	switch(i){
		case FRESHMAN:
			cout << setw(11) << "Category:" << ' ' << "Freshman" << '\n';
			break;

		case SOPHOMORE:
			cout << setw(11) << "Category:" << ' ' << "Sophomore" << '\n';
			break;

		case JUNIOR:
			cout << setw(11) << "Category:" << ' ' << "Junior" << '\n';
			break;

		case SENIOR:
			cout << setw(11) << "Category:" << ' ' << "Senior" << '\n';
			break;
	}
}

void student::print_courseinfo(){
	float avg;
	float sum = stof(course_grade[courselist[0]]);
	
	/* Print 1st Course */
	cout << right
		 << setw(11) << "Course:" << ' ' 
		 << left << setw(25) << courselist[0] << ' '
		 << fixed << setprecision(2) << sum << ' ' << sum << '\n';
	
	/* Print remaining Courses */
	for (float i = 1; i < (float)courselist.size(); i++){
		sum += stof(course_grade[courselist[i]]);
		avg = sum / (i+1);	 
		
		cout << right << setw(11) << "Course:" << ' '
			 << left << setw(25) << courselist[i] << ' ' 
			 << fixed << setprecision(2) << stof(course_grade[courselist[i]]) << ' ' 
			 << avg << '\n';;
	}
	cout << '\n';
}

