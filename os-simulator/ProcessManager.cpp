﻿#include "pch.h"
#include "ProcessManager.h"

int ProcessManager::last_process_id_ = 0;

ProcessManager::ProcessManager()
{
	InitializeProcessPrinter();
	
	dummy_process_ = CreateProcess("dummy", "dummy", 0);
	SetProcessRunning(dummy_process_);
}

ProcessManager::~ProcessManager()
{
	for (auto&& process : processes_)
		delete process;
	processes_.clear();
}

void ProcessManager::RemoveFromVector(Process* process, std::vector<Process*>& vector) const
{
	vector.erase(std::remove(vector.begin(), vector.end(), process), vector.end());
}

void ProcessManager::SetProcessNew(Process* process)
{
	process->set_process_state(Process::New);
	new_processes_.push_back(process);

	std::sort(new_processes_.begin(), new_processes_.end(), [](Process* process1, Process* process2) { return process1->priority() > process2->priority(); });
}

Process* ProcessManager::CreateProcess(std::string process_name, std::string process_file, const int priority)
{
	const auto process = new Process(process_name, process_file, priority, ++last_process_id_);
	processes_.push_back(process);

	SetProcessNew(process);
	// TODO: Get process code from FAT
	// VirtualMemory::GetInstance().create_program(process, );
	
	try
	{
		RAM::GetInstance().add_to_RAM(process);
		SetProcessReady(process);
	}
	catch (std::exception & e)
	{
		std::cout << e.what();
	}

	CPU_M::GetInstance().scheduling();

	return process;
}

void ProcessManager::KillProcess(Process* process)
{
	if (process == dummy_process_)
		throw std::exception("Dummy process cannot be killed");
	
	switch (process->process_state())
	{
	case Process::New:
		RemoveFromVector(process, new_processes_); break;
	case Process::Waiting:
		RemoveFromVector(process, waiting_processes_); break;
	case Process::Ready:
		RemoveFromVector(process, ready_processes_); break;
	case Process::Running:
		running_process_ = dummy_process_; break;
	default: break;
	}

	RemoveFromVector(process, processes_);

	for(auto& process : new_processes_)
	{
		try
		{
			RAM::GetInstance().add_to_RAM(process);
			SetProcessReady(process);
		}
		catch (std::exception & e)
		{
		}
	}

	RAM::GetInstance().delete_from_RAM(process);
	VirtualMemory::GetInstance().delete_program(process);

	CPU_M::GetInstance().scheduling();

	delete process;
}

void ProcessManager::KillProcess(int process_id)
{
	KillProcess(GetProcess(process_id));
}

void ProcessManager::KillProcess(std::string process_name)
{
	KillProcess(GetProcess(process_name));
}

Process* ProcessManager::GetProcess(int process_id)
{
	auto process = std::find_if(processes_.begin(), processes_.end(), [process_id](Process* process) { return process->id() == process_id; });

	if (process == processes_.end())
		throw std::exception("Process doesn't exist"); // TODO: Create own exception
	
	return *process;
}

Process* ProcessManager::GetProcess(std::string process_name)
{
	auto process = std::find_if(processes_.begin(), processes_.end(), [process_name](Process* process) { return process->name() == process_name; });

	if (process == processes_.end())
		throw std::exception("Process doesn't exist"); // TODO: Create own exception

	return *process;
}

void ProcessManager::SetProcessWaiting(Process* process)
{
	switch (process->process_state())
	{
	case Process::New:
		RemoveFromVector(process, new_processes_); break;
	case Process::Waiting:
		return;
	case Process::Ready:
		RemoveFromVector(process, ready_processes_); break;
	case Process::Running:
		SetProcessRunning(dummy_process_); break;
	default: break;
	}

	process->set_process_state(Process::Waiting);
	waiting_processes_.push_back(process);
}

void ProcessManager::SetProcessWaiting(int process_id)
{
	SetProcessWaiting(GetProcess(process_id));
}

void ProcessManager::SetProcessWaiting(std::string process_name)
{
	SetProcessWaiting(GetProcess(process_name));
}

void ProcessManager::SetProcessReady(Process* process)
{
	switch (process->process_state())
	{
	case Process::New:
		RemoveFromVector(process, new_processes_); break;
	case Process::Waiting:
		RemoveFromVector(process, waiting_processes_); break;
	case Process::Ready:
		return;
	case Process::Running:
		SetProcessRunning(dummy_process_); break;
	default: break;
	}

	process->set_process_state(Process::Ready);
	ready_processes_.push_back(process);
}

void ProcessManager::SetProcessReady(int process_id)
{
	SetProcessReady(GetProcess(process_id));
}

void ProcessManager::SetProcessReady(std::string process_name)
{
	SetProcessReady(GetProcess(process_name));
}

void ProcessManager::SetProcessRunning(Process* process)
{
	switch(process->process_state())
	{
	case Process::New:
		RemoveFromVector(process, new_processes_); break;
	case Process::Waiting:
		RemoveFromVector(process, waiting_processes_); break;
	case Process::Ready:
		RemoveFromVector(process, ready_processes_); break;
	case Process::Running:
		return;
	default: break;
	}

	if(running_process_ != nullptr)
		SetProcessReady(running_process_);

	process->set_process_state(Process::Running);
	running_process_ = process;
}

void ProcessManager::SetProcessRunning(int process_id)
{
	SetProcessRunning(GetProcess(process_id));
}

void ProcessManager::SetProcessRunning(std::string process_name)
{
	SetProcessRunning(GetProcess(process_name));
}

void ProcessManager::InitializeProcessPrinter()
{
	process_printer_.AddColumn("ID", 2);
	process_printer_.AddColumn("Name", 10);
	process_printer_.AddColumn("File", 10);
	process_printer_.AddColumn("Priority", 2);
	process_printer_.AddColumn("State", 7);
}

void ProcessManager::PrintProcesses(std::vector<Process*> processes)
{
	process_printer_.PrintHeader();
	for (auto process : processes)
		process_printer_ << process->id() << process->name() << process->file_name() << process->priority() << process->process_state();
	process_printer_.PrintFooter();
}

void ProcessManager::PrintProcesses()
{
	PrintProcesses(processes_);
}

void ProcessManager::PrintProcess(Process* process) 
{
	process_printer_.PrintHeader();
	process_printer_ << process->id() << process->name() << process->file_name() << process->priority() << process->process_state();
	process_printer_.PrintFooter();
}

void ProcessManager::PrintProcess(int process_id) 
{
	PrintProcess(GetProcess(process_id));
}

void ProcessManager::PrintProcess(std::string process_name) 
{
	PrintProcess(GetProcess(process_name));
}

std::vector<Process*> ProcessManager::processes() const
{
	return processes_;
}

std::vector<Process*> ProcessManager::new_processes() const
{
	return new_processes_;
}

std::vector<Process*> ProcessManager::waiting_processes() const
{
	return waiting_processes_;
}

std::vector<Process*> ProcessManager::ready_processes() const
{
	return ready_processes_;
}

Process* ProcessManager::running_process() const
{
	return running_process_;
}
