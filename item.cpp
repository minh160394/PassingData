#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "util.h"
#include "item.h"
using namespace std;
 
	Item::Item(vector<string> &row)
	{
		switch (row.size())
		{
		case 5 :
			itemDescription = row[4];
		case 4:
			if (validItemSequence(row[3]))
				itemSequence = row[3];
			else
				throw string("Expected a sequence number --- found '") + row[3] + "'";
			
			if (validItemName(row[2]))
				itemRomver = row[2];
			else
				throw string("Expected a romover --- found '") + row[2] + "'";
			
			if (validItemName(row[1]))
				itemInstaller = row[1];
			else
				throw string("Expected a installer --- found '") + row[1] + "'";
			
			if (validItemName(row[0]))
				itemName = row[0];
			else
				throw string("Expected a Item name --- found '") + row[0] + "'";
			break;
		default:
			throw string("Expecte 4 or 5 fields, found ") + to_string(row.size());


		}
	}
	void Item::print()
	{
		cout << " /Name/Installer/Romver/Sequence/Description/= "
			<< "/" << itemName
			<< "/" << itemInstaller
			<< "/" << itemRomver
			<< "/" << itemSequence
			<< "/" << itemDescription
			<< "/\n";
	}
	void Item::graph(fstream& os)
	{
		string q = "\"";
		os << q << "Item " + itemName << q << " -> " << q << "Installer: " + itemInstaller << q << "[color=green];\n";
		os << q << "Item " + itemName << q << " -> " << q << "Romver: " + itemRomver << q << "[color=red];\n";
		
	}

	ItemManager::ItemManager(vector<vector<string>> & csvData)
	{
		for(auto& row : csvData)
		{
			try 
			{
				itemList.push_back( move(Item(row)));
			}
			catch (const string& e)
			{
				cerr << e << "\n";
			}
		}
	}
	void ItemManager::print()
	{
		for (auto list : itemList) 
		{
			list.print();
		}
	}

	void ItemManager::graph(string& fileName)
	{
		string f = fileName + ".gv";
		fstream os(f, ios::out | ios::trunc);

		os << "digraph itemGraph {\n";

		for (auto t : itemList) {
			t.graph(os);
		}
		os << "}\n";
		os.close();
		string cmd = string("dot .Tpng") + fileName + ".gv" + fileName + ".gv.png";

		cout << cmd << " returned " << system(cmd.c_str()) << "\n";
	}

	Item * ItemManager::find(string & name)
	{
		for (size_t i = 0; i < itemList.size(); i++)
		{
			if (name == itemList[i].getItemName())
				return &itemList[i];
		}
		return nullptr;
	}

	bool ItemManager::validate(TaskManager & tm)
	{
		int error = 0;
		for (auto item : itemList)
		{
			if (tm.find(item.getItemInstaller()) == nullptr) {
				error++;
				cerr << "Item " << item.getItemName() << "references unknow installer task" << item.getItemInstaller() << "\n";
			}
			if (tm.find(item.getItemRemover()) == nullptr) {
				error++;
				cerr << "Item " << item.getItemName() << "references unknow Romver task" << item.getItemRemover() << "\n";
			}
		}
		return error == 0;
	}
