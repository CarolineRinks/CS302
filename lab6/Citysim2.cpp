// Caroline Rinks
// Lab 6.2 (Citysim)

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <map>
#include <stack>
#include <cfloat>
#include <algorithm>
#include <limits>
using namespace std;

typedef enum { WHITE, BLACK } vcolor_t;    

class city{
	public:
		friend istream & operator>>(istream &, city &); 
		friend ostream & operator<<(ostream &, city &);
		
		void read_cityinfo(vector<city> &, map<string, int> &);
		void write_cityinfo(vector<city> &);

		string get_name()		{ return name; }
		string get_type()		{ return type; }
		int get_zone()			{ return zone; }
		float get_latitude()	{ return latitude; }
		float get_longitude()	{ return longitude; }
		int get_population()	{ return population; }

	private:
		string name;
		string type;
		int zone;
		float latitude;		// in radians
		float longitude;	// in radians
		int population;
};

istream & operator>>(istream &in, city &r) { 
	string line;
	getline(in, line);
	
	if (!line.size()){
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

costtable::costtable(vector<city> &cities) {
	double distance, long_diff;
	float time;
	

	/* Calculate great circle distance between each city and add to distance table,
	 * then calculate air or ground travel time to each city and add to time table */
	for (int i = 0; i < (int)cities.size(); i++) {
		for (int j = 0; j <= i; j++){
			if (j == i) { 
				distance_table.push_back(0);
				time_table.push_back(0);
			}
			else{
				distance = sin(cities[i].get_latitude()) * sin(cities[j].get_latitude());
				long_diff = abs(cities[i].get_longitude() - cities[j].get_longitude()); 
				distance += cos(cities[i].get_latitude()) * cos(cities[j].get_latitude()) * cos(long_diff);
				distance = acos(distance) * 3982.0;
				distance = 10.0 * round((distance/10.0)); 
				
				distance_table.push_back((float)distance);

				if (cities[i].get_type() == "GATEWAY" || cities[j].get_type() == "GATEWAY"){
					time = (float)distance / 570.0;
					time_table.push_back(time);
				}
				else{
					time = (float)distance / 60.0;	
					time_table.push_back(time);
				}
			}
		}
	}
}

float costtable::operator()(int mode, int i, int j){
	if (j > i) {	//swap indices
		int tmp = i;
		i = j;
		j = tmp;
	}
	
	if (mode == 0) {
		return distance_table[((i*(i+1))/2)+j];		// converts i,j coordinate into i coordinate
	}
	else if (mode == 1) {
		return time_table[((i*(i+1))/2)+j];
	}
	else{
		cerr << "invalid mode given: 0|1\n";
		exit(EXIT_FAILURE);
	}
}

/* @brief: Reads city information from a file and stores it.
 * @param cities:
 * @param name_map:
*/
void city::read_cityinfo(vector<city> &cities, map<string, int> &name_map) { 
	ifstream fin;
	city obj;
	int i = 0;	// index of city in cities vector
	
	fin.open("city_list.txt");
	if (!fin.is_open()){
		cerr << "Could not open file.\n";
		exit(EXIT_FAILURE);
	}

	while (fin >> obj){
		obj.latitude = (obj.latitude * (M_PI/180));		//convert to radians
		obj.longitude = (obj.longitude * (M_PI/180));	//convert to radians
		
		cities.push_back(obj);							//add city to list
		
		name_map[obj.name] = i;							//map city name to its index in list
		++i;
	}
	fin.close();
}

/* @brief: Writes city information to a file.
 * @param cities: Holds city information. 
*/
void city::write_cityinfo(vector<city> &cities) {
	ofstream fout;
	fout.open("city_info.txt");
	
	fout << "CITY INFO (N=34):\n" << '\n';
	for (int i = 0; i < (int)cities.size(); i++){
		fout << setw(3) << right << i << ' ' << cities[i] << '\n';
	}
	fout.close();
}

/* @brief: Writes distance table to a file.
 * @param cities: Holds city information
 * @param c: Cost table that holds distance and travel times between cities 
*/
void write_distancetable(vector<city> &cities, costtable &c) { 
	ofstream fout;
	fout.open("city_distancetable.txt");
	
	fout << "DISTANCE TABLE:\n" << '\n';
	
	for (int i = 1; i < (int)cities.size(); i++){
		for (int j = 0; j < i; j++) {
			fout << setw(3) << right << i << ' ' 
				 << setfill('.') << setw(38) << left
				 << cities[i].get_name() + " to " + cities[j].get_name() + ' '
				 << setfill(' ') << setw(7) << right << fixed << setprecision(1) 
				 << c(0, i, j) << " miles\n";
		}
		fout << '\n';
	}	
	fout.close();
}

/* @brief: Writes time table to a file.
 * @param cities: Holds city information
 * @param c: Cost table that holds distance and travel times between cities 
*/
void write_timetable(vector<city> &cities, costtable &c) { 
	ofstream fout;
	fout.open("city_timetable.txt");

	fout << "TIME TABLE:\n" << '\n';

	for (int i = 1; i < (int)cities.size(); i++){
		for (int j = 0; j < i; j++){
			fout << setw(3) << right << i << ' '
				 << setfill('.') << setw(38) << left
				 << cities[i].get_name() + " to " + cities[j].get_name() + ' '
				 << setfill(' ') << setw(5) << right << fixed << setprecision(1)
				 << c(1, i, j) << " hours\n";
		}
		fout << '\n';
	}
	fout.close();
}

void create_citygraph(vector<city> &cities, vector< vector<int> > &graph, costtable &c) { 
	
	vector<int> g_type;		// gateway cities
	vector<int> r_type;		// regional cities

	int N = (int)cities.size();
	graph.resize(N);

	/* Generate 2 lists: one that holds indices of Regional cities and one for Gateway cities */
	for (int i = 0; i < N; i++) {
		if (cities[i].get_type() == "REGIONAL") {
			r_type.push_back(i);
		}
		else if (cities[i].get_type() == "GATEWAY") {
			g_type.push_back(i);
		}
		else {
			cerr << "Invalid type given.\n";
			exit(EXIT_FAILURE);
		}
	}

	vector<int>::iterator i2;
	vector<int>::iterator i1 = r_type.begin();
	float nearest;									// distance of nearest city
	int nearest_i;									// index of nearest city
	bool diff_zone;									// indicates if city should be connected (based on rules)

	while (i1 != r_type.end()) {
		nearest = numeric_limits<float>::max();
		diff_zone = false;
		i2 = i1 + 1;

		/* Connect all regional cities in the same zone */
		while (i2 != r_type.end() && cities[*i1].get_zone() == cities[*i2].get_zone()) {
			graph[*i1].push_back(*i2);
			graph[*i2].push_back(*i1);
			++i2;
		}
		
		//Connect nearest gateway city in same zone:
		i2 = g_type.begin();
		int z = cities[*i1].get_zone();
		
		while (cities[*i2].get_zone() != z) {
			++i2;	//advance pointer to zone z
		}
		while (i2 != g_type.end() && cities[*i2].get_zone() == z) {
			//find nearest city in zone z
			if (c(0, *i1, *i2) < nearest) {
				nearest = c(0, *i1, *i2);	//update nearest city
				nearest_i = *i2;			//update index of nearest city
				diff_zone = true;
			}
			++i2;	//next city in same zone
		}
		if (diff_zone == true){
			graph[*i1].push_back(nearest_i);
			graph[nearest_i].push_back(*i1);
		}
		
		++i1;	//next city 
	}

	i1 = g_type.begin();
	//Iterate thru all Gateway cities
	while (i1 != g_type.end()){
		i2 = i1 + 1;
		
		//loop connects all gateway cities in same zone
		while (i2 != g_type.end() && cities[*i1].get_zone() == cities[*i2].get_zone()){
			graph[*i1].push_back(*i2);
			graph[*i2].push_back(*i1);
			++i2;
		}

		i2 = i1 + 1;
		//Find nearest gateway city in each zone and connect to city
		for (int z = 1; z <= 6; z++){	//repeat for each zone (total of 6 times)
			i2 = g_type.begin();
			diff_zone = false;
			nearest = 5201.0;

			if (z == cities[*i1].get_zone()){
				continue;	//skip current zone
			}

			while (i2 != g_type.end() && cities[*i2].get_zone() != z){
				++i2;	//advance iterator to zone z
			}
			while (i2 != g_type.end() && cities[*i2].get_zone() == z){
				//find nearest city in zone z
				if (c(0, *i1, *i2) < nearest){
					nearest = c(0, *i1, *i2);	//update nearest city
					nearest_i = *i2;			//update index of nearest city
					diff_zone = true;
				}
				++i2; //next city in same zone
			}
			if (diff_zone == true){
				graph[*i1].push_back(nearest_i);
				graph[nearest_i].push_back(*i1);
			}
		}
		++i1;	//next city
	}

	//sort edges and remove duplicates
	for (int i = 0; i < N; i++){
		sort(graph[i].begin(), graph[i].end());
		vector<int>::iterator it = unique(graph[i].begin(), graph[i].end());
		graph[i].resize(it - graph[i].begin());
	}
}

void write_citygraph(vector<city> &cities, vector < vector<int> > &graph, costtable &c) { 
	
	vector<int>::iterator it;
	
	ofstream fout;
	fout.open("mycity_graph.txt");
	
	fout << "CITY GRAPH:\n";

	for (int i = 0; i < (int)cities.size(); i++){
		fout << '\n' << setw(3) << right << i << " " << cities[i].get_name() << '\n';
		
		it = graph[i].begin();
		while (it != graph[i].end()){
			fout << setw(6) << right << *it << " "
				 << setw(19) << left << cities[*it].get_name() << " "
				 << fixed << setprecision(1) << setw(6) << right
				 << c(0, i, *it) << " miles"
				 << setw(6) << c(1, i, *it) << " hours\n";
			++it;
		}
	}
	fout.close();
}

void dijkstra_route(const int MODE, int source, int sink, costtable &c, vector<city> &cities, vector <vector<int>> &graph) {

	vector<vcolor_t> vcolor;	//indicates visited (black) and unvisited (white) vertices
	vector<int> vlink;			//holds each vertex's previous vertex
	vector<float> vdist;		//holds shortest dist/time to each vertex: vector[0]=miles, vector[1]=hours
	
	vcolor.assign(cities.size(), WHITE);						//all vertices start as white (unvisited) 
	vdist.assign(cities.size(), numeric_limits<float>::max());  //dist/time to each vertex starts as infinity
	vdist[source] = 0.0;										//dist/time from source to itself is 0

	vlink.assign(cities.size(), -1);
	vlink[source] = source;  
	
	while (1) {    
		int next_i = -1;    
		float mindist = numeric_limits<float>::max();    
		
		for (int i = 0; i < (int)vcolor.size(); i++) {      
			//find unvisited vertex with smallest known distance from start vertex
			if (vcolor[i] == WHITE && vdist[i] < mindist) {        
				next_i = i;        
				mindist = vdist[i];      
			}    
		}    
		
		int i = next_i;		//current vertex    
		
		if (i == -1){       //stop when all vertices have been visited 
			return;  
		}
		
		vcolor[i] = BLACK;	//mark current vertex as visited  
		
		if (i == sink) {	//break if current vertex is final destination  
			break;
		}

		for (int k = 0; k < (int)graph[i].size(); k++) {	//examine edges of vertex i     
			int j = graph[i][k];		//grab an edge      
			float wij = c(MODE, i, j);  //calculate weight of edge (distance/time)	

			if (vcolor[j] == WHITE) {	//if neighboring vertex hasn't been visited        
				//if calculated distance is less than known distance,
				//update the shortest distance
				if (vdist[i] + wij < vdist[j]) {          
					vdist[j] = vdist[i] + wij;          
					vlink[j] = i;	//update previous vertex       
				}      
			}
		}
	}

	//use a stack to generate vertex path
	stack<int> S;
	for (int i = sink; i != source; i = vlink[i]){
		S.push(i);
	}

	//Print path
	cout << setw(7) << right << "0.0" << " miles " 
		 << setw(5) << "0.0" << " hours : " 
		 << setw(2) << source << " " << setw(17) << left << cities[source].get_name()
		 << '\n';

	float cumulative_time = 0.0;
	float cumulative_dist = 0.0;
	int prev_i = source;

	while (!S.empty()){
		int i = S.top();
		S.pop();
		cumulative_time += c(1, prev_i, i);
		cumulative_dist += c(0, prev_i, i);

		cout << fixed << setprecision(1) << setw(7) << right << cumulative_dist << " miles "
			 << setw(5) << cumulative_time << " hours : "
			 << setw(2) << i << " "
			 << setw(18) << left << cities[i].get_name() << " "
			 << setw(6) << right << c(0, prev_i, i) << " miles "
			 << setw(5) << c(1, prev_i, i) << " hours\n";
		
		prev_i = i;
	}
	cout << '\n';
}

int main(int argc, char *argv[]) {
	//commandline option decoding
	if (argc != 2) {
		cerr << "usage: ./Citysim2 -distance|time|graphinfo\n";
		return -1;
	}
	string mode = argv[1];

	vector<city> cities;			//vertex list
	vector< vector<int> > graph;	//edge matrix (sparse)
	map<string, int> name_map;		//name-to-index map

	city obj;
	obj.read_cityinfo(cities, name_map);	//read data

	costtable c(cities);					//set up cost tables

	create_citygraph(cities, graph, c);		//create graph

	if (mode == "-distance"){
		string source, sink;
		map<string,int>::iterator i1, i2;

		cout << "Enter> ";
		while (cin >> source >> sink){	
			//check if source and sink are in map
			i1 = name_map.find(source);
			i2 = name_map.find(sink);
			if (i1 != name_map.end() && i2 != name_map.end()){
				dijkstra_route(0, name_map[source], name_map[sink], c, cities, graph);
			}
			else{
				cout << '\n' << "No path found between " << source << " and " << sink << '\n';
				return -1;
			}
			cout << "Enter> ";
		}
	}
	else if (mode == "-time"){
		string source, sink;
		map<string,int>::iterator i1, i2;

		cout << "Enter> ";
		while (cin >> source >> sink){
			//check if source and sink are in map
			i1 = name_map.find(source);
			i2 = name_map.find(sink);
			if (i1 != name_map.end() && i2 != name_map.end()){
				dijkstra_route(1, name_map[source], name_map[sink], c, cities, graph);
			}
			else{
				cout << '\n' << "No path found between " << source << " and " << sink << '\n';
				return -1;
			}
			cout << "Enter> ";
		}
	}
	else if (mode == "-graphinfo"){
		obj.write_cityinfo(cities);
		
		write_distancetable(cities, c);
		write_timetable(cities, c);
		write_citygraph(cities, graph, c);

		return 0;
	}
	else {
		cerr << "usage: ./Citysim1 -distance|time|graphinfo\n";
		return -1;
	}

	return 0;
}
