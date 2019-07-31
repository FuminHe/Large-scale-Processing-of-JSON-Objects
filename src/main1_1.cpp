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
//struct json_struct {                                                         
//	int id;                                                                      
//	double cost;																  //
//	std::vector<double> cost_components;             
//													  
//	json_struct(){									  
//		id = 0;										                
//		cost = 0;                                                                 
//	}                                                                            
//};                                                                              
////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//Function Prototype
void json_parse(char* buf);
void findAverage(Json::Value j);
void find_sub_Average(Json::Value j_1, int thr_num);
void findMaxCC(Json::Value j1);
void find_c_idlist(Json::Value j2);
void find_cc_idlist(Json::Value j3);


//global
//const int THREAD_NUM = 4;
double sum = 0;
mutex mtx;

//----------------------------------------------------------------------------------
int main() {

	clock_t start = clock();

	std::ifstream inFile("medium.txt", ios::binary);
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

	findAverage(jsonObj);
	findMaxCC(jsonObj);
	find_c_idlist(jsonObj);
	find_cc_idlist(jsonObj);

}


//-------------------------------------------------------------------------------
void findAverage(Json::Value j) {
	//double sum = 0;
	//for (int i = 0; i < j.size(); ++i) {
	//	sum = sum + j[i]["cost"].asDouble();
	//}

	//double average = sum / j.size();
	//cout << "The average is: " << average << endl;
	thread thr1_1(find_sub_Average, j, 1);
	thread thr1_2(find_sub_Average, j, 2);
	thread thr1_3(find_sub_Average, j, 3);
	thread thr1_4(find_sub_Average, j, 4);
	thr1_1.join();
	thr1_2.join();
	thr1_3.join();
	thr1_4.join();

	cout << "The average is: " << sum / j.size() << endl;
}

void find_sub_Average(Json::Value j_1, int thr_num) {
	int start = 0, end = 0;
	int j_size = j_1.size();
	int interval_size = j_size / 4;
	// c1: 0 -> 
	switch (thr_num) {
	case 1:
		start = 0;
		end = interval_size;
		break;
	case 2:
		start = interval_size + 1;
		end = 2 * interval_size;
		break;
	case 3:
		start = (2 * interval_size) + 1;
		end = 3 * interval_size;
		break;
	case 4:
		start = (3 * interval_size) + 1;
		end = j_size - 1;
		break;
	default:
		cout << "Invalide thr_num!" << endl;
		break;
	}

	double sub_sum = 0;
	for (int i = start; i <= end; ++i) {
		sub_sum = sub_sum + j_1[i]["cost"].asDouble();
	}
	mtx.lock();
	sum = sum + sub_sum;
	mtx.unlock();
}


//-------------------------------------------------------------------------
void findMaxCC(Json::Value j1) {
	double maxCC = 0;
	for (auto& el : j1) {
		for (int i = 0; i < el["cost_components"].size(); ++i) {
			if (el["cost_components"][i].asDouble() > maxCC) {
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


