#pragma once
#include <string>
#include <fstream>

using namespace std;

struct UserAcc {
	string username;
	string password;
	UserAcc(string usr = "-1", string pwd = "-1") {
		username = usr;
		password = pwd;
		//This configration shows that
		//the data is null for this struct
	}
	bool compUser(UserAcc data) {
		if (data.username == username && data.password == password) {
			return true;
		}
		return false;
	}
};

static UserAcc nullUser;
static UserAcc delUser("-2", "-2");

class HashTable
{
private:
	UserAcc* arr;
	int length;
	int data_count;
public:
	HashTable(int defaultLen = 100) {
		length = defaultLen;
		arr = new UserAcc[length];
		data_count = 0;
	}

	int getUserCount() {
		return data_count;
	}

	int getLength() {
		return length;
	}

	UserAcc getUserAtIndex(int i) {
		return arr[i];
	}

	bool checkDataExists(UserAcc data) {
		int key = hashKey(data);

		if (arr[key].compUser(data)) {
			return true;
		}

		for (int i = 1; i < length; i++) {
			int k = (key + i) % length;
			if (arr[k].compUser(nullUser)) {
				return false;	// no such data found
			}
			if (arr[k].compUser(data)) {
				return true;
			}
		}
		return false;
	}

	bool checkUserExists(string usr_name) {
		int key = hashKey({ usr_name, "" });

		if (arr[key].username == usr_name) {
			return true;
		}

		for (int i = 1; i < length; i++) {
			int k = (key + i) % length;
			if (arr[k].username == "-1") {
				return false;	// no such data found
			}
			if (arr[k].username == usr_name) {
				return true;
			}
		}
		return false;

	}

	bool RegisterNewUser(UserAcc usr) {
		if (this->checkUserExists(usr.username)) { return false; }	//User already exists

		InsertData(usr);
		return true;
	}

	void InsertData(UserAcc data) {
		int key = hashKey(data);

		if (arr[key].compUser(nullUser) || arr[key].compUser(delUser)) {
			arr[key].username = data.username;
			arr[key].password = data.password;
			data_count++;
			CheckOverLoading();
			return;
		}

		for (int i = 1; i < length; i++) {
			int k = (key + i) % length;
			if (arr[k].compUser(nullUser) || arr[k].compUser(nullUser)) {
				arr[k].username = data.username;
				arr[k].password = data.password;
				data_count++;
				CheckOverLoading();
				return;
			}
		}
	}

	void DeleteData(UserAcc data) {
		int key = hashKey(data);

		if (arr[key].compUser(data)) {
			arr[key].username = delUser.username;
			arr[key].password = delUser.password;
			data_count--;
			return;
		}

		for (int i = 1; i < length; i++) {
			int k = (key + i) % length;
			if (arr[k].compUser(nullUser) || arr[k].compUser(delUser)) {
				return;	// no such data found
			}
			if (arr[k].compUser(data)) {
				arr[k].username = delUser.username;
				arr[k].password = delUser.password;
				data_count--;
				return;
			}
		}
	}

	void LoadUserNamePassWords() {
		fstream file;
		file.open("userpass.csv", ios::in);
		if (!file.is_open()) {
			fstream file;
			file.open("userpass.csv", ios::out);
			file.close();
			return;
		}	//Error Opening File

		data_count = 0;
		
		if (arr) {
			delete[] arr;
			arr = new UserAcc[length];
		}

		while (!file.eof()) {
			string str;
			string usr = "", pwd = "";
			getline(file, str);
			if (str == "") { break; }
			int i = 0;
			for (i = 0; i < str.length() && str[i] != ','; i++) {
				usr += str[i];
			}
			for (i = i + 1; i < str.length(); i++) {
				pwd += str[i];
			}
			this->InsertData(UserAcc(usr,pwd));
		}
		file.close();
	}

	void SaveUserNamePassWords() {
		fstream file;
		file.open("userpass.csv", ios::out);
		if (!file.is_open()) { return; }	//Error Opening File

		for (int i = 0; i < length; i++) {
			UserAcc usr = this->getUserAtIndex(i);
			if (!usr.compUser(nullUser) 
				&& !usr.compUser(delUser)) {
				//If not Null or Del; Save into file.
				file << usr.username << "," << usr.password << endl;
			}
		}
	}

	void CheckOverLoading() {
		//If Array is filled 70%
		//extend list size
		if (data_count / (double)length < 0.7) {
			return;
		}

		//extend size after saving data
		UserAcc* temp = new UserAcc[length];
		for (int i = 0; i < length; i++) {
			temp[i].username = arr[i].username;
			temp[i].password = arr[i].password;
		}
		data_count = 0;
		delete[] arr;

		arr = new UserAcc[length * 2];
		length *= 2;	//double the size
		for (int i = 0; i < length; i++) {
			if (temp[i].compUser(delUser) == false) {
				if (temp[i].compUser(nullUser) == false) {
					InsertData(temp[i]);
				}
			}
		}

	}
	int hashKey(UserAcc data) {
		string str = data.username;	//hashing based on username
		int key = 0;
		for (int i = 0; i < str.length(); i++) {
			key += str[i];
		}
		return key % length;
	}

	~HashTable() {
		if (arr) {
			delete[] arr;
		}
	}
};

