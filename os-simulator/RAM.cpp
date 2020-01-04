#include "pch.h"
#include "RAM.h"

#include<fstream>

using namespace std;

int RAM::add_to_RAM(string filename) { //zamienic na process
	fstream file;
	string commands, help, line[128];
	int max_size = 0;
	int list_index = 0;



	file.open(filename);
	int length = 0, counter = 0;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, help);
			line[counter] = help;
			counter++;		 //string stream
			for (int i = 0; i <= help.length(); i++) //todo do uzgodnienia z FAT jak ma wygl�da� czytanie. 
			{
				if (help[i] != '\n')
					length++;


			}
			commands += help;
			commands += '\n';
		}


	}
	else {
		cout << "this file cannot be found. Try to change Filename parameter" << endl;
		return 2;
	}
	//cout << "all commands together:" << commands << "\n";

	bool empty = true;
	for (int i = 0; i < commands.length(); i++)
	{
		if (commands[i] != ' ' && commands[i] != '\n')
		{
			empty = false;
			break;
		}
	}

	if (empty)
	{
		cout << "\nthis file is empty\n" << endl;
		return 3;

	}
	bool find_space = false;
	try {


		if (free_space < length || free_space < 2)
		{
			cout << "\nthere is no enough space\n" << endl;
			return 1;
		}
		else {
			{
				if (Free_blocks_list.empty() == false) {
					for (auto e : Free_blocks_list)
					{
						if (e.size >= length)
							find_space = true;
					}
				}
				else {
					find_space = true;
				}
				if (find_space) {
					Free_blocks F_b; //to trzeba b�dzie chyba gdzie� wcze�niej zainicjowa� tbh.

					if (last == 0) {
						Free_blocks_list.pop_back();
						F_b.begining = length + 1;
						last = length + 1;
						F_b.end = 128;
						F_b.size = F_b.end - F_b.begining;
						Free_blocks_list.push_back(F_b);
					}
					else {
						list<Free_blocks>::iterator fbi;

						for (fbi = Free_blocks_list.begin(); fbi != Free_blocks_list.end(); fbi++) {


							if (max_size < fbi->size) {
								max_size = fbi->size;
								list_index = distance(Free_blocks_list.begin(), fbi);
							}

						}

						fbi = Free_blocks_list.begin();
						advance(fbi, list_index);
						fbi->biggest = true;

						for (fbi = Free_blocks_list.begin(); fbi != Free_blocks_list.end(); fbi++) {


							if (fbi->biggest == true && fbi->size >= length)
							{
								F_b.begining = fbi->begining + length;
								fbi->biggest = false;
								break;
							}
						}
						bool finder = false;
						for (auto a : RAM_processes_list) {
							if (a.start >= F_b.begining) {
								F_b.end = a.start - 1;
								finder = true;
								break;
							}
						}
						if (!finder) {
							F_b.end = 128;
						}
						F_b.size = F_b.end - F_b.begining;

						if (F_b.size > 0)
							Free_blocks_list.emplace_front(F_b);

						Free_blocks_list.erase(fbi);
					}

					//tutaj przekaza� trzeba 

					free_space -= length;
				}
			}
		}

	}
	catch (int) { return 1; };
	//Process process(); //tutaj trzeba przekazac id, wielkosc, komendy i takie tam do listy proces�w



	



	
}
