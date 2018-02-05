#ifndef TASK_H
#define TASK_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "util.h"
using namespace std;
class Task
{
	string taskName, taskSlots, taskPass, taskFail;
public:
	Task();
	Task(vector<string> &row);
	void print();
	void graph(fstream& os);
	string& getTaskName() { return taskName; };
	string& getTaskSlots() { return taskSlots; };
	string& getTaskPass() { return taskPass; };
	string& getTaskFail() { return taskFail; };
};

class TaskManager 
{
	vector< Task > taskList;
public:
	TaskManager(vector<vector<string>> &csvData);
	void print();
	size_t getsize() { return taskList.size(); }
	void graph(string& fileName);
	Task* find(string& name);
	Task* getTask(size_t t) { return &taskList[t]; }
	bool validate();
};
#endif