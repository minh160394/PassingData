#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "task.h"
#include "util.h"
#include "order.h"
#include "item.h"
#include "task.h"
#include "machine.h"
#include <time.h>
using namespace std;

class Factory {
	vector <Machine> machineList;
public:
	Factory(ItemManager& im, OrderManager& om, TaskManager& tm)
	{
		srand(time(0));

		for (size_t i = 0; i < tm.getsize(); i++)
		{
			machineList.push_back(move(Machine(tm.getTask(i) ) ) );
		}

		auto FindMachine = [&](string n) -> Machine* {
			for (size_t i = 0; i < machineList.size(); i++) {
				if (machineList[i].getTaskName() == n)
					return &machineList[i];
			}
			return nullptr;
		};

		for (auto m : machineList) {
			string pass = m.getTaskPass();
			if (!pass.empty()) FindMachine(pass)->InintComing();

			string fail = m.getTaskFail();
			if (!fail.empty()) FindMachine(fail)->InintComing();
		}

		int source = -1;
		for (size_t m = 0; m < machineList.size(); m++)	
		{
			cout << m << " : " << machineList[m].getTaskName();
				if (machineList[m].isSingle())
				{
				cout << " Single ";
				}
				else
				{
					if (machineList[m].isSource())
					{
						cout << " Source ";
						if (source == -1)
						{
							source = m;
						}
						else
						{
							throw string("Multiple sources. Fix data and resubmit!");
						}
					}
					if (machineList[m].isSink()) cout << " Sink ";
				}
				cout << "\n";
		}

		cout << "Source is " << source << "\n";

		if (source == -1) throw string("No Source Model!! Fix data and resubmit!! ");

		for (size_t o = 0; o < om.getSize(); o++)
		{
			machineList[source].AddJob(Job(om.getOrder(o)));
		}
		for (auto m : machineList)
		{
			cout << m.getTaskName() << "has" << m.inputQsize() << " job.\n";
		}
		for (auto &m : machineList)
		{
			m.Classify(im);
		}


		int time = 0;
		while (true)
		{
			cout << "Time = " << time << "\n";

			bool AllDone = true;
			for (auto m : machineList) {
				if (m.inputQsize()) {
					AllDone = false;
					break;
				}
			}
		
			if (AllDone) {
				cout << "All Done\n";
				break;
			}

			for (auto &m : machineList) {
				if (m.inputQsize() == 0) {
					continue;
				}

				Job job = m.getJob();

				cout << "'" << m.getTaskName() << "' is looking at job "
					<< job.getCustomer() << "/"
					<< job.getProduct() << "\n";
				if (m.isSink()) {
					cout << "Job arrived at '" << m.getTaskName() << "' a sink --- nothing to do .\n";
					cout << "Job exiting system\n";
					continue;
				}

				bool didSomething = false;

				if (m.getInstaller()) {
					cout << "'" << m.getTaskName() << "' is an installer\n";
				
					for (size_t i = 0; i < job.getSize(); i++) {
						if (job.Installed(i)) continue;

						string itemName = job.getItemList(i);
						Item* itemPtr = im.find(itemName);

						if (m.getTaskName() == itemPtr->getItemInstaller()) {
							job.Installed(i, true);
							didSomething = true;
							time++;
							cout << "'" << m.getTaskName() << "' installed '" << itemName << "'\n";
							break;
						}
					}
				}

				if (m.getRemover()) {
					cout << "'" << m.getTaskName() << "' is an Remover\n";
					for (size_t i = job.getSize() - 1; i >= 0; i--) {
						if (!job.Installed(i)) continue;
						string itemName = job.getItemList(i);
						Item* itemPtr = im.find(itemName);
						if (m.getTaskName() == itemPtr->getItemRemover()) {
							job.Installed(i, false);
							didSomething = true;
							time++;
							cout << "'" << m.getTaskName() << "' remover '" << itemName << "'\n";
							break;
						}
					}
				}
				auto MovePass = [&] {
					string pass = m.getTaskPass();
					cout << " moving job to pass machine '" << pass << "'\n";
					for (auto &m2 : machineList) {
						if (m2.getTaskName() == pass) {
							m2.AddJob(job);
							break;
						}
					}
				};
				auto MoveFail = [&] {
					string fail = m.getTaskFail();
					cout << " moving job to pass machine '" << fail << "'\n";
					for (auto &m2 : machineList) {
						if (m2.getTaskName() == fail) {
							m2.AddJob(job);
							break;
						}
					}
				};
				if (!didSomething) {
					MovePass();
					continue;
				}

				if (m.getTaskFail().empty()) {
					MovePass();
					continue;
				}
				if (rand() & 1) {
					MovePass();
					continue;
				}
				else {
					MoveFail();
					continue;
				}
			}
		}

		cout << "Simulation over \n";
		cout << "Time = " << time << "\n";
	}
};

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 5)
			throw string("Usage: ") + argv[0] + " item order task 3 csv-data-files csv-data-seperator";

		string fileItemName = argv[1];
		string fileOrderName = argv[2];
		string fileTaskName = argv[3];
		char sepertor = argv[4][0];

		vector <vector < string >> csvItemData;
		vector <vector < string >> csvOrderData;
		vector <vector < string >> csvTaskData;

		csvRead(fileItemName, sepertor, csvItemData);
		csvRead(fileOrderName, sepertor, csvOrderData);
		csvRead(fileTaskName, sepertor, csvTaskData);
	
		ItemManager in(csvItemData);
		in.print();
		in.graph(fileItemName);

		TaskManager tm(csvTaskData);
		tm.print();
		tm.graph(fileTaskName);
		OrderManager on(csvOrderData);
		on.print();
		on.graph(fileOrderName);

		

		bool GoodToGo = true;
		cout << "Item integrity check \n";
		try {
			in.validate(tm);
		}
		catch (string& e) {
			GoodToGo = false;
			cerr << e << "\n";
		}

		cout << "Order integrity check \n";
		try {
			on.validate(in);
		}
		catch (string& e) {
			GoodToGo = false;
			cerr << e << "\n";
		}

		cout << "Task integrity check \n";
		try {
			tm.validate();
		}
		catch (string& e) {
			GoodToGo = false;
			cerr << e << "\n";
		}

		if (GoodToGo == false) {
			cerr << "**************************************\n";
			cerr << "* DATA HAS ERRORS. FIX AND RESUBMITT!!\n";
			cerr << "**************************************\n";
			exit(1);
		}

		cout << "*****************************************************\n";
		cout << "* DATA PASSED THE INTEGRITY CHECK AND IS GOOD TO GO!!\n";
		cout << "*****************************************************\n";
	
		Factory factory(in, on, tm);
	}
	catch (const string& e) {
		cerr << e << "\n";
		return 1;
	}
	return 0;
}