#include "json/json.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <unordered_map>
#include <thread>
#include <stdlib.h>
#include <Windows.h>
#include <mutex>
using namespace std;


////////////////////////////////////////////////////////////////////////////////////
//struct json_struct {                                                            //
//	int id;                                                                       //
//	double cost;																  //
//	std::vector<double> cost_components;              Don't need to use this	  //
//													  to store or serialize       //
//	json_struct(){									  because jsoncpp has done    //
//		id = 0;										  it ??                       //
//		cost = 0;                                                                 //
//	}                                                                             //
//};                                                                              //
////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//Function Prototype
void json_parse(char* buf);
void findAverage(Json::Value j);
//void find_sub_Average(Json::Value j_1, int thr_num);
void findMaxCC(Json::Value j1);
void find_c_idlist(Json::Value j2);
void find_cc_idlist(Json::Value j3);

//global
double sum = 0;
mutex mtx;


//----------------------------------------------------------------------------------
int main() {

	clock_t start = clock();

	std::ifstream inFile("large.txt", ios::binary);
	if (inFile.fail()) {
		cout << "Error: Fail to open the file! " << endl;
		exit(1);
	}

	// get file length
	inFile.seekg(0, inFile.end);
	int file_length = inFile.tellg();
	inFile.seekg(0, inFile.beg);

	// add [] to the file, in order to use jsoncpp
	char L_bracket = '[';
	char R_bracket = ']';
	int length = file_length + sizeof(L_bracket) + sizeof(R_bracket);
	char* buffer = new char[length];
	buffer[0] = L_bracket;
	inFile.read(buffer + 1, file_length);
	buffer[file_length + 1] = R_bracket;
	inFile.close();

	json_parse(buffer);
	delete[] buffer;
	inFile.close();

	clock_t end = clock();
	cout << "\nRunning time: " << end - start << " ms" << endl;
	return 0;
}


//----------------------------------------------------------------------------------
void json_parse(char* buf) {
	Json::Reader read;
	Json::Value jsonObj;

	bool parse_ok = read.parse(buf, jsonObj);
	
	if (!parse_ok) {
		cout << "Error: Fail to parse the file!" << endl;
		exit(1);
	}

	//findAverage(jsonObj);
	thread thr1(findAverage, jsonObj);
	thread thr2(findMaxCC, jsonObj);
	thread thr3(find_c_idlist, jsonObj);
	thread thr4(find_cc_idlist, jsonObj);

	thr1.join();
	thr2.join();
	thr3.join();
	thr4.join();
}


//------------------------------------------------------------------------
void findAverage(Json::Value j) {
	double sum = 0;
	for (int i = 0; i < j.size(); ++i) {
		sum = sum + j[i]["cost"].asDouble();
	}
	
	double average = sum / j.size();
	cout << "The average is: " << average << endl;
}



//-------------------------------------------------------------------------
void findMaxCC(Json::Value j1) {
	double maxCC = 0;
	for (auto& el : j1) {
		for (int i = 0; i < el["cost_components"].size(); ++i) {
			if(el["cost_components"][i].asDouble() > maxCC){
				maxCC = el["cost_components"][i].asDouble();
			}
		}
	}
	cout << "The max cost_components is: " << maxCC << endl;
}


//-------------------------------------------------------------------------
void find_c_idlist(Json::Value j2) {
	unordered_map<int, int> Q3List;
	for (auto& el : j2) {
		if (el["cost"].asDouble() > 95) {
			Q3List.insert(map<int, int>::value_type(el["id"].asInt(), el["id"].asInt()));
		}
	}

	//output
	//cout << "\nThe answer for Q3 is: ";
	//unordered_map<int, int>::iterator iter_Q3 = Q3List.begin();
	//for (; iter_Q3 != Q3List.end(); iter_Q3++)
	//	cout << iter_Q3->second << " ";

	ofstream outFile1("Q3List.txt");
	outFile1 << "The answer for Q3 is: ";
	unordered_map<int, int>::iterator iter_Q3 = Q3List.begin();
	while (iter_Q3 != Q3List.end()) {
		outFile1 << iter_Q3->second << " ";
		iter_Q3++;
	}
	outFile1.close();
	cout << "Creat Q3List.txt!" << endl;
}


//---------------------------------------------------------------------------
void find_cc_idlist(Json::Value j3) {
	unordered_map<int, int> Q4List;
	for (auto& el : j3) {
		for (int i = 0; i < el["cost_components"].size(); ++i) {
			if (el["cost_components"][i].asDouble() > 50) {
				Q4List.insert(map<int, int>::value_type(el["id"].asInt(), el["id"].asInt()));
				break;
			}
		}
	}

	//output
	//cout << "\nThe answer for Q4 is: ";
	//unordered_map<int, int>::iterator iter_Q4 = Q4List.begin();
	//for (; iter_Q4 != Q4List.end(); iter_Q4++)
	//	cout << iter_Q4->second << " ";

	ofstream outFile2("Q4List.txt");
	outFile2 << "The id list for Q4 is: ";
	unordered_map<int, int>::iterator iter_Q4 = Q4List.begin();
	while (iter_Q4 != Q4List.end()) {
		outFile2 << iter_Q4->second << " ";
		iter_Q4++;
	}
	outFile2.close();
	cout << "Creat Q4List.txt!" << endl;
}
