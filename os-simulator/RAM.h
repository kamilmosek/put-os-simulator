#pragma once
#include <iostream>
#include "Singleton.h"
#include "Process.h"
#include <conio.h>
#include <string>
#include <list>
#include <algorithm>
#include <fstream>
#include "ProcessManager.h"
#include "Segment.h"

using namespace std;
//TODO sprawdzi� co si� dzieje jak jest bardzo du�o ma�ych wolnych dziur i chce si� doda� proces
//TODO doda� wyj�tek albo co� jak Assembler albo kotkolwiek prosi o co� co jest mniejsze ni� 1 i wieksze niz 128
//TODO do dogadania z Assemblerem
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

class RAM : public Singleton<RAM> { //single tone do sprawdzenia
	friend class Singleton<RAM>;
 //private potem
	int memory_capacity = 256;
	int free_space = 256;
	list<Free_blocks> Free_blocks_list;
	list<RAM_process> RAM_processes_list;
	int command_length;
	string memory[256];
public:
	RAM() {
		Free_blocks F_b;
		F_b.begining = 1;
		F_b.end = 256;
		F_b.size = F_b.end - F_b.begining;
		Free_blocks_list.push_back(F_b);
	}
private:
	int last = 0;
	void merge_RAM();
public:
	int add_to_RAM(Process* process);

	//doda� wska�nik na proces jako argument albo ciagnac po id pobra� kod programu z modu�u FAT //zamienic na proces
	void delete_from_RAM(Process* process);  //doda� wska�nik na proces jako argument
	void show_RAM(); //wyswietlenie zawartosci ram
	string char_RAM(int place);
	string read_RAM(Process* process, int counter); 
	void modify_RAM(int position, int byte);



	
	

};
