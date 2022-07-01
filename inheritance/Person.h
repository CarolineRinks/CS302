/* Caroline Rinks
 * Lab #9
 *
 * Person class, student class, and faculty class header file 
*/

#ifndef __PERSON_H__ 
#define __PERSON_H__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <map>
using namespace std;

enum type_enum { UNKNOWN=-1, FACULTY=0,  ASST_PROF=1, ASSOC_PROF=2, FULL_PROF=3,
                 STUDENT=10, FRESHMAN=11, SOPHOMORE=12, JUNIOR=13, SENIOR=14 };

class person { 
	public:
		person();
		virtual ~person();

		bool operator<(const person &rhs) const	{ return name < rhs.name; }
		
		friend ostream & operator<<(ostream &out, person *p) { 
			p->print_personinfo(); 
			p->print_courseinfo(); 
			return out;
		}
		
		type_enum get_type()	 { return type; }
		type_enum get_category() { return category; }

	protected:
		virtual void add_course(vector<string> &)=0;
		virtual void print_personinfo()=0;
		virtual void print_courseinfo()=0;

		string name;
		type_enum type;
		type_enum category;
};

class faculty : public person {
	public:
		faculty(string &, type_enum &, type_enum &, vector<string> &);
		~faculty();

		void add_course(vector<string> &);

	private:
		void print_personinfo();
		void print_courseinfo();
		
		vector<string> courselist;
};

class student : public person {
	public:
		student(string &, type_enum &, type_enum &, vector<string> &);
		~student();

		void add_course(vector<string> &);

	private:
		void print_personinfo();
		void print_courseinfo();	
		
		vector<string> courselist;
		map<string, string> course_grade;
};

#endif

