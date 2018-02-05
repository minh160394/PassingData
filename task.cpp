#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "util.h"
#include "task.h"
using namespace std;

Task::Task()
{
}

Task::Task(vector<string> &row) : taskSlots("1")
	{
		switch (row.size())
		{
		case 4:
			if (validTaskName(row[3]))
				taskFail = row[3];
			else
				throw string("Expected a fail task name --- found '") + row[3] + "'";
		case 3:
			if (validTaskName(row[2]))
				taskPass = row[2];
			else
				throw string("Expected a pass task name --- found '") + row[2] + "'";
		case 2:
			if (validTaskSlots(row[1]))
				taskSlots = row[1];
			else
				throw string("Expected a slots --- found '") + row[1] + "'";
		case 1:
			if (validItemName(row[0]))
				taskName = row[0];
			else
				throw string("Expected a task name --- found '") + row[0] + "'";
			break;
		default:
			throw string("Expected 1, 2, 3, or 4 fields, found ") + to_string(row.size());
			
		}
	}
	void Task::print()
	{
		cout << " /Name/Slots/Pass/Fail/ = "
			<< "/" << taskName
			<< "/" << taskSlots
			<< "/" << taskPass
			<< "/" << taskFail
			<< "/\n";
	}
	void Task::graph(fstream& os)
	{
		string q = "\"";
		if (!taskPass.empty())
			os << q << taskPass << q << " -> " << q << taskPass << q << "[color=green];\n";

		if (!taskFail.empty())
			os << q << taskName << q << " -> " << q << taskPass << q << "[color=red];\n";

		if (taskPass.empty() && taskFail.empty())
			os << q << taskName << q << ";\n";
	}


	TaskManager::TaskManager(vector<vector<string>> &csvData)
	{
		for (auto& row : csvData)
		{
			try 
			{
				taskList.push_back(move(Task(row)));
			}
			catch (const string& e) {
				cerr << e << "\n";
			}
		}
	}
	void TaskManager::print()
	{
		for (auto t : taskList)
		{
			t.print();
		}
	}
	void TaskManager::graph(string& fileName)
	{
		string f = fileName + ".gv";
		fstream os(f, ios::out | ios::trunc);

		os << "digraph taskGraph {\n";

		for (auto t : taskList) {
			t.graph(os);
		}
		os << "}\n";
		os.close();
		string cmd = string("dot .Tpng") + fileName + ".gv" + fileName + ".gv.png";

		cout << cmd << " returned " << system(cmd.c_str()) << "\n";
	}

	Task * TaskManager::find(string & name)
	{
		for (size_t i = 0; i < taskList.size(); i++)
		{
			if (name == taskList[i].getTaskName())
				return &taskList[i];
		}
		return nullptr;
	}

	bool TaskManager::validate()
	{
		int error = 0;
		for (auto task :taskList)
		{
			if (!task.getTaskPass().empty() && find(task.getTaskPass()) == nullptr) {
				error++;
				cerr << "Task " << task.getTaskName() << "references unknow pass task" << task.getTaskPass() << "\n";
			}
			if (!task.getTaskFail().empty() && find(task.getTaskFail()) == nullptr) {
				error++;
				cerr << "Task " << task.getTaskName() << "references unknow fail task" << task.getTaskFail() << "\n";
			}
		}
		return error == 0;
	}
