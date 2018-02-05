#ifndef ITEM_H
#define ITEM_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "task.h"
#include "util.h"
using namespace std;

class Item
{
	string itemName, itemInstaller, itemRomver, itemSequence, itemDescription;

public:
	Item(vector<string> &row);
	void print();
	void graph(fstream& os);
	string& getItemName() { return itemName; };
	string& getItemInstaller() { return itemInstaller; };
	string& getItemRemover() { return itemRomver; };
	string& getItemSequence() { return itemSequence; };
};

class ItemManager
{
	vector<Item>itemList;
public:
	ItemManager(vector<vector<string>> & csvData);
	void print();
	void graph(string& fileName);
	size_t getsize() { return itemList.size(); }
	Item* getItem(size_t i) { return &itemList[i];}
	Item* find(string& name);
	bool validate(TaskManager& tm);
};
#endif