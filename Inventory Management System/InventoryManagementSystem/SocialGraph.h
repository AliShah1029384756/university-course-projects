#pragma once
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class SocialGraph
{
private:
	string* UsersList;
	vector<string>* FollowersList;
	int totalUsers;
public:
	SocialGraph() {
		totalUsers = 0;
		UsersList = nullptr;		//new string[totalUsers];
		FollowersList = nullptr;	//new vector<string>[totalUsers];
	}
	int getTotalUsers() {
		return totalUsers;
	}
	int hasUser(string usr) {
		for (int i = 0; i < totalUsers; i++) {
			if (UsersList[i] == usr) {
				return i;	// returns the index of user
			}
		}
		return -1;	// -1 shows user does not exist in list
	}
	void loadData() {
		fstream file;
		file.open("followersdata.csv", ios::in);
		totalUsers = 0;
		if (!file.is_open()) {
			//No File? Create a new one :D
			file.open("followersdata.csv", ios::out);
			if (totalUsers > 0) {
				for (int i = 0; i < totalUsers; i++) {
					file << UsersList[i] << ":" << endl;
				}
			}
			file.close();
			//File Created
			return;
		}

		//Count total Users (the no. of lines)
		string line;
		while (getline(file, line, '\n')) {
			totalUsers++;
		}
		file.close();

		file.open("followersdata.csv", ios::in);
		if (!file.is_open()) { return; }

		if (UsersList != nullptr)
			delete[] UsersList;
		if (FollowersList != nullptr)
			delete[] FollowersList;

		UsersList = new string[totalUsers];
		FollowersList = new vector<string>[totalUsers];

		int usr_index = 0;

		while (getline(file, line)) {
			string usr, foll;
			int i;
			for (i = 0; i < line.length() && line[i] != ':'; i++) {
				usr += line[i];
			}
			UsersList[usr_index] = usr;
			i++;	//ignore ':'
			while (i < line.length()) {
				if (line[i] == ',' || line[i] == '\0' || line[i] == '\n') {
					if(foll != "")
						FollowersList[usr_index].push_back(foll);
					foll = "";
					i++; continue;
				}

				foll += line[i];
				i++;
			}
			FollowersList[usr_index].push_back(foll);
			usr_index++;
		}
	}

	vector<string> getFollowingList(string usr) {
		int index = hasUser(usr);
		if (index == -1) { vector<string> temp;  return temp; }

		return FollowersList[index];
	}

	void saveData() {
		fstream file;
		file.open("followersdata.csv", ios::out | ios::trunc);

		if (!file.is_open()) { return; }	//Error?
		for (int i = 0; i < totalUsers; i++) {
			file << UsersList[i] << ":";
			for (int j = 0; j < FollowersList[i].size(); j++) {
				file << FollowersList[i][j];
				if (j + 1 != FollowersList[i].size()) {
					file << ",";
				}
			}
			file << endl;
		}
		file.close();
	}
	void addNewUsr(string usr) {
		fstream file;
		file.open("followersdata.csv", ios::app);
		file << usr << ":" << endl;
		file.close();
		this->loadData();
	}
	bool hasFollower(string usr, string followerName) {
		int index = hasUser(usr);
		if (index == -1) { return false; }

		for (int i = 0; i < FollowersList[index].size(); i++) {
			if (FollowersList[index][i] == followerName) {
				return true;
			}
		}
		return false;
	}
	
	void addFollower(string usr, string followerName) {
		int index = hasUser(usr);
		if (index == -1) { return; }
		if (hasUser(followerName) < 0) { return; }	//Follower User doesn't Exists
		if (usr == followerName) { return; }	//Dont Follow yourself mate

		if (hasFollower(usr, followerName)) { return; }

		FollowersList[index].push_back(followerName);
	}
	void removeFollower(string usr, string followerName) {
		int index = hasUser(usr);
		if (index == -1) { return; }

		for (auto x = FollowersList[index].begin(); x != FollowersList[index].end();) {
			if (*x == followerName) {
				x = FollowersList[index].erase(x);
			}
			else {
				x++;
			}
		}
	}
	vector<string> getUserList() {
		vector<string> ul;
		for (int i = 0; i < totalUsers; i++) {
			ul.push_back(UsersList[i]);
		}
		return ul;
	}
	~SocialGraph() {
		if (UsersList) {
			delete[] UsersList;
		}
		if (FollowersList) {
			delete[] FollowersList;
		}
	}
};
