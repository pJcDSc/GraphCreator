#include <iostream>
#include <cstring>
#include <map>

using namespace std;

struct char_comparator {
  bool operator()(const char *a, const char *b) const {
    return strcmp(a, b) < 0;
  }
};

bool contains(map<char*, int, char_comparator>*, int);
bool parse(char*, int**&, map<char*, int, char_comparator>*, int&);
void addVertex(map<char*, int, char_comparator>*, int&);
void delVertex(int**&, map<char*, int, char_comparator>*, int&);
void addEdge(int**&, map<char*, int, char_comparator>*);
void delEdge(int**&, map<char*, int, char_comparator>*);
void getShortest(int**, map<char*, int, char_comparator>*);
void clear(int**&, map<char*, int, char_comparator>*, int&);
void print(int**, map<char*, int, char_comparator>*);
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
  while (running) {
    cout << "table: " << endl;
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 20; j++) {
    cout << table[i][j] << " ";
     }
     cout << endl;
     }

    cout << "nextVertInd: " << nextVertInd << endl;

    cout << "Enter a command" << endl;
    cin.get(input, 20);
    cin.clear();
    cin.ignore(999, '\n');

    running = parse(input, table, vertices, nextVertInd);
  }
  delete vertices;

  cout << "Thanks for using graph creator" << endl;
}

bool parse(char* input, int** &table, map<char*, int, char_comparator>* vertices, int &nextVertInd) {
  for (int i = 0; i < strlen(input); i++) {
    input[i] = toupper(input[i]);
  }

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

void addVertex(map<char*, int, char_comparator>* vertices, int &nextVertInd) {
  if (nextVertInd == 20) {
    cout << "Too many vertices, sorry." << endl;
    return; 
  }
  char* label = new char();
  cout << "Input vertex label (max 128 chars)" << endl;
  cin.get(label, 129);
  cin.clear();
  cin.ignore(999, '\n');
  map<char*, int>::iterator it; 
  for (it = vertices->begin(); it != vertices -> end(); it++) cout << it->first << ", " << it -> second << endl;
  if (vertices -> find(label) != vertices -> end()) {
    cout << "Sorry, vertex with label \"" << label << "\" already exists." << endl;
    return;
  }
  (*vertices)[label] = nextVertInd;
  nextVertInd = 0;
  while(contains(vertices, nextVertInd)) nextVertInd++; //Go to next available vertex index
  cout << "Vertex added." << endl;
}

void delVertex(int** &table, map<char*, int, char_comparator>* vertices, int &nextVertInd) {
  char* label = new char();
  cout << "Input vertex label to delete (max 128 chars)" << endl;
  cin.get(label, 129);
  cin.clear();
  cin.ignore();
  if (vertices -> find(label) == vertices -> end()) {
    cout << "Could not find vertex with label \"" << label << "\"." << endl;
    return;
  }
  int ref = (*vertices)[label];
  cout << "Ref: " << ref << endl;
  vertices -> erase(label);
  for (int i = 0; i < 20; i++) {
    table[ref][i] = -1;
    table[i][ref] = -1; //Delete all edges connecting to this vertex
  }
  nextVertInd = ref; 
  cout << "Vertex deleted." << endl;
}

void addEdge(int** &table, map<char*, int, char_comparator>* vertices) {
  char* v1 = new char();
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
    cout << "Please input two different vertices." << endl;
    return;
  }
  if (vertices->find(v1) == vertices->end()) {
    cout << "Could not find vertex with label \"" << v1 << "\"." << endl;
    return;
  }
  if (vertices->find(v2) == vertices->end()) {
    cout << "Could not find vertex with label \"" << v2 << "\"." << endl;
    return;
  }
  cout << "Input edge length (positive length only)" << endl;
  int el;
  cin >> el;
  cin.clear();
  cin.ignore(999, '\n');
  if (el <= 0) {
    cout << "Please input a positive edge length." << endl;
    return;
  }
  int ref1 = vertices->find(v1)->second;
  int ref2 = vertices->find(v2)->second;
  cout << "refs: " << ref1 << ", " << ref2 << endl;
  table[ref1][ref2] = el;
  cout << "Edge added" << endl;
  return;
}

void delEdge(int** &table, map<char*, int, char_comparator>* vertices) {

}

void getShortest(int** table, map<char*, int, char_comparator>* vertices) {

}

void clear(int**& table, map<char*, int, char_comparator>* vertices, int& nextVertInd) {
  cout << "Are you sure you want to clear the graph? (y/n)" << endl;
  char dec;
  cin >> dec;
  cin.ignore(999, '\n');
  if (dec == 'n' || dec == 'N') return;
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 20; j++) {
      table[i][j] = -1;
    }
  }
  vertices -> clear();
  nextVertInd = 0;
  cout << "Graph cleared" << endl;
}

void print(int** table, map<char*, int, char_comparator>* vertices) {

}

bool contains(map<char*, int, char_comparator>* vertices, int n) {
  map<char*, int, char_comparator>::iterator it;
  for (it = vertices -> begin(); it != vertices -> end(); ++it) {
    if (it->second == n) return true;
  }
  return false;
}
