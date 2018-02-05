#ifndef ORDER_H
#define ORDER_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "item.h"
#include "util.h"
using namespace std;

class Order {
	string orderCustomer, orderProduct;
	vector<string> itemList;

public:
	Order();
	Order(vector<string> &row);
	void print();
	void graph(fstream& os);
	string& getCustomer() { return orderCustomer; };
	string& getProduct() { return orderProduct; };
	size_t getSize() { return itemList.size(); };
	string& getItemList(size_t i) { return itemList[i]; };
};
class OrderManager
{
	vector<Order>itemList;
public:
	OrderManager(vector<vector<string>> & csvData);
	void print();
	void graph(string& fileName);
	size_t getSize() { return itemList.size(); };
	bool validate(ItemManager& im);
	Order* getOrder(size_t i) { return &itemList[i]; }

};
#endif