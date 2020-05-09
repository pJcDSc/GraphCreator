#include <iostream>
#include <cstring>
#include <map>
#include <vector>

using namespace std;

struct char_comparator {
  bool operator()(const char *a, const char *b) const {
    return strcmp(a, b) < 0;
  }
};

char* find(map<char*, int, char_comparator>*, int);
bool parse(char*, int**&, map<char*, int, char_comparator>*, int&);
void addVertex(map<char*, int, char_comparator>*, int&);
void delVertex(int**&, map<char*, int, char_comparator>*, int&);
void addEdge(int**&, map<char*, int, char_comparator>*);
void delEdge(int**&, map<char*, int, char_comparator>*);
void getShortest(int**, map<char*, int, char_comparator>*);
void clear(int**&, map<char*, int, char_comparator>*, int&);
void print(int**, map<char*, int, char_comparator>*);
int* dijkstra(int, int, int*&, bool*, int*, map<char*, int, char_comparator>*, int**);
void printHelp();

int main() {
  map<char*, int, char_comparator>* vertices = new map<char*, int, char_comparator>;
  int nextVertInd = 0;
  int** table = new int*[20]; //Create an array of int arrays
  for (int i = 0; i < 20; i++) {
    table[i] = new int[20];
    for (int j = 0; j < 20; j++) {
      table[i][j] = -1; //Negative weights aren't allowed in graph, so -1 will be default "no edge" state
    }
  }

  cout << "Welcome to graph creator" << endl;
  cout << "Type \"help\" for more commands" << endl;

  char* input = new char(); 
  bool running = true;
  while (running) {  //Main loop
    cout << "Enter a command" << endl;
    cin.get(input, 20);
    cin.clear();
    cin.ignore(999, '\n');

    running = parse(input, table, vertices, nextVertInd); //Parse input, if parse returns false exit while loop
  }
  delete vertices;
  for (int i = 0; i < 20; i++) {
    delete[] table[i]; //Free the memory
  }
  delete[] table;

  cout << "Thanks for using graph creator" << endl;
  return 0;
}

//Function to parse input and call other functions
bool parse(char* input, int** &table, map<char*, int, char_comparator>* vertices, int &nextVertInd) {
  for (int i = 0; i < strlen(input); i++) {
    input[i] = toupper(input[i]); //Capitalize input
  }

  //Parse using strcmp
  if (strcmp(input, "HELP") == 0) {
    printHelp();
  }
  else if (strcmp(input, "ADDV") == 0) {
    addVertex(vertices, nextVertInd);
  }
  else if (strcmp(input, "DELV") == 0) {
    delVertex(table, vertices, nextVertInd);
  }
  else if (strcmp(input, "ADDE") == 0) {
    addEdge(table, vertices);
  }
  else if (strcmp(input, "DELE") == 0) {
    delEdge(table, vertices);
  }
  else if (strcmp(input, "QUIT") == 0) {
    return false;
  }
  else if (strcmp(input, "SHORTEST") == 0) {
    getShortest(table, vertices);
  }
  else if (strcmp(input, "CLEAR") == 0) {
    clear(table, vertices, nextVertInd);
  }
  else if (strcmp(input, "PRINT") == 0) {
    print(table, vertices);
  }
  else {
    cout << "Sorry, command not recognized" << endl;
  }
  return true;
}

//Prints help
void printHelp() {
  cout << "Graph creator help:" << endl;
  cout << "addv: add a vertex (20 max)" << endl;
  cout << "delv: delete a vertex" << endl;
  cout << "adde: add an edge" << endl;
  cout << "dele: delete an edge" << endl;
  cout << "clear: clear all vertices and edges" << endl;
  cout << "print: print a list of all vertices and edges" << endl;
  cout << "shortest: find the shortest length path between two vertices" << endl;
  cout << "quit: quit the program" << endl;
}

//Add a vertex by adding a reference to the vertex map.
//nextVertInd refers to what index the next vertex should get. Since deletion can be out of order we search for the lowest possible nextVertInd every time we add
void addVertex(map<char*, int, char_comparator>* vertices, int &nextVertInd) {
  if (nextVertInd == 20) { //Table holds 20 total
    cout << "Too many vertices, sorry." << endl;
    return; 
  }
  char* label = new char(); //Get new label
  cout << "Input vertex label (max 128 chars)" << endl;
  cin.get(label, 129);
  cin.clear();
  cin.ignore(999, '\n');
  map<char*, int>::iterator it; 
  if (vertices -> find(label) != vertices -> end()) { //Check if label already exists
    cout << "Sorry, vertex with label \"" << label << "\" already exists." << endl;
    return;
  }
  (*vertices)[label] = nextVertInd; //Add to map
  nextVertInd = 0;
  while(find(vertices, nextVertInd) != NULL) nextVertInd++; //Go to next available vertex index
  cout << "Vertex added." << endl;
  return;
}

//Delete a vertex by removing it from the the map. Also remove all edges in the table connected to vertex
void delVertex(int** &table, map<char*, int, char_comparator>* vertices, int &nextVertInd) {
  char* label = new char(); //Get deletion label
  cout << "Input vertex label to delete (max 128 chars)" << endl;
  cin.get(label, 129);
  cin.clear();
  cin.ignore();
  if (vertices -> find(label) == vertices -> end()) { //Make sure it exists
    cout << "Could not find vertex with label \"" << label << "\"." << endl;
    return;
  }
  int ref = (*vertices)[label]; //Get index reference (to delete edges in table)
  vertices -> erase(label); //Delete from map
  for (int i = 0; i < 20; i++) {
    table[ref][i] = -1;
    table[i][ref] = -1; //Delete all edges connecting to this vertex
  }
  nextVertInd = ref; //Since we just cleared this vertex, its guaranteed to be free, so we can set nextVertInd to this one.
  cout << "Vertex deleted." << endl;
  return;
}

//Adds an edge to the matrix
void addEdge(int** &table, map<char*, int, char_comparator>* vertices) {
  char* v1 = new char(); //Get vertex labels
  char* v2 = new char();
  cout << "Input first vertex label" << endl;
  cin.get(v1, 129);
  cin.clear();
  cin.ignore(999, '\n');
  cout << "Input second vertex label" << endl;
  cin.get(v2, 129);
  cin.clear();
  cin.ignore(999, '\n');
  if (strcmp(v1, v2) == 0) {
    cout << "Please input two different vertices." << endl; //No self-edges (?) irrelevant anyways
    return;
  }
  if (vertices->find(v1) == vertices->end()) { //Make sure both vertices exist
    cout << "Could not find vertex with label \"" << v1 << "\"." << endl;
    return;
  }
  if (vertices->find(v2) == vertices->end()) {
    cout << "Could not find vertex with label \"" << v2 << "\"." << endl;
    return;
  }
  cout << "Input edge length (positive length only)" << endl; //Make sure edge length is positive. Dijkstra doesn't work with negative edge lengths, since negative cycles may exist
  int el;
  cin >> el;
  cin.clear();
  cin.ignore(999, '\n');
  if (el <= 0) {
    cout << "Please input a positive edge length." << endl;
    return;
  }
  int ref1 = vertices->find(v1)->second; //Add edge length to adjacency matrix
  int ref2 = vertices->find(v2)->second;
  table[ref1][ref2] = el;
  cout << "Edge added" << endl;
  return;
}

//Delete an edge from the matrix
void delEdge(int** &table, map<char*, int, char_comparator>* vertices) {
  char* v1 = new char(); //Get vertex labels
  char* v2 = new char();
  cout << "Input first vertex label" << endl;
  cin.get(v1, 129);
  cin.clear();
  cin.ignore(999, '\n');
  cout << "Input second vertex label" << endl;
  cin.get(v2, 129);
  cin.clear();
  cin.ignore(999, '\n');
  if (strcmp(v1, v2) == 0) { //Make sure they are different (technically not required but whatever)
    cout << "Please input two different vertices." << endl;
    return;
  }
  if (vertices->find(v1) == vertices->end()) { //Make sure both vertices exist
    cout << "Could not find vertex with label \"" << v1 << "\"." << endl;
    return;
  }
  if (vertices->find(v2) == vertices->end()) {
    cout << "Could not find vertex with label \"" << v2 << "\"." << endl;
    return;
  }
  int ref1 = vertices -> find(v1) -> second; //get current edge length
  int ref2 = vertices -> find(v2) -> second;
  if (table[ref1][ref2] == -1) {             //-1 = no edge here
    cout << "There is no edge there." << endl;
    return;
  }
  table[ref1][ref2] = -1; //Set to -1
  cout << "Edge deleted" << endl;
  return;
}

//Calls dijkstra
void getShortest(int** table, map<char*, int, char_comparator>* vertices) {
  char* v1 = new char(); //Get start and end point
  char* v2 = new char();
  cout << "Input starting vertex label" << endl;
  cin.get(v1, 129);
  cin.clear();
  cin.ignore(999, '\n');
  cout << "Input ending vertex label" << endl;
  cin.get(v2, 129);
  cin.clear();
  cin.ignore(999, '\n');
  if (strcmp(v1, v2) == 0) { //If start = end then distance is 0
    cout << "Vertices are the same, so shortest distance is 0" << endl;
    return;
  }
  if (vertices->find(v1) == vertices->end()) { //make sure both vertices exist
    cout << "Could not find vertex with label \"" << v1 << "\"." << endl;
    return;
  }
  if (vertices->find(v2) == vertices->end()) {
    cout << "Could not find vertex with label \"" << v2 << "\"." << endl;
    return;
  }
  int start = vertices -> find(v1) -> second;
  int end = vertices -> find(v2) -> second;

  //Dist is passed in by reference so needs to be initialized a bit differently
  int* dist = NULL; 
  dist = new int[20];
  bool visited[20];
  int parents[20];
  int* parent = dijkstra(start, end, dist, visited, parents, vertices, table); //Call dijkstra, get path
  if (parent == NULL) { //Only returns NULL when there is no path
    cout << "No path from \"" << v1 << "\" to \"" << v2 << "\" found." << endl;
    return;
  }
  vector<char*>* path = new vector<char*>();
  int x = end;
  while (x != start) { //Parent stores which node updated that node to get the shortest possible length, so tracing parent back to start will give shortest path
    path -> push_back(find(vertices, x));
    x = parent[x];
  }
  path -> push_back(find(vertices, start));
  cout << "Shortest path is: "; //Print all nodes in path
  bool isFirst =  true;
  vector<char*>::reverse_iterator it; //We added the nodes to the vector backwards, so we will use reverse iterator
  for (it = path -> rbegin(); it != path -> rend(); ++it) { //Use rbegin and rend with reverse_iterator to go through backwards
    if (!isFirst) cout << " -> ";
    isFirst = false;
    cout << *it;
  }
  cout << endl << "With length " << dist[end] << endl; //Dist stores distances to all nodes, so dist[end] stores the distance to the goal node
  return;
}

//O(n^2) implementation of dijkstras with naive searching because I don't want to use priority queue
int* dijkstra(int s, int e, int* &dist, bool* visited, int* parent, map<char*, int, char_comparator>* vmap, int** table) {
  if (s == e) return parent; //Reached the destination
  int next = -1;           //Stores index of next node to visit
  int low = 2147483647;    //Lowest distance seen so far
  bool finished = true;    //Check if algorithm is completed (no path to destination)
  for (int i = 0; i < 20; i++) {
    if (table[s][i] != -1 && !visited[i]) { //If there as an edge to this node and it is unvisited
      int newd = dist[s] + table[s][i];     //Calculate new distance to this node
      if (dist[i] == 0 || dist[i] > newd) {
	dist[i] = newd;                     //Update distance if needed
	parent[i] = s;                      //If distance is updated parent must also update
      }
      if (dist[i] < low) {                  //Check if this is new closest unvisited node
        next = i;
        low = dist[i];
      }
      finished = false;                     //If there are any nodes to update then this algorithm isn't done yet
    }
  }
  visited[s] = true;                        //Mark node as visited so we don't come here again
  if (finished) return NULL; //No more nodes to check, dead-end (can't get to target node)
  return dijkstra(next, e, dist, visited, parent, vmap, table); //Recurse on next node
}

//Clear the matrix and the vertex map
void clear(int**& table, map<char*, int, char_comparator>* vertices, int& nextVertInd) {
  cout << "Are you sure you want to clear the graph? (y/n)" << endl; //Confirmation
  char dec;
  cin >> dec;
  cin.ignore(999, '\n');
  if (dec == 'n' || dec == 'N') return;
  for (int i = 0; i < 20; i++) { //Delete matrix
    for (int j = 0; j < 20; j++) {
      table[i][j] = -1;
    }
  }
  vertices -> clear(); //Delete map
  nextVertInd = 0;
  cout << "Graph cleared" << endl;
}

//Prints all vertices and connections
void print(int** table, map<char*, int, char_comparator>* vertices) {
  cout << "Connections: " << endl;
  for (int i = 0; i < 20; i++) { //For each vertex
    char* l = find(vertices, i); //Make sure it exists in the vertex map
    if (l == NULL) continue;
    bool first = true;           //Print all connections
    cout << l << ": ";
    for (int j = 0; j < 20; j++) {
      if (table[i][j] == -1) continue;
      if (!first) cout << ", ";
      first = false;
      cout << find(vertices, j) << "(" << table[i][j] << ")";
    }
    cout << endl;
  }
}

//Returns the key corresponding to the value. In this case the name of the label with index n
char* find(map<char*, int, char_comparator>* vertices, int n) {
  map<char*, int, char_comparator>::iterator it;
  for (it = vertices -> begin(); it != vertices -> end(); ++it) {
    if (it->second == n) return it -> first; //Match
  }
  return NULL; // no match
}
