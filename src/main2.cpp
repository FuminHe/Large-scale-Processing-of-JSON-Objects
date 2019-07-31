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


//----------------------------------------------------------------------------------
//Function Prototype
void json_parse(char* buf);
void find_sub_idList(Json::Value j_1, int thr_num);
void insert_id(int ID, int map_num);

//global
mutex mtx;
unordered_map<int, vector<int>> id_map; 
// key=1 -->  [0-12]
// key=2 -->  [13-24]
// key=3 -->  [25-36]
// key=4 -->  [37-48]
// key=5 -->  [49-60]

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

	//output
	//for (int i = 1; i <= 5; i++) {
	//	//cout << "\nThe id of cost_components which are in " << i << " : ";
	//	for (int j = 0; j < id_map[i].size(); j++) {
	//		cout << id_map[i][j] << " ";
	//	}		
	//}

	ofstream outFile("idList.txt");
	outFile << "The id for cost_components which are in [0-12] are: ";
	for (int i = 0; i < id_map[1].size(); i++) {
		outFile << id_map[1][i] << " ";
	}
	outFile << "\nThe id for cost_components which are in [13-24] are: ";
	for (int i = 0; i < id_map[2].size(); i++) {
		outFile << id_map[2][i] << " ";
	}
	outFile << "\nThe id for cost_components which are in [25-36] are: ";
	for (int i = 0; i < id_map[3].size(); i++) {
		outFile << id_map[3][i] << " ";
	}
	outFile << "\nThe id for cost_components which are in [37-48] are: ";
	for (int i = 0; i < id_map[4].size(); i++) {
		outFile << id_map[4][i] << " ";
	}
	outFile << "\nThe id for cost_components which are in [49-60] are: ";
	for (int i = 0; i < id_map[5].size(); i++) {
		outFile << id_map[5][i] << " ";
	}
	outFile.close();
	cout << "Creat idList.txt!" << endl;

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

	thread thr1(find_sub_idList, jsonObj, 1);
	thread thr2(find_sub_idList, jsonObj, 2);
	thread thr3(find_sub_idList, jsonObj, 3);
	thread thr4(find_sub_idList, jsonObj, 4);
	thread thr5(find_sub_idList, jsonObj, 5);

	thr1.join();
	thr2.join();
	thr3.join();
	thr4.join();
	thr5.join();

}


//-------------------------------------------------------------------------------
void find_sub_idList(Json::Value j_1, int thr_num) {
	int start = 0, end = 0;
	int j_size = j_1.size();
	int interval_size = j_size / 5;
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
		end = 4 * interval_size;
		break;
	case 5:
		start = (4 * interval_size) + 1;
		end = j_size - 1;
		break;
	default:
		cout << "Invalide thr_num!" << endl;
		break;
	}

	//double sub_sum = 0;
	for (int i = start; i <= end; ++i) {
		//sub_sum = sub_sum + j_1[i]["cost"].asDouble();
		for (int j = 0; j < j_1[i]["cost_components"].size(); ++j) {
			if (j_1[i]["cost_components"][j].asDouble() <= 12){
				mtx.lock();
				insert_id(j_1[i]["id"].asInt(), 1);
				mtx.unlock();
			}
			else if (j_1[i]["cost_components"][j].asDouble() <= 24){
				mtx.lock();
				insert_id(j_1[i]["id"].asInt(), 2);
				mtx.unlock();
			}
			else if (j_1[i]["cost_components"][j].asDouble() <= 36) {
				mtx.lock();
				insert_id(j_1[i]["id"].asInt(), 3);
				mtx.unlock();
			}
			else if (j_1[i]["cost_components"][j].asDouble() <= 48) {
				mtx.lock();
				insert_id(j_1[i]["id"].asInt(), 4);
				mtx.unlock();
			}
			else if (j_1[i]["cost_components"][j].asDouble() <= 60) {
				mtx.lock();
				insert_id(j_1[i]["id"].asInt(), 5);
				mtx.unlock();
			}
		}
	}		
}


//=========================================================================
void insert_id(int ID, int key_num) {
	vector<int>::iterator iter;
	iter = find(id_map[key_num].begin(), id_map[key_num].end(), ID);
	if (iter == id_map[key_num].end()) {
		// if this id is not in the vector
		id_map[key_num].push_back(ID);
	}		
}
