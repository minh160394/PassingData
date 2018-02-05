#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "util.h"

using namespace std;

void trim(string & s)
{
	while (!s.empty() && s[0] == ' ')
		s.erase(0, 1);
	while (!s.empty() && s[s.size() - 1] == ' ')
	{
		s.erase(s.size() - 1, 1);
	}
}
void csvRead(string& filename, char seperator, vector<vector<string>> & csvData)
{
	fstream is(filename, ios::in);
	if (!is.is_open())
	{
		throw string("Cannot open file '") + filename + "'";
	}
	string line;
	while (getline(is, line))
	{
		if (line != "") {
			auto cr = line.find('\r');
			if (cr != string::npos)
				line.erase(cr);

			string field;
			vector<string> fields;
			size_t index = 0;
			bool valid = false;
			while (index < line.size())
			{
				
					while (index < line.size() && line[index] != seperator)
					{
						field += line[index];
						index++;
					}
					index++;
					trim(field);
					
					if (!field.empty()) {
						valid = true;
						fields.push_back(move(field));
					}
				
			}
			if(valid)
			csvData.push_back(move(fields));
		}
	}
	is.close();
}
void csvPrint(vector<vector<string>> & csvData)
{
	for (auto row : csvData)
	{
		for (auto column : row)
		{
			cout << "(" << column << ")";
		}
		cout << "\n";
	}
}

bool validTaskName(std::string & s)
{
	for (auto c : s)
	{
		if (!(isalnum(c) || c == ' '))
			return false;
	}
	return true;
}

bool validTaskSlots(std::string & s)
{
	for (auto c : s) 
	{
		if (!isdigit(c))
			return false;
	}
	return true;
}

bool validItemName(std::string & s)
{
	for (auto c : s) 
	{
		if (!(isalnum(c) || c == ' '))
			return false;
	}
	return true;
}

bool validItemSequence(std::string & s)
{
	for (auto c : s)
	{
		if (!isdigit(c))
			return false;
	}
	return true;
}

bool validCustomerName(std::string & s)
{
	for (auto c : s) 
	{
		if (!(isalnum(c) || c == ' '))
			return false;
	}
	return true;
}

bool validProductName(std::string & s)
{
	for (auto c : s) 
	{
		if (!(isalnum(c) || c == ' '))
			return false;
	}
	return true;
}
