#include <string>
#include <vector>

void trim(std::string & s);
void csvRead(std::string& filename, char seperator, std::vector<std::vector<std::string>> & csvData);
void csvPrint(std::vector<std::vector<std::string>> & csvData);

bool validTaskName(std::string &s);
bool validTaskSlots(std::string &s);
bool validItemName(std::string &s);
bool validItemSequence(std::string &s);
bool validCustomerName(std::string &s);
bool validProductName(std::string &s);