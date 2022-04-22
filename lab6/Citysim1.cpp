/* Caroline Rinks
 * Lab 6 (Citysim)
 * CS302 Spring 2021
 *
 *
*/

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
using namespace std;

class city {
	public:
		friend istream & operator>>(istream &, city &); 
		friend ostream & operator<<(ostream &, city &);
		
		void read_cityinfo(vector<city> &);
		void write_cityinfo(vector<city> &);

		string get_name()		{ return name; }
		string get_type()		{ return type; }
		int get_zone()			{ return zone; }
		float get_latitude()	{ return latitude; }		// (latitude * (M_PI/180));
		float get_longitude()	{ return longitude; }		// (longitude * (M_PI/180));
		int get_population()	{ return population; }

	private:
		string name;
		string type;
		int zone;
		float latitude;
		float longitude;
		int population;
};

istream & operator>>(istream &in, city &r) { 
	string line;
	getline(in, line);
	
	if (!line.size()) {
		getline(in, line);
	}

	while (line[0] == '#'){
		getline(in, line);
	}

	istringstream sin;
	sin.str(line);
	
	sin >> r.zone >> r.name >> r.type >> r.latitude >> r.longitude >> r.population;

	return in;
}

ostream & operator<<(ostream &out, city &r) {
	out << left << setw(18) << r.name
		<< setw(12) << r.type
		<< setw(2) << r.zone 
		<< setw(10) << right << r.population
		<< setw(8) << fixed << setprecision(2) << (r.latitude * (180/M_PI))
		<< setw(8) << (r.longitude * (180/M_PI));
	
	return out; 
}

class costtable {
	public:
		costtable(vector<city> &);

		float operator()(int, int, int);

	private:
		vector<float> distance_table;
		vector<float> time_table;
};

/* @brief: Constructor for costtable, populates a distance table and time table 
 * @param graph: A graph of cities
*/
costtable::costtable(vector<city> &graph) {
	double distance, long_diff;
	float time;

	for (int i = 0; i < (int)graph.size(); i++) {
		for (int j = 0; j <= i; j++) {
			if (j == i) {
				distance_table.push_back(0);
				time_table.push_back(0);
			}
			else {
				/* Calculate great circle distance between 2 cities to the nearest 10.0 mile 
				 * and add to distance table */
				distance = sin(graph[i].get_latitude()) * sin(graph[j].get_latitude());
				long_diff = abs(graph[i].get_longitude() - graph[j].get_longitude()); 
				distance += cos(graph[i].get_latitude()) * cos(graph[j].get_latitude()) * cos(long_diff);
				distance = acos(distance) * 3982.0;
				distance = 10.0 * round((distance/10.0)); 
				
				distance_table.push_back((float)distance);

				/* Calculate air or ground travel time and add to time table */
				if (graph[i].get_type() == "GATEWAY" || graph[j].get_type() == "GATEWAY") {
					time = (float)distance / 570.0;
					time_table.push_back(time);
				}
				else {
					time = (float)distance / 60.0;	
					time_table.push_back(time);
				}
			}
		}
	}
}

float costtable::operator()(int mode, int i, int j) {
	if (j > i) {	//swap indices
		int tmp = i;
		i = j;
		j = tmp;
	}
	
	if (mode == 0) {
		return distance_table[((i*(i+1))/2)+j];
	}
	else if (mode == 1) {
		return time_table[((i*(i+1))/2)+j];
	}
	else {
		cerr << "invalid mode given: 0|1\n";
		exit(EXIT_FAILURE);
	}
}

/* @brief: Reads city information from a file and stores it in a graph of city objects.
 * @param graph: A graph of cities */
void city::read_cityinfo(vector<city> &graph) { 
	ifstream fin;
	city obj;
	
	fin.open("city_list.txt");
	if(!fin.is_open()){
		cerr << "Could not open file.\n";
		exit(EXIT_FAILURE);
	}

	while (fin >> obj){
		obj.latitude = (obj.latitude * (M_PI/180));
		obj.longitude = (obj.longitude * (M_PI/180));
		graph.push_back(obj);	
	}
	fin.close();
}

/* @brief: Writes city information to a file.
 * @param graph: A graph that holds city information. */
void city::write_cityinfo(vector<city> &graph) {
	ofstream fout;
	fout.open("city_info.txt");
	
	fout << "CITY INFO (N=34):\n" << '\n';
	for (int i = 0; i < (int)graph.size(); i++){
		fout << setw(3) << right << i << ' ' << graph[i] << '\n';
	}
	fout.close();
}

/* @brief: Writes a city's distance table to a file
 *
void write_distancetable(vector<city> &graph, costtable &c) { 
	ofstream fout;
	fout.open("city_distancetable.txt");
	
	fout << "DISTANCE TABLE:\n" << '\n';
	
	for (int i = 1; i < (int)graph.size(); i++){
		for (int j = 0; j < i; j++) {
			fout << setw(3) << right << i << ' ' 
				 << setfill('.') << setw(38) << left
				 << graph[i].get_name() + " to " + graph[j].get_name() + ' '
				 << setfill(' ') << setw(7) << right << fixed << setprecision(1) 
				 << c(0, i, j) << " miles\n";
		}
		fout << '\n';
	}
	
	fout.close();
}

void write_timetable(vector<city> &graph, costtable &c) { 
	ofstream fout;
	fout.open("city_timetable.txt");

	fout << "TIME TABLE:\n" << '\n';

	for (int i = 1; i < (int)graph.size(); i++){
		for (int j = 0; j < i; j++){
			fout << setw(3) << right << i << ' '
				 << setfill('.') << setw(38) << left
				 << graph[i].get_name() + " to " + graph[j].get_name() + ' '
				 << setfill(' ') << setw(5) << right << fixed << setprecision(1)
				 << c(1, i, j) << " hours\n";
		}
		fout << '\n';
	}

	fout.close();
}

int main(int argc, char *argv[]) {
	
	if (argc != 2){
		cerr << "usage: ./Citysim1 -distance|time|graphinfo\n";
		return -1;
	}

	string mode = argv[1];
	city obj;
	vector<city> graph;

	obj.read_cityinfo(graph);

	costtable c(graph);

	if (mode == "-distance"){
		// Part2 of lab
	}
	else if (mode == "-time"){
		// Part2 of lab
	}
	else if (mode == "-graphinfo"){
		obj.write_cityinfo(graph);
		
		write_distancetable(graph, c);
		write_timetable(graph, c);

		//V2: write_citygraph()

		return 0;
	}
	else {
		cerr << "usage: ./Citysim1 -distance|time|graphinfo\n";
		return -1;
	}

	//V2: while (not done) {
			//enter> from, to cities
			//dijkstra_route(costmode,from,to) 
		//}

	return 0;
}
