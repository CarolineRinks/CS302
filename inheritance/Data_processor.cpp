/* Caroline Rinks
 * Lab #9
 *
 * Driver Code
 * 
 * This program reads an xml file of student and faculty data, stores it in a list with 
 * the help of a Person class, and inherited student and faculty classes, and then sorts
 * the list by implementing smart pointers.
*/

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
using namespace std;

#include "Person.h"
#include "Sptrsort.h"

int main(int argc, char *argv[]) {
	
	if (argc != 2) {
		cerr << "usage: ./Data_processor -person|student|faculty < file.xml\n";
		return -1;
	}
	
	string _person = "-person";
	string _student = "-student";
	string _faculty = "-faculty";
	
	int print_mode;

	if (argv[1] == _person){
		print_mode = 0;
	}
	else if (argv[1] == _faculty){
		print_mode = 1;
	}
	else if (argv[1] == _student){
		print_mode = 2;
	}
	else{
		cerr << "usage: ./Data_processor -person|student|faculty < file.xml\n";
		return -1;
	}
	
	person * n_person;
	vector<person *> person_list;

	type_enum Type = UNKNOWN;
	type_enum Category = UNKNOWN;

	string Name = "";			// empty string to build name
	string cat = "";			// empty string to build category 
	vector<string> Courses;		// list of courses

	int line = 1;
	size_t iL, iR;
	string input;

	while (getline(cin, input)) {
		++line;

		if ((iL = input.find('<')) == string::npos) {
			continue;
		} 
		else if (input.compare(iL,9,"<faculty>") == 0) {
			Type = FACULTY;		// set type 
		} 
		else if (input.compare(iL,10,"</faculty>") == 0) {
			
			/* Add faculty person to database */
			if (strcmp(cat.c_str(), "Assistant Professor") == 0){
				Category = ASST_PROF;
			}
			else if (strcmp(cat.c_str(), "Associate Professor") == 0){
				Category = ASSOC_PROF;
			}
			else{	
				Category = FULL_PROF;
			}

			n_person = new faculty(Name, Type, Category, Courses);
			person_list.push_back(n_person);

			Name.clear();		//reset name
			Type = UNKNOWN;		//reset type
			Category = UNKNOWN;	//reset subtype
			cat.clear();		//reset category
			Courses.clear();	//reset course list

			continue;
		} 
		else if (input.compare(iL,9,"<student>") == 0) {
			Type = STUDENT;		// set type
		} 
		else if (input.compare(iL,10,"</student>") == 0) {
			
			/* Add student person to database */
			if (strcmp(cat.c_str(), "Freshman") == 0){
				Category = FRESHMAN;
			}
			else if (strcmp(cat.c_str(), "Sophomore") == 0){
				Category = SOPHOMORE;
			}
			else if (strcmp(cat.c_str(), "Junior") == 0){
				Category = JUNIOR;		
			}
			else{
				Category = SENIOR;
			}

			n_person = new student(Name, Type, Category, Courses);
			person_list.push_back(n_person);

			Name.clear();		 //reset name
			Type = UNKNOWN;		 //reset type
			Category = UNKNOWN;  //reset subtype
			cat.clear();		 //reset category
			Courses.clear();	 //reset course list

			continue;
		} 
		else if (input.compare(iL,5,"<name") == 0) {
			int tmp = iL;
			iL = input.find("=\"", iL);
			iR = input.find("\"/>", iL+2);

			/* Extract name (using iL, iR) */
			iL += 2;
			while (iL != iR){
				Name += input[iL];	// Build name string
				++iL;
			}
			iL = tmp;		// reset iL to original position b4 next iteration
		} 
		else if (input.compare(iL,10,"<category=") == 0) {
			int tmp = iL;
			iL = input.find("=\"", iL);
			iR = input.find("\"/>", iL+2);

			/* Extracting Category (using iL, iR) */
			iL += 2;
			while (iL != iR){
				cat += input[iL];	// Build category string
				++iL;
			}
			iL = tmp;		// reset iL to original position b4 next iteration
		} 
		else if (input.compare(iL,7,"<course") == 0) {
			iL = input.find("=\"", iL);
			iR = input.find("\"", iL+2);

			string course = "";

			/* Build course name string (using iL, iR) */
			iL += 2;
			while (iL != iR){
				course += input[iL];
				++iL;
			}
			Courses.push_back(course);	// Add course name to course list

			/* Extract Course (using iL, iR) */
			iL = iR;
			if (Type == FACULTY) {
				iR = input.find("/>", iL+1);
			} 
			else if (Type == STUDENT) {
				/* Extract grade (using iL, iR) - for STUDENT type only */
				iL = input.find("gp=\"", iL);
				iR = input.find("\"/>", iL+4);
				
				string gp = "";
				iL += 4;
				
				while (iL != iR){
					gp += input[iL];
					++iL;
				
				}
				Courses.push_back(gp);
				gp.clear();
			}
			course.clear();
		}
	}

	sptrsort(person_list);		// Sort list using smart pointers
	
	/* Print list to stdout according to mode specified by user */
	switch(print_mode){
		case 0:
			/* Printing for -person mode */
			for (int i = 0; i < (int)person_list.size(); i++){
				if (person_list[i]->get_type() == FACULTY){
					cout << person_list[i];
				}
			}
			for (int i = 0; i < (int)person_list.size(); i++){
				if (person_list[i]->get_type() == STUDENT){
					cout << person_list[i];
				}
			}
			break;

		case 1: 
			/* Printing for -faculty mode */
			for (int i = 0; i < (int)person_list.size(); i++){
				if (person_list[i]->get_category() == ASST_PROF){
					cout << person_list[i];
				}
			}
			for (int i = 0; i < (int)person_list.size(); i++){
				if (person_list[i]->get_category() == ASSOC_PROF){
					cout << person_list[i];
				}
			}
			for (int i = 0; i < (int)person_list.size(); i++){
				if (person_list[i]->get_category() == FULL_PROF){
					cout << person_list[i];
				}
			}
			break;

		case 2: 
			/* Print for -student mode */
			for (int i = 0; i < (int)person_list.size(); i++){
				if (person_list[i]->get_category() == FRESHMAN){
					cout << person_list[i];
				}
			}
			for (int i = 0; i < (int)person_list.size(); i++){
				if (person_list[i]->get_category() == SOPHOMORE){
					cout << person_list[i];
				}
			}
			for (int i = 0; i < (int)person_list.size(); i++){
				if (person_list[i]->get_category() == JUNIOR){
					cout << person_list[i];
				}
			}
			for (int i = 0; i < (int)person_list.size(); i++){
				if (person_list[i]->get_category() == SENIOR){
					cout << person_list[i];
				}
			}
			break;
	}

	/* Free memory */
	person *cpy;
	for (int i = (int)person_list.size(); i >= 0; i--){
		cpy = person_list[i];
		person_list.pop_back();

		delete cpy;
	}
}
