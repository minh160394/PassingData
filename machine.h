#ifndef MACHINE_H
#define MACHINE_H
#include <queue>
#include "task.h"
#include "job.h"
using namespace std;
class Machine : public Task {
	int intComing;
	queue <Job> inputQ;
	bool bInstaller;
	bool bRomver;

public:
	Machine() : intComing(0)
	{

	}
	Machine(Task * t) : Task(*t), intComing(0)
	{

	}
	void InintComing()
	{
		intComing++;
	}
	bool isSource() const { return intComing == 0; }
	bool isSink() { return getTaskPass().empty(); }
	bool isSingle() { return isSource() && isSink(); }

	void AddJob(Job job) { inputQ.push(move(job)); }

	size_t inputQsize() const { return inputQ.size(); }

	Job getJob()
	{
		Job job = move(inputQ.front() );
		inputQ.pop();
		return job;
	}
	void Classify(ItemManager& im)
	{
		bInstaller = false;
		bRomver = false;

		for (size_t i = 0; i < im.getsize(); i++)
		{
			if (getTaskName() == im.getItem(i)->getItemInstaller()) {
				bInstaller = true;
			}
			if (getTaskName() == im.getItem(i)->getItemRemover()) {
				bRomver = false;
			}
			if (bInstaller) cout << getTaskName() << "is an installer\n";
			if (bRomver) cout << getTaskName() << "is a remover\n";
			if (bInstaller && bRomver)
				throw getTaskName() + "is BOTH an and a romver";
		}	
	}
		bool getInstaller() const { return bInstaller; }
		bool getRemover() const { return bRomver; }
};
#endif 