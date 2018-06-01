/*
Program: A list of students that you can add, delete, and print.
Author: Dieter Voegels
Date:10/13/17
*/
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ctime>

using namespace std;

//Student struct containing the first and last name, id, and gpa
struct Student{
  char* fName = new char[80];
  char* lName = new char[80];
  int id;
  float gpa;
  Student* next = NULL;
  Student* previous = NULL;
};

//initialize methods
bool hashTable(Student* table[], int &size, Student* student);
void rehash(Student* table[], int size, Student* student);
void Print(Student* table[], int size);
void Delete();

//main method
int main(){
  //initialize variables
  Student* *table = new Student*[100];
  int size = 100;
  for(int i = 0; i < 100; i++){
    table[i] = NULL;
  }
  char input[10];
  bool quit = false;
  bool rehash = false;
  //while loop
  while(!quit){
    //ask the user to input a command, and if they do not know the commands they can type help to get commands
    cout << "Type in a command. Type 'help' for commands" << endl;
    cin.getline(input, 10);
    //add a student
    if(strcmp(input, "add") == 0){
      //create a new student pointer pointing to a student struct
      Student* student = new Student;

      //have the user enter in the Students first and last name, ID, and GPA
      cout << "Enter in the first name of the student" << endl;
      cin >> student->fName;
      cout << "Enter in the last name of the student." << endl;
      cin >> student->lName;
      cout << "Enter in the student's id" << endl;
      cin >> student->id;
      cout << "Enter in the student's GPA" << endl;
      cin >> student->gpa;

      rehash = hashTable(table, size, student);
      
      cin.ignore(20, '\n');
    }
    //delete a student
    if (strcmp(input, "delete") == 0){
      cout << "what is the id of the student?" << endl;
      int id;
      char* ID;
      cin.getline(ID,7);
      id = atoi(ID);
      int index = id%size;
      delete table[index];
      table[index] = NULL;
    }
    //print out all students
    if(strcmp(input, "print") == 0){
      Print(table, size);
    }
    
    if(strcmp(input, "random") == 0){
      int inputNumber = 0;
      char* input = new char[200];
      char* fNames[200];
      char* lNames[200];
      bool empty = false;
      int index;
      cout << "How many students do you want to add?" << endl;
      cin.getline(input, 100);
      inputNumber = atoi(input);

      for(int i = 0; i < inputNumber; i++){
	ifstream file;
	file.open("FNames.txt");      
	if(file.is_open()){
	  for(int i = 0; file.eof() == false; i++){
	    file.getline(input, 200);
	    fNames[i] = new char[200];
	    strcpy(fNames[i], input);
	  }
	}
	else{
	  cout << "Could not find the file, try again." << endl;
	}
	
	ifstream file2("LNames.txt");
	if(file.is_open()){
	  for(int i = 0; file2.eof() == false; i++){
	    file2.getline(input, 200);
	    lNames[i] = new char[200];
	    strcpy(lNames[i], input);
	  }
	}
	else{
	  cout << "Could not find the file, try again." << endl;
	}
	
	Student* student = new Student;
	srand(time(NULL)/(i+1));
	index = rand()%100;
	student->fName = fNames[index];
	srand(time(NULL)/(i+1));
	index = rand()%100;
	student->lName = lNames[index];
	index = rand();
	student->gpa = index%4;
	student->id = index%1000000;
	//student->id = 123456;
	
	rehash = hashTable(table, size, student);
      }
      
      //print out all commands
      if(strcmp(input, "help") == 0){
	cout << "type 'add' to add a student to the list" << endl;
	cout << "type 'delete' to delete a student from the list" << endl;
	cout << "type 'print' to pring out the students" << endl;
	cout << "type 'random' to randomly generate students to add" << endl;
	cout << "type 'quit' to quit the application" << endl;
	cout << "type 'help' to open up view all commands" << endl;
      }
      
      //kill the program
      if(strcmp(input, "quit") == 0){
	quit = true;
      }
    }
    if(rehash == true){
      Student* oldTable[size];
      for(int i = 0; i < size; i++){
	oldTable[i] = table[i];
      }
      size = size*2;
      delete[] table;
      table = new Student*[size];
      for(int i = 0; i < size; i++){
	table[i] = NULL;
      }
      for(int i = 0; i < size/2; i++){
	if(oldTable[i] != NULL){
	  if(oldTable[i]->next != NULL){
	    if(oldTable[i]->next->next != NULL){
	      if(oldTable[i]->next->next->next != NULL){
		hashTable(table, size, oldTable[i]->next->next->next);
		oldTable[i]->next->next->next = NULL;
	      }
	      hashTable(table, size, oldTable[i]->next->next);
	      oldTable[i]->next->next = NULL;
	    }
	    hashTable(table, size, oldTable[i]->next);
	    oldTable[i]->next = NULL;
	  }
	  hashTable(table, size, oldTable[i]);
	}
      }
      rehash = false;
    }
  }
}

bool hashTable(Student* table[], int &size, Student* student){
  int index;
  index = student->id%size;
  if(table[index] == NULL){
    table[index] = student;
  }
  else{
    if(table[index]->next == NULL){
      table[index]->next = student;
      student->previous = table[index]->next;
    }
    else if(table[index]->next->next == NULL){
      table[index]->next->next = student;
      student->previous = table[index]->next->next;
    }
    else{
      table[index]->next->next->next = student;
      return true;
    }
  }
  int number = 0;
  for(int i = 0; i < size; i++){
    if(table[i] != NULL){
      number++;
    }
  }
  if(number >= size/2){
    return true;
  }
}

//print all of the students
void Print(Student* table[], int size){
  for(int i = 0; i < size; i++){
    if(table[i] != NULL){
      cout << table[i]->fName << endl;
      cout << table[i]->lName << endl;
      cout << table[i]->gpa << endl;
      cout << table[i]->id << endl;
      if(table[i]->next != NULL){
	cout << table[i]->next->fName << endl;
	cout << table[i]->next->lName << endl;
	cout << table[i]->next->gpa << endl;
	cout << table[i]->next->id << endl;
	if(table[i]->next->next != NULL){
	  cout << table[i]->next->next->fName << endl;
	  cout << table[i]->next->next->lName << endl;
	  cout << table[i]->next->next->gpa << endl;
	  cout << table[i]->next->next->id << endl;
	}
      }
    }
  }
}
