#include "pch.h"

#include <bitset>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "Disc.h"
#include "FAT.h"
#include "DirectoryFile.h"



class FileM : public Singleton<FileM>
{

public:


	friend class Singleton<FileM>;
	FileM();
	//Instrukcja Otworzenia Pliku
	std::string OpenFile(Process*pcb);
	//instrukcja Zamknięcia Pliku
	bool CloseFile(std::string ProcessName);
	//stworz plik
	bool CreateFile(const std::string& name);
	//usun plik
	bool DeleteFile(const std::string& name);
	//dodaj dodatkowa nazwe do pliku
	bool AddNewName(const std::string& name, const std::string& name2);
	//zmien nazwe pliku
	bool ReplaceNewName(const std::string& name, const std::string& name2);
	//zapisz tresc do pliku o podanej nazwie
	bool WriteFile(const std::string& name, string tresc);
	//Przesyla plik jako string
	std::string SendFile(const std::string& name);
	//Wyswietla plik za pomoca couta
	bool PrintFile(const std::string& name);
	//Szuka pliku true - istnieje false - nie istnieje
	bool InvestigateFile(const std::string& name);
	//Wyswietla pliki w katalogu
	bool ListDirectory() const;
	//Wyswietla tablice FAT
	bool ListFAT() const;
	//Wyswietla zajete miejsce na dysku
	bool Stats() const;
	int FindFreeDirectory();
	Drive dysk;
	FAT FileTable;
	DirectoryFile DIR;
	
	//Pobiera informacje z Pliku
	bool ExtractFile(const std::string& name, std::fstream tekst);

private:
	int FreeBlockCount;
	
	int FindFreeBlock();

	int FindFile(const std::string& name);

	bool Clearall();









};

