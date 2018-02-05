#ifndef JOB_H
#define JOB_H
#include "order.h"
#include <vector>
using namespace std;
class Job : public Order
{
	vector<bool> installed;
public:
	Job()	{}
	Job(Order *o) : Order(*o) 
	{
		installed.resize(getSize());
		for (size_t i = 0; i < getSize(); i++) 
		{
			installed[i] = false;
		}
	}
	bool Installed(size_t i) { return installed[i]; }
	void Installed(size_t i, bool value) { installed[i] = value; }
};
#endif 