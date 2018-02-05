#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "util.h"
#include "order.h"
using namespace std;

Order::Order(): orderCustomer("\0"), orderProduct("\0")
{
}

Order::Order(vector<string> &row)
	{
			if (row.size() < 3)
				throw string("Expected 3 or more fields, found ") + to_string(row.size());
			if (validCustomerName(row[0]))
				orderCustomer = row[0];
			else
				throw string("Expected a Customer name --- found '") + row[1] + "'";
			if (validProductName(row[1]))
				orderProduct = row[1];
			else
				throw string("Expected a Product name --- found '") + row[0] + "'";
			
			for (size_t i = 2; i < row.size(); i++) 
			{
				if (!row[i].empty() && row[i] != row[i - 1]) {

					if (validItemName(row[i]))
						itemList.push_back(row[i]);
					else
						throw string("Expected a item name --- found '") + row[i] + "'";
				}
			}
	}
	void Order::print()
	{
		cout << " /Customer/Product/Items .../"
			<< "/" << orderCustomer
			<< "/" << orderProduct
			<< "/\n";
	}
	void Order::graph(fstream& os)
	{
		string q = "\"";
		for (auto a : itemList)
			os << q << orderCustomer + ":" + orderProduct << q << " -> "<< "\""<<a <<"\"" <<"[color=blue];\n";
	}

	OrderManager::OrderManager(vector<vector<string>> & csvData)
	{
		for (auto& row : csvData)
		{
			try
			{
				itemList.push_back(move(Order(row)));
			}
			catch (const string& e)
			{
				cerr << e << "\n";
			}
		}
	}
	void OrderManager::print()
	{
		for (auto list : itemList)
		{
			list.print();
		}
	}

	void OrderManager::graph(string& fileName)
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

	bool OrderManager::validate(ItemManager & im)
	{
		int error = 0;
		for (auto order : itemList) {
			for (size_t i = 0; i < order.getSize(); i++) {
				if (im.find(order.getItemList(i)) == nullptr) {
					error++;
					cerr << "Customer " << order.getCustomer() << " ordered product " << order.getProduct()
						<< "reference unknown item " << order.getItemList(i) << "\n";
				}
			}
		 }
		return error == 0;
	}

