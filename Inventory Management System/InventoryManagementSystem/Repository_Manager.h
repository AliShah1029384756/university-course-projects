#pragma once
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct LNode {
	string data;
	LNode* next;
	LNode(string s) {
		data = s;
		next = nullptr;
	}
};

class LinkedList {
private:
	LNode* head;
	int node_count;
public:
	LinkedList() {
		node_count = 0;
		head = nullptr;
	}
	void Insert(string str) {
		LNode* newNode = new LNode(str);
		node_count++;
		if (head == nullptr) {
			head = newNode;
		}
		else {
			LNode* temp = head;
			while (temp->next != nullptr) {
				temp = temp->next;
			}
			temp->next = newNode;
		}
	}
	void Delete(string str) {
		if (head == nullptr) { return; }
		if (head->data == str) {
			LNode* temp = head;
			head = head->next;
			node_count--;
			delete temp;
		}
		else {
			LNode* curr = head;
			LNode* prev = head;
			while (curr != nullptr && curr->data != str) {
				prev = curr;
				curr = curr->next;
			}
			if (curr == nullptr) { return; }

			prev->next = curr->next;
			node_count--;
			delete curr;
		}
	}
	int getNodeCount() {
		return node_count;
	}
	string getDataAt(int index) {
		if (head == nullptr) { return ""; }
		if (index == 0) { return head->data; }
		int i = 0;
		LNode* curr = head;
		while (curr != nullptr && index != i) {
			curr = curr->next;
			i++;
		}
		if (curr == nullptr) { return ""; }
		return curr->data;
	}
};

struct Node {
	string Repo_Name;
	LinkedList* files;
	LinkedList* commits;
	bool isPrivate;
	int height;
	Node* left;
	Node* right;
	Node(string RepName) {
		Repo_Name = RepName;
		files = new LinkedList;
		commits = new LinkedList;
		isPrivate = true;	//Private Repo by default
		height = 1;
		left = nullptr;
		right = nullptr;
	}
};

class Repository_Manager
{
private:
	string Repo_Owner;	//User's Name
	Node* root;

	int getHeight(Node* node) {
		if (node == nullptr)
			return 0;
		return node->height;
	}
	int getBalance(Node* node) {
		if (node == nullptr)
			return 0;
		return getHeight(node->left) - getHeight(node->right);
	}
	

	void setPrivate(Node* node, string repo_name, bool status) {
		if (node == nullptr) {
			return;
		}
		else {
			if (node->Repo_Name == repo_name) {
				node->isPrivate = status;
			}
			else if (node->Repo_Name < repo_name) {
				setPrivate(node->right, repo_name, status);
			}
			else {	//node->Repo_Name > name
				setPrivate(node->left, repo_name, status);
			}
		}
	}
	Node* rightRotate(Node* y) {
		Node* x = y->left;
		Node* T = x->right;
		x->right = y; 
		y -> left = T;
		y->height = max(getHeight(y->left), getHeight(y->right)) + 1; 
		x -> height = max(getHeight(x->left), getHeight(x->right)) + 1;
		return x;
	}
	Node* leftRotate(Node* x) {
		Node* y = x->right;
		Node* T = y->left;
		y->left = x; 
		x -> right = T;
		x->height = max(getHeight(x->left), getHeight(x->right)) + 1; 
		y -> height = max(getHeight(y->left), getHeight(y->right)) + 1;
		return y;
	}

	Node searchRepo(Node* node, string name) {
		if (node == nullptr) {
			return Node("");
		}
		else {
			if (node->Repo_Name == name) {
				return *node;
			}
			else if (node->Repo_Name < name) {
				return searchRepo(node->right, name);
			}
			else {	//node->Repo_Name > name
				return searchRepo(node->left, name);
			}
		}
	}

	Node* insertFile(Node* node, string repName, string fileName = "", string commit_msg = "") {
		if (node == nullptr) {
			Node* newRepo = new Node(repName);
			if (fileName != "") {	
				//File Name can be "" when only creating a new Repo
				//Add Into Linked List
				newRepo->files->Insert(fileName);
			}
			if (commit_msg != "") {
				//Add Into Commits
				newRepo->commits->Insert(commit_msg);
			}
			return newRepo; 
		}
		if (repName < node->Repo_Name) {
			node->left = insertFile(node->left, repName, fileName, commit_msg);
		}
		else if (repName > node->Repo_Name) {
			node->right = insertFile(node->right, repName, fileName, commit_msg);
		}
		else {
			//Repo Exists append into it
			if (fileName != "") {
				//File Name can be "" when only creating a new Repo
				//Add Into Linked List
				node->files->Insert(fileName);
			}
			if (commit_msg != "") {
				//Add Into Commits
				node->commits->Insert(commit_msg);
			}
			return node; 
		}

		node->height = 1 + max(getHeight(node->left), getHeight(node->right));
		int balance = getBalance(node);

		if (balance > 1 && repName < node->left->Repo_Name)
			return rightRotate(node);
		if (balance < -1 && repName > node->right->Repo_Name)
			return leftRotate(node);
		if (balance > 1 && repName > node->left->Repo_Name) {
			node->left = leftRotate(node->left);
			return rightRotate(node);
		}
		if (balance < -1 && repName < node->right->Repo_Name) {
			node->right = rightRotate(node->right);
			return leftRotate(node);
		}
		return node;
	}
	Node* minValueNode(Node* node) {
		Node* current = node; 
		while (current->left != nullptr)
			current = current->left;
		return current;
	}

	Node* deleteRepo(Node* root, string repoName) {
		if (root == nullptr) {
			return root;
		}
		if (repoName < root->Repo_Name) { 
			root->left = deleteRepo(root->left, repoName); 
		}
		else if (repoName > root->Repo_Name) {
			root->right = deleteRepo(root->right, repoName);
		}
		else {
			if (root->left == nullptr || root->right == nullptr) {
				Node* temp = root->left ? root->left : root->right;
				if (temp == nullptr) {
					temp = root;
					root = nullptr;
				}
				else {
					*root = *temp;
				}
				delete temp;
			}
			else
			{
				Node* temp = minValueNode(root->right);
				root->Repo_Name = temp->Repo_Name; 
				root->files = temp->files;
				root->commits = temp->commits;
				root->right = deleteRepo(root->right, temp->Repo_Name);
			}
		}

		if (root == nullptr)
			return root;

		root->height = 1 + max(getHeight(root->left), getHeight(root->right));
		int balance = getBalance(root);

		if (balance > 1 && getBalance(root->left) >= 0)
			return rightRotate(root);
		if (balance > 1 && getBalance(root->left) < 0) {
			root->left = leftRotate(root->left);
			return rightRotate(root);
		}
		if (balance < -1 && getBalance(root->right) <= 0)
			return leftRotate(root);
		if (balance < -1 && getBalance(root->right) > 0) {
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}
		return root;
	}
	void deleteFileFromRepo(Node* node, string repoName, string fileName, string commit_msg) {
		if (node == nullptr) {
			return;
		}
		if (node->Repo_Name == repoName) {
			node->files->Delete(fileName);
			node->commits->Insert(commit_msg);
		}
		else if (node->Repo_Name < repoName) {
			deleteFileFromRepo(node->right, repoName, fileName, commit_msg);
		}
		else {// node->Repo_Name > repoName
			deleteFileFromRepo(node->left, repoName, fileName, commit_msg);
		}
	}
	void saveDataRecurr(Node*& node) {
		if (node == nullptr) { return; }	//Just in case
		if (Repo_Owner == "") { return; }
		
		fstream file;
		string file_name = Repo_Owner + "_repo.csv";

		file.open(file_name, ios::app);
		if (!file.is_open()) { return; }
		string isprivt = node->isPrivate ? "1" : "0";
		string line = node->Repo_Name + ":" + isprivt + ":";
		int f_count = node->files->getNodeCount();
		int c_count = node->commits->getNodeCount();

		for (int i = 0; i < f_count; i++) {
			line += node->files->getDataAt(i);
			if (i + 1 != f_count)
				line += ",";
		}
		line += ":";
		for (int i = 0; i < c_count; i++) {
			line += node->commits->getDataAt(i);
			if (i + 1 != c_count)
				line += ",";
		}
		file << line << endl;
		file.close();
	}
	void inorderTraversalSave(Node* node) {
		if (node == nullptr) { return; }
		
		inorderTraversalSave(node->left);
		saveDataRecurr(node);
		inorderTraversalSave(node->right);
		
	}
	void inorderTraversalRepoGrab(Node* node, vector<string>& vect) {
		if (node != nullptr) {
			inorderTraversalRepoGrab(node->left, vect);
			vect.push_back(node->Repo_Name);
			inorderTraversalRepoGrab(node->right, vect);
		}
	}
	Node* getRepoRecurr(Node* node, string name) {
		if (node == nullptr) {
			return nullptr;
		}
		else if (node->Repo_Name == name) {
			return node;
		}
		else if (node->Repo_Name < name) {
			return getRepoRecurr(node->right, name);
		}
		else {	//node->Repo_Name > name
			return getRepoRecurr(node->left, name);
		}
	}
public:
	Repository_Manager() {
		root = nullptr;
	}

	void createRepo(string repoName) {
		root = this->insertFile(root, repoName);
	}
	void insertFile(string repoName, string fileName) {
		root = this->insertFile(root, repoName, fileName);
	}
	void insertCommitMsg(string repoName, string commitMsg) {
		root = this->insertFile(root, repoName, "", commitMsg);
	}

	void loadData(string Owner) {
		fstream file;
		Repo_Owner = Owner;
		string file_name = Repo_Owner + "_repo.csv";
		file.open(file_name, ios::in);
		if (!file.is_open()) {
			file.open(file_name, ios::out);	//Create a new File
			file.close(); return;
		}

		string line;

		//int repo_count = 0;
		//while (getline(file, line) && line != "") {
		//	repo_count++;
		//}
		//file.close();

		/*file.open(file_name, ios::in);
		if (!file.is_open()) { return; }*/
		
		while (getline(file, line, '\n')) {
			string repo_name;
			string fileName;
			string commit_msg;
			if (line == "") { break; }

			int i;
			for (i = 0; i < line.length() && line[i] != ':'; i++) {
				repo_name += line[i];
			}
			i++;	//Ingore the ':'
			if(repo_name != "")
				createRepo(repo_name);
			//Get Private Status
			char isprivt = line[i]; i+=2; //Ignore the '0'/'1' AND ':'
			this->setPrivate(root, repo_name, isprivt - '0');
			
			while (i <= line.length() && line[i] != ':') {
				if (line[i] == ',' || line[i] == '\0' || line[i] == '\n') {
					if (fileName != "")
						this->insertFile(repo_name, fileName);
					fileName = "";
					i++; continue;
				}
				fileName += line[i];
				i++;
			}
			i++; //Ignore the ':'
			this->insertFile(repo_name, fileName);
			while (i < line.length()) {
				if (line[i] == ',' || line[i] == '\0' || line[i] == '\n') {
					if (commit_msg != "")
						this->insertCommitMsg(repo_name, commit_msg);
					commit_msg = "";
					i++; continue;
				}
				commit_msg += line[i];
				i++;
			}
			this->insertCommitMsg(repo_name, commit_msg);
		}
		file.close();
	}
	void SaveData() {
		if (Repo_Owner == "") { return; }
		fstream file;
		string file_name = Repo_Owner + "_repo.csv";

		//This part of code clears the file contents
		file.open(file_name, ios::out | ios::trunc);	//Create a new File
		file.close();
		//

		inorderTraversalSave(root);
	}
	Node searchRepo(string name) {
		return searchRepo(root, name);
	}
	Node* getRepo(string name) {
		return getRepoRecurr(root, name);
	}
	vector<string> getRepoList() {
		vector<string> temp;
		inorderTraversalRepoGrab(root, temp);
		return temp;
	}
};

