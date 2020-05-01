#include <iostream>
#include <cstring>
#include <map>

using namespace std;

bool parse(char*, int**&, map<char*, int>*, int&);
void addVertex(map<char*, int>*, int&);
void delVertex(map<char*, int>*, int&);
void addEdge(int**&, map<char*, int>*, int);
void delEdge(int**&, map<char*, int>*, int);
void getShortest(int**, map<char*, int>*);
void printHelp();

int main() {
  map<char*, int>* vertices = new map<char*, int>;
  int numVerts = 0;
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
	//cout << "yEET";
      }
      cout << endl;
    }

    cout << "numverts: " << numVerts << endl;
    
    cin.get(input, 20);
    cin.clear();
    cin.ignore(999, '\n');

    running = parse(input, table, vertices, numVerts);
  }
  delete vertices;

  cout << "Thanks for using graph creator" << endl;
}

bool parse(char* input, int** &table, map<char*, int>* vertices, int &numVerts) {
  for (int i = 0; i < strlen(input); i++) {
    input[i] = toupper(input[i]);
  }

  if (strcmp(input, "HELP") == 0) {
    printHelp();
  }
  else if (strcmp(input, "ADDV") == 0) {
    addVertex(vertices, numVerts);
  }
  else if (strcmp(input, "DELV") == 0) {
    delVertex(vertices, numVerts);
  }
  else if (strcmp(input, "ADDE") == 0) {
    addEdge(table, vertices, numVerts);
  }
  else if (strcmp(input, "DELE") == 0) {
    delEdge(table, vertices, numVerts);
  }
  else if (strcmp(input, "QUIT") == 0) {
    return false;
  }
  else if (strcmp(input, "SHORTEST") == 0) {
    getShortest(table, vertices);
  }
  else {
    cout << "Sorry, command not recognized" << endl;
  }
  return true;
}

void printHelp() {
  cout << "Graph creator help:" << endl;
  cout << "addv: add a vertex" << endl;
  cout << "delv: delete a vertex" << endl;
  cout << "adde: add an edge" << endl;
  cout << "dele: delete an edge" << endl;
  cout << "quit: quit the program" << endl;
  cout << "shortest: find the shortest length path between two vertices" << endl;
}

void addVertex(map<char*, int>* vertices, int &numVerts) {
  
}

void delVertex(map<char*, int>* vertices, int &numVerts) {

}

void addEdge(int** &table, map<char*, int>* vertices, int numVerts) {

}

void delEdge(int** &table, map<char*, int>* vertices, int numVerts) {

}

void getShortest(int** table, map<char*, int>* vertices) {

}