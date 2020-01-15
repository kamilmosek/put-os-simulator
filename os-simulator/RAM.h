#pragma once
#include<iostream>
#include "Process.h"
#include<conio.h>
#include<string>
#include<list>
#include<algorithm>
#include<fstream>
#include "ProcessManager.h"

using namespace std;

struct Free_blocks {
	int size;
	int begining;
	int end = size + begining;
	bool biggest = false;
};

struct RAM_process
{
	int id;
	int size;
	int start;
	string commands;
	int MemoryPointer = 0;

};

class RAM { //single tone do sprawdzenia
public: //private potem
	int memory_capacity = 128;
	int free_space = 128;
	list<Free_blocks> Free_blocks_list;
	list<RAM_process> RAM_processes_list;
	int command_length;
	//string memory[128];

	RAM() {
		Free_blocks F_b;
		F_b.begining = 1;
		F_b.end = 128;
		F_b.size = F_b.end - F_b.begining;
		Free_blocks_list.push_back(F_b);
	}
private:
	int last = 0;
public:
	int add_to_RAM(Process* process);  //doda� wska�nik na proces jako argument albo ciagnac po id pobra� kod programu z modu�u FAT //zamienic na proces
	void delete_from_RAM(Process* process);  //doda� wska�nik na proces jako argument
	void show_RAM(); //wyswietlenie zawartosci ram
	char char_RAM(Process* process, int place);
	string read_RAM(Process* process, int counter);
	void merge_RAM();

};
