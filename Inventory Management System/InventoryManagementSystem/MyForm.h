#pragma once
#include <sstream>	//Used in String^ changeString(string str)
#include "HashTable.h"
#include "SocialGraph.h"
#include "Repository_Manager.h"

namespace TestProjectGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	HashTable UserAccounts;
	SocialGraph UserFollowGraph;
	string currUser = "";
	int currUserIndex = -1;
	string activeViewUserName = "";
	int activeViewUserIndex = -1;
	Node* Edit_Active_Repo = nullptr;
	Repository_Manager* repoMangr;
	vector<string> users_list;

	

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		ListBox^ curr_usr_repo_list;

		MyForm(void)
		{
			InitializeComponent();
			password_login->UseSystemPasswordChar = true;	//Hide Password by Default

			//Making Everything Parent of MyForm
			InitalizeGroupBox(MainMenu, true);
			InitalizeGroupBox(UserPage_Box, false);
			InitalizeGroupBox(credits_box, false);
			InitalizeGroupBox(register_acc_box, false);
			//InitalizeGroupBox(another_groupbox, false);

			this->loadingData();

			this->debugging();
			
		}
		void loadingData() {
			//Loading Data 
			UserAccounts.LoadUserNamePassWords();
			UserFollowGraph.loadData();
			int l = UserAccounts.getLength();

			if (UserFollowGraph.getTotalUsers() != UserAccounts.getUserCount()) {
				debug_label->Text = "Running Code!";

				for (int i = 0; i < l; i++) {
					string usr = UserAccounts.getUserAtIndex(i).username;
					if (usr == "-1") { continue; }
					if (UserFollowGraph.hasUser(usr) == -1) {
						UserFollowGraph.addNewUsr(usr);
					}
				}
			}

			users_list = UserFollowGraph.getUserList();

			int usercount = users_list.size();
			//if (repoMangr != nullptr) { delete[] repoMangr; }

			repoMangr = new Repository_Manager[usercount];
			for (int i = 0; i < usercount; i++) {
				string rep_owner = users_list[i];
				repoMangr[i].loadData(rep_owner);
			}
		}
		void InitalizeGroupBox(GroupBox^ gb, bool default) {
			gb->Parent = this;
			gb->SetBounds(12, 12, 796, 506);

			if (default) {
				gb->Enabled = true;
				gb->Visible = true;
			}
			else {
				gb->Enabled = false;
				gb->Visible = false;
			}

		}
		void debugging() {
			//Debugging
			TestingUserPass->Items->Clear();
			int l = UserAccounts.getLength();
			debug_label->Text = "UA: " + UserAccounts.getUserCount()
				+ "   Graph: " + UserFollowGraph.getTotalUsers();
			for (int i = 0; i < l; i++) {
				UserAcc usr = UserAccounts.getUserAtIndex(i);
				if (!usr.compUser(nullUser) &&
					!usr.compUser(delUser))
					TestingUserPass->Items->Add(changeString(usr.username + ":" + usr.password));
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^ MainMenu;
	private: System::Windows::Forms::TextBox^ password_login;
	private: System::Windows::Forms::TextBox^ user__login;
	private: System::Windows::Forms::Button^ login_btn;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::CheckBox^ checkBox1;
	private: System::Windows::Forms::ListBox^ TestingUserPass;
	private: System::Windows::Forms::Button^ register_btn;
	private: System::Windows::Forms::GroupBox^ register_acc_box;












	private: System::Windows::Forms::Button^ reg_back_btn;

	private: System::Windows::Forms::Label^ MainMenu_Login_Label;
	private: System::Windows::Forms::Label^ login_error_label;
	private: System::Windows::Forms::GroupBox^ UserPage_Box;
	private: System::Windows::Forms::Label^ usr_name_label;
	private: System::Windows::Forms::Label^ user_title_label;

	private: System::Windows::Forms::GroupBox^ UsersListBox;
	private: System::Windows::Forms::ListBox^ sel_user_list;
	private: System::Windows::Forms::Label^ debug_label;
	private: System::Windows::Forms::GroupBox^ following_list_box;
	private: System::Windows::Forms::ListBox^ following_list;
	private: System::Windows::Forms::Button^ view_follower;
private: System::Windows::Forms::Button^ quit_btn;
private: System::Windows::Forms::Label^ label_usr_cmit_list;
private: System::Windows::Forms::Label^ label_usr_files_list;
private: System::Windows::Forms::Label^ curr_viewing_label;
private: System::Windows::Forms::Button^ log_out;
private: System::Windows::Forms::Label^ label_welcomeback;
private: System::Windows::Forms::Label^ label_curr_usr_cmit_hist;
private: System::Windows::Forms::Label^ label_curr_usr_file_list;
private: System::Windows::Forms::ListBox^ curr_usr_comit_hist_list;
private: System::Windows::Forms::ListBox^ curr_usr_file_list;
private: System::Windows::Forms::Label^ label_curr_repo_list;
private: System::Windows::Forms::Button^ curr_usr_view_repo;
private: System::Windows::Forms::Label^ label_sel_repo_name_show;


private: System::Windows::Forms::TabPage^ edit_repo;
private: System::Windows::Forms::Label^ label3;
private: System::Windows::Forms::Button^ edit_remove_commit;
private: System::Windows::Forms::Label^ label_file_remove;

private: System::Windows::Forms::TextBox^ edit_repo_cmit_msg; private: System::Windows::Forms::Label^ label_cmit_msg;
private: System::Windows::Forms::Label^ label_remove_cmit_msg;

private: System::Windows::Forms::TextBox^ edit_repo_file_name;
private: System::Windows::Forms::Label^ label_edit_remove_box;
private: System::Windows::Forms::Button^ edit_remove_btn;
private: System::Windows::Forms::Label^ label_edit_remove_file;
private: System::Windows::Forms::Button^ edit_repo_remove_btn;
private: System::Windows::Forms::ListBox^ Edit_File_List;
private: System::Windows::Forms::GroupBox^ curr_usr_remove_box;

private: System::Windows::Forms::Button^ edit_repo_add_btn;

private: System::Windows::Forms::Button^ curr_usr_edit_repo;


private: System::Windows::Forms::ListBox^ usr_commit_hist_list;
private: System::Windows::Forms::ListBox^ usr_files_list;
	private: System::Windows::Forms::Button^ UnFollow;
	private: System::Windows::Forms::GroupBox^ RipHub;
	private: System::Windows::Forms::Button^ view_user;

	private: System::Windows::Forms::Button^ follow_user;
	private: System::Windows::Forms::TabControl^ View_User_Page;
	private: System::Windows::Forms::TabPage^ Current_User_Page;
	private: System::Windows::Forms::TabPage^ tabPage2;
private: System::Windows::Forms::Button^ usr_repo_copy_btn;
private: System::Windows::Forms::Button^ usr_repo_open_btn;
private: System::Windows::Forms::ListBox^ usr_repo_list;
private: System::Windows::Forms::Label^ label_usr_repo_list;
private: System::Windows::Forms::GroupBox^ curr_usr_add_box;

private: System::Windows::Forms::Label^ label_file_added_msg;

private: System::Windows::Forms::Button^ edit_add_cmit_btn;

private: System::Windows::Forms::Label^ label_add_box;
private: System::Windows::Forms::TextBox^ edit_cmit_add_msg;


private: System::Windows::Forms::Label^ label_cmit_add;
private: System::Windows::Forms::TextBox^ edit_add_file_name;
private: System::Windows::Forms::Label^ label_file_add_name;
private: System::Windows::Forms::Button^ create_repo_btn;
private: System::Windows::Forms::Label^ label_name_repo;
private: System::Windows::Forms::Button^ confirm_create;
private: System::Windows::Forms::TextBox^ create_repo_name;
private: System::Windows::Forms::GroupBox^ Create_Repo_Box;
private: System::Windows::Forms::Label^ label_repo_status;
private: System::Windows::Forms::Button^ repo_private_toggle;
private: System::Windows::Forms::Button^ show_credits_btn;
private: System::Windows::Forms::GroupBox^ credits_box;
private: System::Windows::Forms::Button^ credits_back_btn;

private: System::Windows::Forms::Label^ reg_acc_created_label;
private: System::Windows::Forms::Label^ reg_rpwd_label;
private: System::Windows::Forms::Label^ reg_pwd_label;
private: System::Windows::Forms::Label^ reg_usr_label;
private: System::Windows::Forms::TextBox^ r_rpwd_textbox;
private: System::Windows::Forms::TextBox^ r_pwd_textbox;
private: System::Windows::Forms::Label^ Register_Box_Label;
private: System::Windows::Forms::TextBox^ r_usr_txtbox;
private: System::Windows::Forms::Label^ register_error_label;
private: System::Windows::Forms::Button^ reg_newacc_btn;
private: System::Windows::Forms::Label^ label_credits_title;












	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		String^ changeString(string str) {
			String^ newStr;
			for (int i = 0; str[i] != '\0'; i++) {
				newStr += (wchar_t)str[i];
			}
			return newStr;
		}
		string changeString(String^ str) {
			stringstream ss;
			for (int i = 0; i < str->Length; i++) {
				char c = static_cast<char>(str[i]);
				ss << c;
			}
			return ss.str();
		}
		void UpdateFollowingList() {
			UserFollowGraph.saveData();
			UserFollowGraph.loadData();
			users_list = UserFollowGraph.getUserList();
			//Update Following List
			vector<string> following = UserFollowGraph.getFollowingList(currUser);
			following_list->Items->Clear();
			for (int i = 0; i < following.size(); i++) {
				if (following[i] == "") { continue; }
				following_list->Items->Add(changeString(following[i]));
			}
		}
		void UpdateUsersList() {
			//Update Users List
			sel_user_list->Items->Clear();
			for (int i = 0; i < users_list.size(); i++) {
				if (users_list[i] == "") { continue; }
				sel_user_list->Items->Add(changeString(users_list[i]));
			}
		}
		int searchUserIndex(string check_usr) {
			int user_count = UserAccounts.getUserCount();
			int i;
			for (i = 0; i < user_count; i++) {
				if (check_usr == users_list[i])
					return i;
			}
			if (i == user_count) { return -1; }
			return -1;
		}
		void UpdateUsrRepoList(string check_usr) {
			int i = searchUserIndex(check_usr);
			usr_repo_list->Items->Clear();
			usr_files_list->Items->Clear();
			usr_commit_hist_list->Items->Clear();
			if (i == -1) {
				activeViewUserName = "";
				activeViewUserIndex = -1;
				return;
			}

			activeViewUserName = check_usr;
			activeViewUserIndex = i;

			//User Index is i
			vector<string> repoList = repoMangr[i].getRepoList();

			for (int j = 0; j < repoList.size(); j++) {
				usr_repo_list->Items->Add(changeString(repoList[j]));
			}
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->MainMenu = (gcnew System::Windows::Forms::GroupBox());
			this->show_credits_btn = (gcnew System::Windows::Forms::Button());
			this->quit_btn = (gcnew System::Windows::Forms::Button());
			this->debug_label = (gcnew System::Windows::Forms::Label());
			this->login_error_label = (gcnew System::Windows::Forms::Label());
			this->MainMenu_Login_Label = (gcnew System::Windows::Forms::Label());
			this->register_btn = (gcnew System::Windows::Forms::Button());
			this->TestingUserPass = (gcnew System::Windows::Forms::ListBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->password_login = (gcnew System::Windows::Forms::TextBox());
			this->user__login = (gcnew System::Windows::Forms::TextBox());
			this->login_btn = (gcnew System::Windows::Forms::Button());
			this->register_acc_box = (gcnew System::Windows::Forms::GroupBox());
			this->credits_box = (gcnew System::Windows::Forms::GroupBox());
			this->credits_back_btn = (gcnew System::Windows::Forms::Button());
			this->reg_acc_created_label = (gcnew System::Windows::Forms::Label());
			this->reg_back_btn = (gcnew System::Windows::Forms::Button());
			this->reg_rpwd_label = (gcnew System::Windows::Forms::Label());
			this->reg_pwd_label = (gcnew System::Windows::Forms::Label());
			this->reg_usr_label = (gcnew System::Windows::Forms::Label());
			this->r_rpwd_textbox = (gcnew System::Windows::Forms::TextBox());
			this->r_pwd_textbox = (gcnew System::Windows::Forms::TextBox());
			this->Register_Box_Label = (gcnew System::Windows::Forms::Label());
			this->r_usr_txtbox = (gcnew System::Windows::Forms::TextBox());
			this->register_error_label = (gcnew System::Windows::Forms::Label());
			this->reg_newacc_btn = (gcnew System::Windows::Forms::Button());
			this->UserPage_Box = (gcnew System::Windows::Forms::GroupBox());
			this->log_out = (gcnew System::Windows::Forms::Button());
			this->view_user = (gcnew System::Windows::Forms::Button());
			this->follow_user = (gcnew System::Windows::Forms::Button());
			this->view_follower = (gcnew System::Windows::Forms::Button());
			this->UnFollow = (gcnew System::Windows::Forms::Button());
			this->RipHub = (gcnew System::Windows::Forms::GroupBox());
			this->View_User_Page = (gcnew System::Windows::Forms::TabControl());
			this->Current_User_Page = (gcnew System::Windows::Forms::TabPage());
			this->Create_Repo_Box = (gcnew System::Windows::Forms::GroupBox());
			this->create_repo_name = (gcnew System::Windows::Forms::TextBox());
			this->label_name_repo = (gcnew System::Windows::Forms::Label());
			this->confirm_create = (gcnew System::Windows::Forms::Button());
			this->create_repo_btn = (gcnew System::Windows::Forms::Button());
			this->curr_usr_edit_repo = (gcnew System::Windows::Forms::Button());
			this->label_curr_usr_cmit_hist = (gcnew System::Windows::Forms::Label());
			this->label_curr_usr_file_list = (gcnew System::Windows::Forms::Label());
			this->curr_usr_comit_hist_list = (gcnew System::Windows::Forms::ListBox());
			this->curr_usr_file_list = (gcnew System::Windows::Forms::ListBox());
			this->label_curr_repo_list = (gcnew System::Windows::Forms::Label());
			this->curr_usr_view_repo = (gcnew System::Windows::Forms::Button());
			this->curr_usr_repo_list = (gcnew System::Windows::Forms::ListBox());
			this->label_welcomeback = (gcnew System::Windows::Forms::Label());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->curr_viewing_label = (gcnew System::Windows::Forms::Label());
			this->label_usr_cmit_list = (gcnew System::Windows::Forms::Label());
			this->label_usr_files_list = (gcnew System::Windows::Forms::Label());
			this->usr_commit_hist_list = (gcnew System::Windows::Forms::ListBox());
			this->usr_files_list = (gcnew System::Windows::Forms::ListBox());
			this->label_usr_repo_list = (gcnew System::Windows::Forms::Label());
			this->usr_repo_copy_btn = (gcnew System::Windows::Forms::Button());
			this->usr_repo_open_btn = (gcnew System::Windows::Forms::Button());
			this->usr_repo_list = (gcnew System::Windows::Forms::ListBox());
			this->edit_repo = (gcnew System::Windows::Forms::TabPage());
			this->label_repo_status = (gcnew System::Windows::Forms::Label());
			this->repo_private_toggle = (gcnew System::Windows::Forms::Button());
			this->curr_usr_add_box = (gcnew System::Windows::Forms::GroupBox());
			this->label_file_added_msg = (gcnew System::Windows::Forms::Label());
			this->edit_add_cmit_btn = (gcnew System::Windows::Forms::Button());
			this->label_add_box = (gcnew System::Windows::Forms::Label());
			this->edit_cmit_add_msg = (gcnew System::Windows::Forms::TextBox());
			this->label_cmit_add = (gcnew System::Windows::Forms::Label());
			this->edit_add_file_name = (gcnew System::Windows::Forms::TextBox());
			this->label_file_add_name = (gcnew System::Windows::Forms::Label());
			this->curr_usr_remove_box = (gcnew System::Windows::Forms::GroupBox());
			this->label_file_remove = (gcnew System::Windows::Forms::Label());
			this->edit_remove_commit = (gcnew System::Windows::Forms::Button());
			this->label_edit_remove_box = (gcnew System::Windows::Forms::Label());
			this->edit_repo_cmit_msg = (gcnew System::Windows::Forms::TextBox());
			this->label_remove_cmit_msg = (gcnew System::Windows::Forms::Label());
			this->edit_repo_file_name = (gcnew System::Windows::Forms::TextBox());
			this->label_edit_remove_file = (gcnew System::Windows::Forms::Label());
			this->edit_repo_remove_btn = (gcnew System::Windows::Forms::Button());
			this->edit_repo_add_btn = (gcnew System::Windows::Forms::Button());
			this->Edit_File_List = (gcnew System::Windows::Forms::ListBox());
			this->label_sel_repo_name_show = (gcnew System::Windows::Forms::Label());
			this->following_list_box = (gcnew System::Windows::Forms::GroupBox());
			this->following_list = (gcnew System::Windows::Forms::ListBox());
			this->UsersListBox = (gcnew System::Windows::Forms::GroupBox());
			this->sel_user_list = (gcnew System::Windows::Forms::ListBox());
			this->usr_name_label = (gcnew System::Windows::Forms::Label());
			this->user_title_label = (gcnew System::Windows::Forms::Label());
			this->label_credits_title = (gcnew System::Windows::Forms::Label());
			this->MainMenu->SuspendLayout();
			this->register_acc_box->SuspendLayout();
			this->credits_box->SuspendLayout();
			this->UserPage_Box->SuspendLayout();
			this->RipHub->SuspendLayout();
			this->View_User_Page->SuspendLayout();
			this->Current_User_Page->SuspendLayout();
			this->Create_Repo_Box->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->edit_repo->SuspendLayout();
			this->curr_usr_add_box->SuspendLayout();
			this->curr_usr_remove_box->SuspendLayout();
			this->following_list_box->SuspendLayout();
			this->UsersListBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// MainMenu
			// 
			this->MainMenu->Controls->Add(this->show_credits_btn);
			this->MainMenu->Controls->Add(this->quit_btn);
			this->MainMenu->Controls->Add(this->debug_label);
			this->MainMenu->Controls->Add(this->login_error_label);
			this->MainMenu->Controls->Add(this->MainMenu_Login_Label);
			this->MainMenu->Controls->Add(this->register_btn);
			this->MainMenu->Controls->Add(this->TestingUserPass);
			this->MainMenu->Controls->Add(this->checkBox1);
			this->MainMenu->Controls->Add(this->label2);
			this->MainMenu->Controls->Add(this->label1);
			this->MainMenu->Controls->Add(this->password_login);
			this->MainMenu->Controls->Add(this->user__login);
			this->MainMenu->Controls->Add(this->login_btn);
			this->MainMenu->Location = System::Drawing::Point(12, 12);
			this->MainMenu->Name = L"MainMenu";
			this->MainMenu->Size = System::Drawing::Size(796, 506);
			this->MainMenu->TabIndex = 0;
			this->MainMenu->TabStop = false;
			this->MainMenu->Enter += gcnew System::EventHandler(this, &MyForm::MainMenu_Enter);
			// 
			// show_credits_btn
			// 
			this->show_credits_btn->Location = System::Drawing::Point(331, 371);
			this->show_credits_btn->Name = L"show_credits_btn";
			this->show_credits_btn->Size = System::Drawing::Size(159, 23);
			this->show_credits_btn->TabIndex = 8;
			this->show_credits_btn->Text = L"Credits";
			this->show_credits_btn->UseVisualStyleBackColor = true;
			this->show_credits_btn->Click += gcnew System::EventHandler(this, &MyForm::show_credits_btn_Click);
			// 
			// quit_btn
			// 
			this->quit_btn->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->quit_btn->Location = System::Drawing::Point(331, 410);
			this->quit_btn->Name = L"quit_btn";
			this->quit_btn->Size = System::Drawing::Size(159, 46);
			this->quit_btn->TabIndex = 11;
			this->quit_btn->Text = L"Quit";
			this->quit_btn->UseVisualStyleBackColor = true;
			this->quit_btn->Click += gcnew System::EventHandler(this, &MyForm::quit_btn_Click);
			// 
			// debug_label
			// 
			this->debug_label->AutoSize = true;
			this->debug_label->Location = System::Drawing::Point(19, 359);
			this->debug_label->Name = L"debug_label";
			this->debug_label->Size = System::Drawing::Size(35, 13);
			this->debug_label->TabIndex = 10;
			this->debug_label->Text = L"label3";
			// 
			// login_error_label
			// 
			this->login_error_label->AutoSize = true;
			this->login_error_label->Enabled = false;
			this->login_error_label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->login_error_label->ForeColor = System::Drawing::Color::Red;
			this->login_error_label->Location = System::Drawing::Point(212, 233);
			this->login_error_label->Name = L"login_error_label";
			this->login_error_label->Size = System::Drawing::Size(397, 20);
			this->login_error_label->TabIndex = 9;
			this->login_error_label->Text = L"Username And Passwords combination does not exists";
			this->login_error_label->Visible = false;
			// 
			// MainMenu_Login_Label
			// 
			this->MainMenu_Login_Label->AutoSize = true;
			this->MainMenu_Login_Label->Font = (gcnew System::Drawing::Font(L"Times New Roman", 26.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->MainMenu_Login_Label->Location = System::Drawing::Point(236, 71);
			this->MainMenu_Login_Label->Name = L"MainMenu_Login_Label";
			this->MainMenu_Login_Label->Size = System::Drawing::Size(349, 40);
			this->MainMenu_Login_Label->TabIndex = 8;
			this->MainMenu_Login_Label->Text = L"Login into an Account";
			// 
			// register_btn
			// 
			this->register_btn->Location = System::Drawing::Point(331, 332);
			this->register_btn->Name = L"register_btn";
			this->register_btn->Size = System::Drawing::Size(159, 23);
			this->register_btn->TabIndex = 7;
			this->register_btn->Text = L"Register New Account";
			this->register_btn->UseVisualStyleBackColor = true;
			this->register_btn->Click += gcnew System::EventHandler(this, &MyForm::register_btn_Click);
			// 
			// TestingUserPass
			// 
			this->TestingUserPass->FormattingEnabled = true;
			this->TestingUserPass->Location = System::Drawing::Point(12, 378);
			this->TestingUserPass->Name = L"TestingUserPass";
			this->TestingUserPass->Size = System::Drawing::Size(120, 95);
			this->TestingUserPass->TabIndex = 6;
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(514, 200);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(51, 17);
			this->checkBox1->TabIndex = 3;
			this->checkBox1->Text = L"show";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::checkBox1_CheckedChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(229, 200);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(76, 16);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Password:  ";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(229, 173);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(76, 16);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Username: ";
			// 
			// password_login
			// 
			this->password_login->Location = System::Drawing::Point(313, 196);
			this->password_login->Name = L"password_login";
			this->password_login->Size = System::Drawing::Size(194, 20);
			this->password_login->TabIndex = 2;
			// 
			// user__login
			// 
			this->user__login->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->user__login->Location = System::Drawing::Point(313, 170);
			this->user__login->Name = L"user__login";
			this->user__login->Size = System::Drawing::Size(194, 22);
			this->user__login->TabIndex = 1;
			// 
			// login_btn
			// 
			this->login_btn->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->login_btn->Location = System::Drawing::Point(331, 270);
			this->login_btn->Name = L"login_btn";
			this->login_btn->Size = System::Drawing::Size(159, 46);
			this->login_btn->TabIndex = 4;
			this->login_btn->Text = L"Login";
			this->login_btn->UseVisualStyleBackColor = true;
			this->login_btn->Click += gcnew System::EventHandler(this, &MyForm::login_btn_Click);
			// 
			// register_acc_box
			// 
			this->register_acc_box->Controls->Add(this->credits_box);
			this->register_acc_box->Controls->Add(this->reg_acc_created_label);
			this->register_acc_box->Controls->Add(this->reg_back_btn);
			this->register_acc_box->Controls->Add(this->reg_rpwd_label);
			this->register_acc_box->Controls->Add(this->reg_pwd_label);
			this->register_acc_box->Controls->Add(this->reg_usr_label);
			this->register_acc_box->Controls->Add(this->r_rpwd_textbox);
			this->register_acc_box->Controls->Add(this->r_pwd_textbox);
			this->register_acc_box->Controls->Add(this->Register_Box_Label);
			this->register_acc_box->Controls->Add(this->r_usr_txtbox);
			this->register_acc_box->Controls->Add(this->register_error_label);
			this->register_acc_box->Controls->Add(this->reg_newacc_btn);
			this->register_acc_box->Enabled = false;
			this->register_acc_box->Location = System::Drawing::Point(12, 6);
			this->register_acc_box->Name = L"register_acc_box";
			this->register_acc_box->Size = System::Drawing::Size(796, 506);
			this->register_acc_box->TabIndex = 1;
			this->register_acc_box->TabStop = false;
			this->register_acc_box->Visible = false;
			// 
			// credits_box
			// 
			this->credits_box->Controls->Add(this->label_credits_title);
			this->credits_box->Controls->Add(this->credits_back_btn);
			this->credits_box->Enabled = false;
			this->credits_box->Location = System::Drawing::Point(0, 6);
			this->credits_box->Name = L"credits_box";
			this->credits_box->Size = System::Drawing::Size(796, 506);
			this->credits_box->TabIndex = 10;
			this->credits_box->TabStop = false;
			this->credits_box->Visible = false;
			// 
			// credits_back_btn
			// 
			this->credits_back_btn->Location = System::Drawing::Point(652, 471);
			this->credits_back_btn->Name = L"credits_back_btn";
			this->credits_back_btn->Size = System::Drawing::Size(138, 29);
			this->credits_back_btn->TabIndex = 2;
			this->credits_back_btn->Text = L"<==   Back";
			this->credits_back_btn->UseVisualStyleBackColor = true;
			this->credits_back_btn->Click += gcnew System::EventHandler(this, &MyForm::credits_back_btn_Click);
			// 
			// reg_acc_created_label
			// 
			this->reg_acc_created_label->AutoSize = true;
			this->reg_acc_created_label->Enabled = false;
			this->reg_acc_created_label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->reg_acc_created_label->ForeColor = System::Drawing::Color::Green;
			this->reg_acc_created_label->Location = System::Drawing::Point(301, 150);
			this->reg_acc_created_label->Name = L"reg_acc_created_label";
			this->reg_acc_created_label->Size = System::Drawing::Size(194, 25);
			this->reg_acc_created_label->TabIndex = 9;
			this->reg_acc_created_label->Text = L"Account Created!";
			this->reg_acc_created_label->Visible = false;
			// 
			// reg_back_btn
			// 
			this->reg_back_btn->Location = System::Drawing::Point(652, 471);
			this->reg_back_btn->Name = L"reg_back_btn";
			this->reg_back_btn->Size = System::Drawing::Size(138, 29);
			this->reg_back_btn->TabIndex = 2;
			this->reg_back_btn->Text = L"<==   Back";
			this->reg_back_btn->UseVisualStyleBackColor = true;
			this->reg_back_btn->Click += gcnew System::EventHandler(this, &MyForm::reg_back_btn_Click);
			// 
			// reg_rpwd_label
			// 
			this->reg_rpwd_label->AutoSize = true;
			this->reg_rpwd_label->Location = System::Drawing::Point(287, 255);
			this->reg_rpwd_label->Name = L"reg_rpwd_label";
			this->reg_rpwd_label->Size = System::Drawing::Size(97, 13);
			this->reg_rpwd_label->TabIndex = 8;
			this->reg_rpwd_label->Text = L"Repeat Password: ";
			// 
			// reg_pwd_label
			// 
			this->reg_pwd_label->AutoSize = true;
			this->reg_pwd_label->Location = System::Drawing::Point(287, 229);
			this->reg_pwd_label->Name = L"reg_pwd_label";
			this->reg_pwd_label->Size = System::Drawing::Size(87, 13);
			this->reg_pwd_label->TabIndex = 7;
			this->reg_pwd_label->Text = L"Enter Password: ";
			// 
			// reg_usr_label
			// 
			this->reg_usr_label->AutoSize = true;
			this->reg_usr_label->Location = System::Drawing::Point(287, 204);
			this->reg_usr_label->Name = L"reg_usr_label";
			this->reg_usr_label->Size = System::Drawing::Size(91, 13);
			this->reg_usr_label->TabIndex = 6;
			this->reg_usr_label->Text = L"Enter UserName: ";
			// 
			// r_rpwd_textbox
			// 
			this->r_rpwd_textbox->Location = System::Drawing::Point(384, 252);
			this->r_rpwd_textbox->Name = L"r_rpwd_textbox";
			this->r_rpwd_textbox->Size = System::Drawing::Size(131, 20);
			this->r_rpwd_textbox->TabIndex = 5;
			// 
			// r_pwd_textbox
			// 
			this->r_pwd_textbox->Location = System::Drawing::Point(384, 226);
			this->r_pwd_textbox->Name = L"r_pwd_textbox";
			this->r_pwd_textbox->Size = System::Drawing::Size(131, 20);
			this->r_pwd_textbox->TabIndex = 4;
			// 
			// Register_Box_Label
			// 
			this->Register_Box_Label->AutoSize = true;
			this->Register_Box_Label->Font = (gcnew System::Drawing::Font(L"Times New Roman", 26.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Register_Box_Label->Location = System::Drawing::Point(209, 76);
			this->Register_Box_Label->Name = L"Register_Box_Label";
			this->Register_Box_Label->Size = System::Drawing::Size(378, 40);
			this->Register_Box_Label->TabIndex = 3;
			this->Register_Box_Label->Text = L"Register A New Account";
			// 
			// r_usr_txtbox
			// 
			this->r_usr_txtbox->Location = System::Drawing::Point(384, 200);
			this->r_usr_txtbox->Name = L"r_usr_txtbox";
			this->r_usr_txtbox->Size = System::Drawing::Size(131, 20);
			this->r_usr_txtbox->TabIndex = 2;
			// 
			// register_error_label
			// 
			this->register_error_label->AutoSize = true;
			this->register_error_label->ForeColor = System::Drawing::Color::Red;
			this->register_error_label->Location = System::Drawing::Point(350, 384);
			this->register_error_label->Name = L"register_error_label";
			this->register_error_label->Size = System::Drawing::Size(96, 13);
			this->register_error_label->TabIndex = 1;
			this->register_error_label->Text = L"register_error_label";
			this->register_error_label->Visible = false;
			// 
			// reg_newacc_btn
			// 
			this->reg_newacc_btn->Location = System::Drawing::Point(316, 322);
			this->reg_newacc_btn->Name = L"reg_newacc_btn";
			this->reg_newacc_btn->Size = System::Drawing::Size(165, 41);
			this->reg_newacc_btn->TabIndex = 0;
			this->reg_newacc_btn->Text = L"Register";
			this->reg_newacc_btn->UseVisualStyleBackColor = true;
			this->reg_newacc_btn->Click += gcnew System::EventHandler(this, &MyForm::reg_newacc_btn_Click);
			// 
			// UserPage_Box
			// 
			this->UserPage_Box->Controls->Add(this->log_out);
			this->UserPage_Box->Controls->Add(this->view_user);
			this->UserPage_Box->Controls->Add(this->follow_user);
			this->UserPage_Box->Controls->Add(this->view_follower);
			this->UserPage_Box->Controls->Add(this->UnFollow);
			this->UserPage_Box->Controls->Add(this->RipHub);
			this->UserPage_Box->Controls->Add(this->following_list_box);
			this->UserPage_Box->Controls->Add(this->UsersListBox);
			this->UserPage_Box->Controls->Add(this->usr_name_label);
			this->UserPage_Box->Controls->Add(this->user_title_label);
			this->UserPage_Box->Location = System::Drawing::Point(12, 12);
			this->UserPage_Box->Name = L"UserPage_Box";
			this->UserPage_Box->Size = System::Drawing::Size(796, 506);
			this->UserPage_Box->TabIndex = 1;
			this->UserPage_Box->TabStop = false;
			// 
			// log_out
			// 
			this->log_out->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->log_out->Location = System::Drawing::Point(22, 20);
			this->log_out->Name = L"log_out";
			this->log_out->Size = System::Drawing::Size(110, 39);
			this->log_out->TabIndex = 11;
			this->log_out->Text = L"Log out";
			this->log_out->UseVisualStyleBackColor = true;
			this->log_out->Click += gcnew System::EventHandler(this, &MyForm::log_out_Click);
			// 
			// view_user
			// 
			this->view_user->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->view_user->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->view_user->Location = System::Drawing::Point(124, 468);
			this->view_user->Name = L"view_user";
			this->view_user->Size = System::Drawing::Size(96, 24);
			this->view_user->TabIndex = 10;
			this->view_user->Text = L"View";
			this->view_user->UseVisualStyleBackColor = false;
			this->view_user->Click += gcnew System::EventHandler(this, &MyForm::view_user_Click);
			// 
			// follow_user
			// 
			this->follow_user->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->follow_user->Location = System::Drawing::Point(15, 468);
			this->follow_user->Name = L"follow_user";
			this->follow_user->Size = System::Drawing::Size(96, 24);
			this->follow_user->TabIndex = 9;
			this->follow_user->Text = L"Follow";
			this->follow_user->UseVisualStyleBackColor = true;
			this->follow_user->Click += gcnew System::EventHandler(this, &MyForm::follow_user_Click);
			// 
			// view_follower
			// 
			this->view_follower->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->view_follower->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->view_follower->Location = System::Drawing::Point(124, 278);
			this->view_follower->Name = L"view_follower";
			this->view_follower->Size = System::Drawing::Size(96, 24);
			this->view_follower->TabIndex = 8;
			this->view_follower->Text = L"View";
			this->view_follower->UseVisualStyleBackColor = false;
			this->view_follower->Click += gcnew System::EventHandler(this, &MyForm::view_follower_Click);
			// 
			// UnFollow
			// 
			this->UnFollow->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->UnFollow->Location = System::Drawing::Point(15, 278);
			this->UnFollow->Name = L"UnFollow";
			this->UnFollow->Size = System::Drawing::Size(96, 24);
			this->UnFollow->TabIndex = 7;
			this->UnFollow->Text = L"Unfollow";
			this->UnFollow->UseVisualStyleBackColor = true;
			this->UnFollow->Click += gcnew System::EventHandler(this, &MyForm::UnFollow_Click);
			// 
			// RipHub
			// 
			this->RipHub->Controls->Add(this->View_User_Page);
			this->RipHub->Location = System::Drawing::Point(232, 102);
			this->RipHub->Name = L"RipHub";
			this->RipHub->Size = System::Drawing::Size(548, 387);
			this->RipHub->TabIndex = 6;
			this->RipHub->TabStop = false;
			// 
			// View_User_Page
			// 
			this->View_User_Page->Alignment = System::Windows::Forms::TabAlignment::Bottom;
			this->View_User_Page->Controls->Add(this->Current_User_Page);
			this->View_User_Page->Controls->Add(this->tabPage2);
			this->View_User_Page->Controls->Add(this->edit_repo);
			this->View_User_Page->Dock = System::Windows::Forms::DockStyle::Fill;
			this->View_User_Page->Location = System::Drawing::Point(3, 16);
			this->View_User_Page->Multiline = true;
			this->View_User_Page->Name = L"View_User_Page";
			this->View_User_Page->SelectedIndex = 0;
			this->View_User_Page->Size = System::Drawing::Size(542, 368);
			this->View_User_Page->TabIndex = 0;
			// 
			// Current_User_Page
			// 
			this->Current_User_Page->Controls->Add(this->Create_Repo_Box);
			this->Current_User_Page->Controls->Add(this->create_repo_btn);
			this->Current_User_Page->Controls->Add(this->curr_usr_edit_repo);
			this->Current_User_Page->Controls->Add(this->label_curr_usr_cmit_hist);
			this->Current_User_Page->Controls->Add(this->label_curr_usr_file_list);
			this->Current_User_Page->Controls->Add(this->curr_usr_comit_hist_list);
			this->Current_User_Page->Controls->Add(this->curr_usr_file_list);
			this->Current_User_Page->Controls->Add(this->label_curr_repo_list);
			this->Current_User_Page->Controls->Add(this->curr_usr_view_repo);
			this->Current_User_Page->Controls->Add(this->curr_usr_repo_list);
			this->Current_User_Page->Controls->Add(this->label_welcomeback);
			this->Current_User_Page->Location = System::Drawing::Point(4, 4);
			this->Current_User_Page->Name = L"Current_User_Page";
			this->Current_User_Page->Padding = System::Windows::Forms::Padding(3);
			this->Current_User_Page->Size = System::Drawing::Size(534, 342);
			this->Current_User_Page->TabIndex = 0;
			this->Current_User_Page->Text = L"Home_Page";
			this->Current_User_Page->UseVisualStyleBackColor = true;
			// 
			// Create_Repo_Box
			// 
			this->Create_Repo_Box->Controls->Add(this->create_repo_name);
			this->Create_Repo_Box->Controls->Add(this->label_name_repo);
			this->Create_Repo_Box->Controls->Add(this->confirm_create);
			this->Create_Repo_Box->Enabled = false;
			this->Create_Repo_Box->Location = System::Drawing::Point(215, 255);
			this->Create_Repo_Box->Name = L"Create_Repo_Box";
			this->Create_Repo_Box->Size = System::Drawing::Size(277, 59);
			this->Create_Repo_Box->TabIndex = 22;
			this->Create_Repo_Box->TabStop = false;
			this->Create_Repo_Box->Visible = false;
			// 
			// create_repo_name
			// 
			this->create_repo_name->BackColor = System::Drawing::Color::LightGray;
			this->create_repo_name->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->create_repo_name->Location = System::Drawing::Point(9, 29);
			this->create_repo_name->Name = L"create_repo_name";
			this->create_repo_name->Size = System::Drawing::Size(170, 20);
			this->create_repo_name->TabIndex = 19;
			// 
			// label_name_repo
			// 
			this->label_name_repo->AutoSize = true;
			this->label_name_repo->Location = System::Drawing::Point(6, 13);
			this->label_name_repo->Name = L"label_name_repo";
			this->label_name_repo->Size = System::Drawing::Size(91, 13);
			this->label_name_repo->TabIndex = 21;
			this->label_name_repo->Text = L"Name Repository:";
			// 
			// confirm_create
			// 
			this->confirm_create->Location = System::Drawing::Point(185, 26);
			this->confirm_create->Name = L"confirm_create";
			this->confirm_create->Size = System::Drawing::Size(75, 23);
			this->confirm_create->TabIndex = 20;
			this->confirm_create->Text = L"Create!";
			this->confirm_create->UseVisualStyleBackColor = true;
			this->confirm_create->Click += gcnew System::EventHandler(this, &MyForm::confirm_create_Click);
			// 
			// create_repo_btn
			// 
			this->create_repo_btn->BackColor = System::Drawing::Color::DarkTurquoise;
			this->create_repo_btn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->create_repo_btn->ForeColor = System::Drawing::SystemColors::ControlText;
			this->create_repo_btn->Location = System::Drawing::Point(24, 278);
			this->create_repo_btn->Name = L"create_repo_btn";
			this->create_repo_btn->Size = System::Drawing::Size(142, 23);
			this->create_repo_btn->TabIndex = 18;
			this->create_repo_btn->Text = L"Create new Repo";
			this->create_repo_btn->UseVisualStyleBackColor = false;
			this->create_repo_btn->Click += gcnew System::EventHandler(this, &MyForm::create_repo_btn_Click);
			// 
			// curr_usr_edit_repo
			// 
			this->curr_usr_edit_repo->BackColor = System::Drawing::Color::DarkTurquoise;
			this->curr_usr_edit_repo->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->curr_usr_edit_repo->ForeColor = System::Drawing::SystemColors::ControlText;
			this->curr_usr_edit_repo->Location = System::Drawing::Point(98, 242);
			this->curr_usr_edit_repo->Name = L"curr_usr_edit_repo";
			this->curr_usr_edit_repo->Size = System::Drawing::Size(68, 23);
			this->curr_usr_edit_repo->TabIndex = 17;
			this->curr_usr_edit_repo->Text = L"Edit";
			this->curr_usr_edit_repo->UseVisualStyleBackColor = false;
			this->curr_usr_edit_repo->Click += gcnew System::EventHandler(this, &MyForm::curr_usr_edit_repo_Click);
			// 
			// label_curr_usr_cmit_hist
			// 
			this->label_curr_usr_cmit_hist->AutoSize = true;
			this->label_curr_usr_cmit_hist->Location = System::Drawing::Point(370, 59);
			this->label_curr_usr_cmit_hist->Name = L"label_curr_usr_cmit_hist";
			this->label_curr_usr_cmit_hist->Size = System::Drawing::Size(87, 13);
			this->label_curr_usr_cmit_hist->TabIndex = 16;
			this->label_curr_usr_cmit_hist->Text = L"Commit\'s Hostory";
			// 
			// label_curr_usr_file_list
			// 
			this->label_curr_usr_file_list->AutoSize = true;
			this->label_curr_usr_file_list->Location = System::Drawing::Point(197, 59);
			this->label_curr_usr_file_list->Name = L"label_curr_usr_file_list";
			this->label_curr_usr_file_list->Size = System::Drawing::Size(59, 13);
			this->label_curr_usr_file_list->TabIndex = 15;
			this->label_curr_usr_file_list->Text = L"List of Files";
			// 
			// curr_usr_comit_hist_list
			// 
			this->curr_usr_comit_hist_list->FormattingEnabled = true;
			this->curr_usr_comit_hist_list->Location = System::Drawing::Point(370, 77);
			this->curr_usr_comit_hist_list->Name = L"curr_usr_comit_hist_list";
			this->curr_usr_comit_hist_list->Size = System::Drawing::Size(141, 160);
			this->curr_usr_comit_hist_list->TabIndex = 14;
			// 
			// curr_usr_file_list
			// 
			this->curr_usr_file_list->FormattingEnabled = true;
			this->curr_usr_file_list->Location = System::Drawing::Point(197, 77);
			this->curr_usr_file_list->Name = L"curr_usr_file_list";
			this->curr_usr_file_list->Size = System::Drawing::Size(141, 160);
			this->curr_usr_file_list->TabIndex = 13;
			// 
			// label_curr_repo_list
			// 
			this->label_curr_repo_list->AutoSize = true;
			this->label_curr_repo_list->Location = System::Drawing::Point(24, 59);
			this->label_curr_repo_list->Name = L"label_curr_repo_list";
			this->label_curr_repo_list->Size = System::Drawing::Size(96, 13);
			this->label_curr_repo_list->TabIndex = 12;
			this->label_curr_repo_list->Text = L"List of User\'s Repo";
			// 
			// curr_usr_view_repo
			// 
			this->curr_usr_view_repo->BackColor = System::Drawing::Color::DarkTurquoise;
			this->curr_usr_view_repo->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->curr_usr_view_repo->ForeColor = System::Drawing::SystemColors::ControlText;
			this->curr_usr_view_repo->Location = System::Drawing::Point(24, 242);
			this->curr_usr_view_repo->Name = L"curr_usr_view_repo";
			this->curr_usr_view_repo->Size = System::Drawing::Size(68, 23);
			this->curr_usr_view_repo->TabIndex = 10;
			this->curr_usr_view_repo->Text = L"View";
			this->curr_usr_view_repo->UseVisualStyleBackColor = false;
			this->curr_usr_view_repo->Click += gcnew System::EventHandler(this, &MyForm::curr_usr_view_repo_Click);
			// 
			// curr_usr_repo_list
			// 
			this->curr_usr_repo_list->BackColor = System::Drawing::Color::SkyBlue;
			this->curr_usr_repo_list->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->curr_usr_repo_list->FormattingEnabled = true;
			this->curr_usr_repo_list->Location = System::Drawing::Point(24, 78);
			this->curr_usr_repo_list->Name = L"curr_usr_repo_list";
			this->curr_usr_repo_list->Size = System::Drawing::Size(141, 158);
			this->curr_usr_repo_list->TabIndex = 9;
			// 
			// label_welcomeback
			// 
			this->label_welcomeback->AutoSize = true;
			this->label_welcomeback->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label_welcomeback->Location = System::Drawing::Point(18, 21);
			this->label_welcomeback->Name = L"label_welcomeback";
			this->label_welcomeback->Size = System::Drawing::Size(195, 24);
			this->label_welcomeback->TabIndex = 0;
			this->label_welcomeback->Text = L"Welcome Back, <usr>";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->curr_viewing_label);
			this->tabPage2->Controls->Add(this->label_usr_cmit_list);
			this->tabPage2->Controls->Add(this->label_usr_files_list);
			this->tabPage2->Controls->Add(this->usr_commit_hist_list);
			this->tabPage2->Controls->Add(this->usr_files_list);
			this->tabPage2->Controls->Add(this->label_usr_repo_list);
			this->tabPage2->Controls->Add(this->usr_repo_copy_btn);
			this->tabPage2->Controls->Add(this->usr_repo_open_btn);
			this->tabPage2->Controls->Add(this->usr_repo_list);
			this->tabPage2->Location = System::Drawing::Point(4, 4);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(534, 342);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"View User";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// curr_viewing_label
			// 
			this->curr_viewing_label->AutoSize = true;
			this->curr_viewing_label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->curr_viewing_label->Location = System::Drawing::Point(14, 15);
			this->curr_viewing_label->Name = L"curr_viewing_label";
			this->curr_viewing_label->Size = System::Drawing::Size(168, 24);
			this->curr_viewing_label->TabIndex = 9;
			this->curr_viewing_label->Text = L"Currently Viewing: ";
			// 
			// label_usr_cmit_list
			// 
			this->label_usr_cmit_list->AutoSize = true;
			this->label_usr_cmit_list->Location = System::Drawing::Point(364, 64);
			this->label_usr_cmit_list->Name = L"label_usr_cmit_list";
			this->label_usr_cmit_list->Size = System::Drawing::Size(87, 13);
			this->label_usr_cmit_list->TabIndex = 8;
			this->label_usr_cmit_list->Text = L"Commit\'s Hostory";
			// 
			// label_usr_files_list
			// 
			this->label_usr_files_list->AutoSize = true;
			this->label_usr_files_list->Location = System::Drawing::Point(191, 64);
			this->label_usr_files_list->Name = L"label_usr_files_list";
			this->label_usr_files_list->Size = System::Drawing::Size(59, 13);
			this->label_usr_files_list->TabIndex = 7;
			this->label_usr_files_list->Text = L"List of Files";
			// 
			// usr_commit_hist_list
			// 
			this->usr_commit_hist_list->FormattingEnabled = true;
			this->usr_commit_hist_list->Location = System::Drawing::Point(364, 82);
			this->usr_commit_hist_list->Name = L"usr_commit_hist_list";
			this->usr_commit_hist_list->Size = System::Drawing::Size(141, 160);
			this->usr_commit_hist_list->TabIndex = 6;
			// 
			// usr_files_list
			// 
			this->usr_files_list->FormattingEnabled = true;
			this->usr_files_list->Location = System::Drawing::Point(191, 82);
			this->usr_files_list->Name = L"usr_files_list";
			this->usr_files_list->Size = System::Drawing::Size(141, 160);
			this->usr_files_list->TabIndex = 5;
			// 
			// label_usr_repo_list
			// 
			this->label_usr_repo_list->AutoSize = true;
			this->label_usr_repo_list->Location = System::Drawing::Point(18, 64);
			this->label_usr_repo_list->Name = L"label_usr_repo_list";
			this->label_usr_repo_list->Size = System::Drawing::Size(96, 13);
			this->label_usr_repo_list->TabIndex = 4;
			this->label_usr_repo_list->Text = L"List of User\'s Repo";
			// 
			// usr_repo_copy_btn
			// 
			this->usr_repo_copy_btn->BackColor = System::Drawing::Color::DarkTurquoise;
			this->usr_repo_copy_btn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->usr_repo_copy_btn->ForeColor = System::Drawing::SystemColors::ControlText;
			this->usr_repo_copy_btn->Location = System::Drawing::Point(91, 247);
			this->usr_repo_copy_btn->Name = L"usr_repo_copy_btn";
			this->usr_repo_copy_btn->Size = System::Drawing::Size(68, 23);
			this->usr_repo_copy_btn->TabIndex = 3;
			this->usr_repo_copy_btn->Text = L"Copy";
			this->usr_repo_copy_btn->UseVisualStyleBackColor = false;
			// 
			// usr_repo_open_btn
			// 
			this->usr_repo_open_btn->BackColor = System::Drawing::Color::DarkTurquoise;
			this->usr_repo_open_btn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->usr_repo_open_btn->ForeColor = System::Drawing::SystemColors::ControlText;
			this->usr_repo_open_btn->Location = System::Drawing::Point(18, 247);
			this->usr_repo_open_btn->Name = L"usr_repo_open_btn";
			this->usr_repo_open_btn->Size = System::Drawing::Size(68, 23);
			this->usr_repo_open_btn->TabIndex = 2;
			this->usr_repo_open_btn->Text = L"Open";
			this->usr_repo_open_btn->UseVisualStyleBackColor = false;
			this->usr_repo_open_btn->Click += gcnew System::EventHandler(this, &MyForm::usr_repo_open_btn_Click);
			// 
			// usr_repo_list
			// 
			this->usr_repo_list->BackColor = System::Drawing::Color::SkyBlue;
			this->usr_repo_list->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->usr_repo_list->FormattingEnabled = true;
			this->usr_repo_list->Location = System::Drawing::Point(18, 83);
			this->usr_repo_list->Name = L"usr_repo_list";
			this->usr_repo_list->Size = System::Drawing::Size(141, 158);
			this->usr_repo_list->TabIndex = 1;
			// 
			// edit_repo
			// 
			this->edit_repo->Controls->Add(this->label_repo_status);
			this->edit_repo->Controls->Add(this->repo_private_toggle);
			this->edit_repo->Controls->Add(this->curr_usr_add_box);
			this->edit_repo->Controls->Add(this->curr_usr_remove_box);
			this->edit_repo->Controls->Add(this->edit_repo_remove_btn);
			this->edit_repo->Controls->Add(this->edit_repo_add_btn);
			this->edit_repo->Controls->Add(this->Edit_File_List);
			this->edit_repo->Controls->Add(this->label_sel_repo_name_show);
			this->edit_repo->Location = System::Drawing::Point(4, 4);
			this->edit_repo->Name = L"edit_repo";
			this->edit_repo->Size = System::Drawing::Size(534, 342);
			this->edit_repo->TabIndex = 2;
			this->edit_repo->Text = L"Edit_Repo";
			this->edit_repo->UseVisualStyleBackColor = true;
			this->edit_repo->Click += gcnew System::EventHandler(this, &MyForm::edit_repo_Click);
			// 
			// label_repo_status
			// 
			this->label_repo_status->AutoSize = true;
			this->label_repo_status->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label_repo_status->Location = System::Drawing::Point(24, 55);
			this->label_repo_status->Name = L"label_repo_status";
			this->label_repo_status->Size = System::Drawing::Size(54, 16);
			this->label_repo_status->TabIndex = 8;
			this->label_repo_status->Text = L"Status: -";
			// 
			// repo_private_toggle
			// 
			this->repo_private_toggle->Location = System::Drawing::Point(142, 50);
			this->repo_private_toggle->Name = L"repo_private_toggle";
			this->repo_private_toggle->Size = System::Drawing::Size(51, 27);
			this->repo_private_toggle->TabIndex = 7;
			this->repo_private_toggle->Text = L"Toggle";
			this->repo_private_toggle->UseVisualStyleBackColor = true;
			this->repo_private_toggle->Click += gcnew System::EventHandler(this, &MyForm::repo_private_toggle_Click);
			// 
			// curr_usr_add_box
			// 
			this->curr_usr_add_box->Controls->Add(this->label_file_added_msg);
			this->curr_usr_add_box->Controls->Add(this->edit_add_cmit_btn);
			this->curr_usr_add_box->Controls->Add(this->label_add_box);
			this->curr_usr_add_box->Controls->Add(this->edit_cmit_add_msg);
			this->curr_usr_add_box->Controls->Add(this->label_cmit_add);
			this->curr_usr_add_box->Controls->Add(this->edit_add_file_name);
			this->curr_usr_add_box->Controls->Add(this->label_file_add_name);
			this->curr_usr_add_box->Enabled = false;
			this->curr_usr_add_box->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->curr_usr_add_box->Location = System::Drawing::Point(409, 31);
			this->curr_usr_add_box->Name = L"curr_usr_add_box";
			this->curr_usr_add_box->Size = System::Drawing::Size(258, 240);
			this->curr_usr_add_box->TabIndex = 6;
			this->curr_usr_add_box->TabStop = false;
			this->curr_usr_add_box->Visible = false;
			// 
			// label_file_added_msg
			// 
			this->label_file_added_msg->AutoSize = true;
			this->label_file_added_msg->Enabled = false;
			this->label_file_added_msg->Location = System::Drawing::Point(93, 209);
			this->label_file_added_msg->Name = L"label_file_added_msg";
			this->label_file_added_msg->Size = System::Drawing::Size(76, 16);
			this->label_file_added_msg->TabIndex = 5;
			this->label_file_added_msg->Text = L"File Added!";
			this->label_file_added_msg->Visible = false;
			// 
			// edit_add_cmit_btn
			// 
			this->edit_add_cmit_btn->Location = System::Drawing::Point(75, 169);
			this->edit_add_cmit_btn->Name = L"edit_add_cmit_btn";
			this->edit_add_cmit_btn->Size = System::Drawing::Size(112, 33);
			this->edit_add_cmit_btn->TabIndex = 2;
			this->edit_add_cmit_btn->Text = L"Add File";
			this->edit_add_cmit_btn->UseVisualStyleBackColor = true;
			this->edit_add_cmit_btn->Click += gcnew System::EventHandler(this, &MyForm::edit_add_cmit_btn_Click);
			// 
			// label_add_box
			// 
			this->label_add_box->AutoSize = true;
			this->label_add_box->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label_add_box->Location = System::Drawing::Point(79, 28);
			this->label_add_box->Name = L"label_add_box";
			this->label_add_box->Size = System::Drawing::Size(109, 16);
			this->label_add_box->TabIndex = 4;
			this->label_add_box->Text = L"Add a new File";
			// 
			// edit_cmit_add_msg
			// 
			this->edit_cmit_add_msg->Location = System::Drawing::Point(18, 126);
			this->edit_cmit_add_msg->Name = L"edit_cmit_add_msg";
			this->edit_cmit_add_msg->Size = System::Drawing::Size(209, 22);
			this->edit_cmit_add_msg->TabIndex = 3;
			// 
			// label_cmit_add
			// 
			this->label_cmit_add->AutoSize = true;
			this->label_cmit_add->Location = System::Drawing::Point(18, 107);
			this->label_cmit_add->Name = L"label_cmit_add";
			this->label_cmit_add->Size = System::Drawing::Size(115, 16);
			this->label_cmit_add->TabIndex = 2;
			this->label_cmit_add->Text = L"Commit Message:";
			// 
			// edit_add_file_name
			// 
			this->edit_add_file_name->Location = System::Drawing::Point(94, 64);
			this->edit_add_file_name->Name = L"edit_add_file_name";
			this->edit_add_file_name->Size = System::Drawing::Size(125, 22);
			this->edit_add_file_name->TabIndex = 1;
			// 
			// label_file_add_name
			// 
			this->label_file_add_name->AutoSize = true;
			this->label_file_add_name->Location = System::Drawing::Point(18, 67);
			this->label_file_add_name->Name = L"label_file_add_name";
			this->label_file_add_name->Size = System::Drawing::Size(72, 16);
			this->label_file_add_name->TabIndex = 0;
			this->label_file_add_name->Text = L"File Name:";
			// 
			// curr_usr_remove_box
			// 
			this->curr_usr_remove_box->Controls->Add(this->label_file_remove);
			this->curr_usr_remove_box->Controls->Add(this->edit_remove_commit);
			this->curr_usr_remove_box->Controls->Add(this->label_edit_remove_box);
			this->curr_usr_remove_box->Controls->Add(this->edit_repo_cmit_msg);
			this->curr_usr_remove_box->Controls->Add(this->label_remove_cmit_msg);
			this->curr_usr_remove_box->Controls->Add(this->edit_repo_file_name);
			this->curr_usr_remove_box->Controls->Add(this->label_edit_remove_file);
			this->curr_usr_remove_box->Enabled = false;
			this->curr_usr_remove_box->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->curr_usr_remove_box->Location = System::Drawing::Point(230, 220);
			this->curr_usr_remove_box->Name = L"curr_usr_remove_box";
			this->curr_usr_remove_box->Size = System::Drawing::Size(258, 227);
			this->curr_usr_remove_box->TabIndex = 2;
			this->curr_usr_remove_box->TabStop = false;
			this->curr_usr_remove_box->Visible = false;
			// 
			// label_file_remove
			// 
			this->label_file_remove->AutoSize = true;
			this->label_file_remove->Enabled = false;
			this->label_file_remove->Location = System::Drawing::Point(85, 209);
			this->label_file_remove->Name = L"label_file_remove";
			this->label_file_remove->Size = System::Drawing::Size(95, 16);
			this->label_file_remove->TabIndex = 5;
			this->label_file_remove->Text = L"File Removed!";
			this->label_file_remove->Visible = false;
			// 
			// edit_remove_commit
			// 
			this->edit_remove_commit->Location = System::Drawing::Point(75, 169);
			this->edit_remove_commit->Name = L"edit_remove_commit";
			this->edit_remove_commit->Size = System::Drawing::Size(112, 33);
			this->edit_remove_commit->TabIndex = 2;
			this->edit_remove_commit->Text = L"Remove";
			this->edit_remove_commit->UseVisualStyleBackColor = true;
			this->edit_remove_commit->Click += gcnew System::EventHandler(this, &MyForm::edit_remove_commit_Click);
			// 
			// label_edit_remove_box
			// 
			this->label_edit_remove_box->AutoSize = true;
			this->label_edit_remove_box->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label_edit_remove_box->Location = System::Drawing::Point(79, 28);
			this->label_edit_remove_box->Name = L"label_edit_remove_box";
			this->label_edit_remove_box->Size = System::Drawing::Size(108, 16);
			this->label_edit_remove_box->TabIndex = 4;
			this->label_edit_remove_box->Text = L"Remove a File";
			// 
			// edit_repo_cmit_msg
			// 
			this->edit_repo_cmit_msg->Location = System::Drawing::Point(18, 125);
			this->edit_repo_cmit_msg->Name = L"edit_repo_cmit_msg";
			this->edit_repo_cmit_msg->Size = System::Drawing::Size(209, 22);
			this->edit_repo_cmit_msg->TabIndex = 3;
			// 
			// label_remove_cmit_msg
			// 
			this->label_remove_cmit_msg->AutoSize = true;
			this->label_remove_cmit_msg->Location = System::Drawing::Point(18, 107);
			this->label_remove_cmit_msg->Name = L"label_remove_cmit_msg";
			this->label_remove_cmit_msg->Size = System::Drawing::Size(115, 16);
			this->label_remove_cmit_msg->TabIndex = 2;
			this->label_remove_cmit_msg->Text = L"Commit Message:";
			// 
			// edit_repo_file_name
			// 
			this->edit_repo_file_name->Location = System::Drawing::Point(94, 64);
			this->edit_repo_file_name->Name = L"edit_repo_file_name";
			this->edit_repo_file_name->Size = System::Drawing::Size(125, 22);
			this->edit_repo_file_name->TabIndex = 1;
			// 
			// label_edit_remove_file
			// 
			this->label_edit_remove_file->AutoSize = true;
			this->label_edit_remove_file->Location = System::Drawing::Point(18, 67);
			this->label_edit_remove_file->Name = L"label_edit_remove_file";
			this->label_edit_remove_file->Size = System::Drawing::Size(72, 16);
			this->label_edit_remove_file->TabIndex = 0;
			this->label_edit_remove_file->Text = L"File Name:";
			// 
			// edit_repo_remove_btn
			// 
			this->edit_repo_remove_btn->Location = System::Drawing::Point(24, 303);
			this->edit_repo_remove_btn->Name = L"edit_repo_remove_btn";
			this->edit_repo_remove_btn->Size = System::Drawing::Size(102, 23);
			this->edit_repo_remove_btn->TabIndex = 5;
			this->edit_repo_remove_btn->Text = L"Remove a File";
			this->edit_repo_remove_btn->UseVisualStyleBackColor = true;
			this->edit_repo_remove_btn->Click += gcnew System::EventHandler(this, &MyForm::edit_repo_remove_btn_Click);
			// 
			// edit_repo_add_btn
			// 
			this->edit_repo_add_btn->Location = System::Drawing::Point(24, 277);
			this->edit_repo_add_btn->Name = L"edit_repo_add_btn";
			this->edit_repo_add_btn->Size = System::Drawing::Size(102, 23);
			this->edit_repo_add_btn->TabIndex = 4;
			this->edit_repo_add_btn->Text = L"Add a File";
			this->edit_repo_add_btn->UseVisualStyleBackColor = true;
			this->edit_repo_add_btn->Click += gcnew System::EventHandler(this, &MyForm::edit_repo_add_btn_Click);
			// 
			// Edit_File_List
			// 
			this->Edit_File_List->FormattingEnabled = true;
			this->Edit_File_List->Location = System::Drawing::Point(24, 85);
			this->Edit_File_List->Name = L"Edit_File_List";
			this->Edit_File_List->SelectionMode = System::Windows::Forms::SelectionMode::None;
			this->Edit_File_List->Size = System::Drawing::Size(169, 186);
			this->Edit_File_List->TabIndex = 3;
			// 
			// label_sel_repo_name_show
			// 
			this->label_sel_repo_name_show->AutoSize = true;
			this->label_sel_repo_name_show->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label_sel_repo_name_show->Location = System::Drawing::Point(20, 21);
			this->label_sel_repo_name_show->Name = L"label_sel_repo_name_show";
			this->label_sel_repo_name_show->Size = System::Drawing::Size(178, 20);
			this->label_sel_repo_name_show->TabIndex = 0;
			this->label_sel_repo_name_show->Text = L"Selected Repo: none";
			// 
			// following_list_box
			// 
			this->following_list_box->Controls->Add(this->following_list);
			this->following_list_box->Location = System::Drawing::Point(12, 102);
			this->following_list_box->Name = L"following_list_box";
			this->following_list_box->Size = System::Drawing::Size(211, 173);
			this->following_list_box->TabIndex = 5;
			this->following_list_box->TabStop = false;
			this->following_list_box->Text = L"Following";
			// 
			// following_list
			// 
			this->following_list->Dock = System::Windows::Forms::DockStyle::Fill;
			this->following_list->FormattingEnabled = true;
			this->following_list->Location = System::Drawing::Point(3, 16);
			this->following_list->Name = L"following_list";
			this->following_list->Size = System::Drawing::Size(205, 154);
			this->following_list->TabIndex = 0;
			this->following_list->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::following_list_SelectedIndexChanged);
			// 
			// UsersListBox
			// 
			this->UsersListBox->Controls->Add(this->sel_user_list);
			this->UsersListBox->Location = System::Drawing::Point(12, 315);
			this->UsersListBox->Name = L"UsersListBox";
			this->UsersListBox->Size = System::Drawing::Size(211, 150);
			this->UsersListBox->TabIndex = 4;
			this->UsersListBox->TabStop = false;
			this->UsersListBox->Text = L"UsersList";
			// 
			// sel_user_list
			// 
			this->sel_user_list->Dock = System::Windows::Forms::DockStyle::Fill;
			this->sel_user_list->FormattingEnabled = true;
			this->sel_user_list->Location = System::Drawing::Point(3, 16);
			this->sel_user_list->Name = L"sel_user_list";
			this->sel_user_list->Size = System::Drawing::Size(205, 131);
			this->sel_user_list->TabIndex = 0;
			this->sel_user_list->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::sel_user_list_SelectedIndexChanged);
			// 
			// usr_name_label
			// 
			this->usr_name_label->AutoSize = true;
			this->usr_name_label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 21.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->usr_name_label->Location = System::Drawing::Point(432, 26);
			this->usr_name_label->Name = L"usr_name_label";
			this->usr_name_label->Size = System::Drawing::Size(210, 33);
			this->usr_name_label->TabIndex = 2;
			this->usr_name_label->Text = L"<User-Name>";
			// 
			// user_title_label
			// 
			this->user_title_label->AutoSize = true;
			this->user_title_label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 21.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->user_title_label->Location = System::Drawing::Point(226, 26);
			this->user_title_label->Name = L"user_title_label";
			this->user_title_label->Size = System::Drawing::Size(206, 33);
			this->user_title_label->TabIndex = 1;
			this->user_title_label->Text = L"Logged In As:";
			// 
			// label_credits_title
			// 
			this->label_credits_title->AutoSize = true;
			this->label_credits_title->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label_credits_title->Location = System::Drawing::Point(221, 145);
			this->label_credits_title->Name = L"label_credits_title";
			this->label_credits_title->Size = System::Drawing::Size(355, 168);
			this->label_credits_title->TabIndex = 3;
			this->label_credits_title->Text = L"Made By: \r\n        Maratib Lashari\r\n        Asheer Adnan\r\n        Alia\r\n";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(820, 530);
			this->Controls->Add(this->register_acc_box);
			this->Controls->Add(this->MainMenu);
			this->Controls->Add(this->UserPage_Box);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->MainMenu->ResumeLayout(false);
			this->MainMenu->PerformLayout();
			this->register_acc_box->ResumeLayout(false);
			this->register_acc_box->PerformLayout();
			this->credits_box->ResumeLayout(false);
			this->credits_box->PerformLayout();
			this->UserPage_Box->ResumeLayout(false);
			this->UserPage_Box->PerformLayout();
			this->RipHub->ResumeLayout(false);
			this->View_User_Page->ResumeLayout(false);
			this->Current_User_Page->ResumeLayout(false);
			this->Current_User_Page->PerformLayout();
			this->Create_Repo_Box->ResumeLayout(false);
			this->Create_Repo_Box->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->edit_repo->ResumeLayout(false);
			this->edit_repo->PerformLayout();
			this->curr_usr_add_box->ResumeLayout(false);
			this->curr_usr_add_box->PerformLayout();
			this->curr_usr_remove_box->ResumeLayout(false);
			this->curr_usr_remove_box->PerformLayout();
			this->following_list_box->ResumeLayout(false);
			this->UsersListBox->ResumeLayout(false);
			this->ResumeLayout(false);

		}

#pragma endregion

	private: System::Void checkBox1_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		if (password_login->UseSystemPasswordChar)
			password_login->UseSystemPasswordChar = false;
		else
			password_login->UseSystemPasswordChar = true;
	}
	private: System::Void login_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		this->loadingData();
		string usr = changeString(user__login->Text);
		string pwd = changeString(password_login->Text);
		if (UserAccounts.checkDataExists({ usr,pwd }) && usr != "" && pwd != "") {
			//Login
			login_error_label->Hide();
			currUser = usr;
			currUserIndex = searchUserIndex(usr);
			usr_name_label->Text = changeString(usr);

			MainMenu->Enabled = false;
			MainMenu->Visible = false;
			UserPage_Box->Enabled = true;
			UserPage_Box->Visible = true;

			label_welcomeback->Text = "Welcome Back, " + changeString(usr);

			vector<string> repoList = repoMangr[currUserIndex].getRepoList();
			curr_usr_repo_list->Items->Clear();
			curr_usr_file_list->Items->Clear();
			curr_usr_comit_hist_list->Items->Clear();
			for (int i = 0; i < repoList.size(); i++) {
				curr_usr_repo_list->Items->Add(changeString(repoList[i]));
			}

			UpdateFollowingList();
			UpdateUsersList();
			
		}
		else {
			//Invalid Data - No Acc Exists
			login_error_label->Show();
		}
	}
	private: System::Void reg_newacc_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		if (r_pwd_textbox->Text != r_rpwd_textbox->Text) {
			register_error_label->Text = "Password does not match!";
			reg_acc_created_label->Hide();
			register_error_label->Show();
		}
		else if (UserAccounts.checkUserExists(changeString(r_usr_txtbox->Text))) {
			register_error_label->Text = "User with this username already exists!";
			reg_acc_created_label->Hide();
			register_error_label->Show();
		}
		else {
			string usr, pwd;
			usr = changeString(r_usr_txtbox->Text);
			pwd = changeString(r_pwd_textbox->Text);
			if (usr == "") {
				register_error_label->Text = "User cannot be empty!";
				reg_acc_created_label->Hide();
				register_error_label->Show();
				return;
			}
			UserAccounts.InsertData({usr, pwd});
			UserFollowGraph.addNewUsr(usr);
			UserAccounts.SaveUserNamePassWords();
			UserAccounts.LoadUserNamePassWords();
			this->debugging();
			reg_acc_created_label->Show();
			register_error_label->Hide();
			r_usr_txtbox->Text = "";
			r_pwd_textbox->Text = "";
			r_rpwd_textbox->Text = "";

			loadingData();
		}
	}
	private: System::Void register_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		MainMenu->Visible = false;
		MainMenu->Enabled = false;

		register_acc_box->Visible = true;
		register_acc_box->Enabled = true;
	}
	private: System::Void reg_back_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		MainMenu->Visible = true;
		MainMenu->Enabled = true;

		register_acc_box->Visible = false;
		register_acc_box->Enabled = false;

		register_error_label->Hide();
		reg_acc_created_label->Hide();
	}
	private: System::Void MainMenu_Enter(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void log_out_Click(System::Object^ sender, System::EventArgs^ e) {
		MainMenu->Visible = true;
		MainMenu->Enabled = true;

		UserPage_Box->Visible = false;
		UserPage_Box->Enabled = false;

		currUser = "";
		currUserIndex = -1;
		Edit_Active_Repo = nullptr;
		Edit_File_List->Items->Clear();
		label_sel_repo_name_show->Text = "Selected Repo: none";
		label_repo_status->Text = "Status: -";
	}
	private: System::Void view_follower_Click(System::Object^ sender, System::EventArgs^ e) {
		int index = following_list->SelectedIndex;
		if (index < 0) { return; }

		String^ view_user = following_list->Items[index]->ToString();
		string v_usr = changeString(view_user);

		curr_viewing_label->Text = "Currently Viewing: " + view_user;

		//Update User's Repo List
		UpdateUsrRepoList(v_usr);

	}
	private: System::Void UnFollow_Click(System::Object^ sender, System::EventArgs^ e) {
		int index = following_list->SelectedIndex;
		if (index < 0) { return; }

		string fol = changeString(following_list->Items[index]->ToString());

		UserFollowGraph.removeFollower(currUser, fol);
		UpdateFollowingList();
	}
	private: System::Void follow_user_Click(System::Object^ sender, System::EventArgs^ e) {

		int index = sel_user_list->SelectedIndex;
		if (index < 0) { return; }

		string fol = changeString(sel_user_list->Items[index]->ToString());

		UserFollowGraph.addFollower(currUser, fol);
		UpdateFollowingList();

	}
	private: System::Void view_user_Click(System::Object^ sender, System::EventArgs^ e) {
		int index = sel_user_list->SelectedIndex;
		if (index < 0) { return; }

		String^ view_user = sel_user_list->Items[index]->ToString();
		string v_usr = changeString(view_user);
		
		curr_viewing_label->Text = "Currently Viewing: " + view_user;

		//Update User's Repo List
		UpdateUsrRepoList(v_usr);
	}
	private: System::Void quit_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		Close();
	}
	private: System::Void usr_repo_open_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		int selectedItem = usr_repo_list->SelectedIndex;
		if (selectedItem < 0) { return; }
		String^ repoName = usr_repo_list->Items[selectedItem]->ToString();
		Node repo = repoMangr[activeViewUserIndex].searchRepo(changeString(repoName));
		if (repo.Repo_Name == "") { return; }

		usr_files_list->Items->Clear();
		usr_commit_hist_list->Items->Clear();
		
		if (repo.isPrivate) {
			usr_files_list->Items->Add("Private Repository.");
			usr_commit_hist_list->Items->Add("Access Denied.");
			return;
		}

		//Display Files
		for (int i = 0; i < repo.files->getNodeCount(); i++) {
			usr_files_list->Items->Add(changeString(repo.files->getDataAt(i)));
		}

		//Display Commit History
		for (int i = 0; i < repo.commits->getNodeCount(); i++) {
			usr_commit_hist_list->Items->Add(changeString(repo.commits->getDataAt(i)));
		}

	}
	private: System::Void following_list_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		int i = following_list->SelectedIndex;
		sel_user_list->SelectedIndex = -1;
		following_list->SelectedIndex = i;
	}
	private: System::Void sel_user_list_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		int i = sel_user_list->SelectedIndex;
		following_list->SelectedIndex = -1;
		sel_user_list->SelectedIndex = i;
	}
	private: System::Void curr_usr_view_repo_Click(System::Object^ sender, System::EventArgs^ e) {
		int selectedItem = curr_usr_repo_list->SelectedIndex;
		if (selectedItem < 0) { return; }
		String^ repoName = curr_usr_repo_list->Items[selectedItem]->ToString();
		Node repo = repoMangr[currUserIndex].searchRepo(changeString(repoName));
		if (repo.Repo_Name == "") { return; }

		curr_usr_file_list->Items->Clear();
		curr_usr_comit_hist_list->Items->Clear();

		//Display Files
		for (int i = 0; i < repo.files->getNodeCount(); i++) {
			curr_usr_file_list->Items->Add(changeString(repo.files->getDataAt(i)));
		}

		//Display Commit History
		for (int i = 0; i < repo.commits->getNodeCount(); i++) {
			curr_usr_comit_hist_list->Items->Add(changeString(repo.commits->getDataAt(i)));
		}
	}

	private: System::Void edit_repo_add_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		curr_usr_add_box->Visible = true;
		curr_usr_add_box->Enabled = true;

		curr_usr_remove_box->Visible = false;
		curr_usr_remove_box->Enabled = false;

		curr_usr_add_box->Parent = edit_repo;
		curr_usr_add_box->SetBounds(246, 81, 258, 245);

	}
	private: System::Void edit_repo_remove_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		curr_usr_add_box->Visible = false;
		curr_usr_add_box->Enabled = false;

		curr_usr_remove_box->Visible = true;
		curr_usr_remove_box->Enabled = true;

		curr_usr_remove_box->Parent = edit_repo;
		curr_usr_remove_box->SetBounds(246, 81, 258, 245);
	}
	private: System::Void edit_repo_Click(System::Object^ sender, System::EventArgs^ e) {

	}
	private: System::Void edit_remove_commit_Click(System::Object^ sender, System::EventArgs^ e) {
		string fileToRemove = changeString(edit_repo_file_name->Text);
		string commitMsg = changeString(edit_repo_cmit_msg->Text);

		if (fileToRemove == "") { return; }
		if (commitMsg == "") {
			commitMsg = fileToRemove + " was removed.";
		}

		Edit_Active_Repo->files->Delete(fileToRemove);
		Edit_Active_Repo->commits->Insert(commitMsg);

		int files_count = Edit_Active_Repo->files->getNodeCount();
		Edit_File_List->Items->Clear();
		if (files_count < 0) { return; }

		for (int i = 0; i < files_count; i++) {
			string fileN = Edit_Active_Repo->files->getDataAt(i);
			Edit_File_List->Items->Add(changeString(fileN));
		}
		repoMangr[currUserIndex].SaveData();
		label_file_remove->Visible = true;
		label_file_remove->Enabled = true;
	}
	private: System::Void curr_usr_edit_repo_Click(System::Object^ sender, System::EventArgs^ e) {
		int selIndex = curr_usr_repo_list->SelectedIndex;
		if (selIndex < 0) { return; }
		string currUsrRepoName = changeString(curr_usr_repo_list->Items[selIndex]->ToString());
		
		Edit_Active_Repo = repoMangr[currUserIndex].getRepo(currUsrRepoName);	//Take O(logN) time
		if (Edit_Active_Repo == nullptr || Edit_Active_Repo->Repo_Name == "") { return; }

		int files_count = Edit_Active_Repo->files->getNodeCount();
		Edit_File_List->Items->Clear();
		if (files_count < 0) { return; }

		for (int i = 0; i < files_count; i++) {
			string fileN = Edit_Active_Repo->files->getDataAt(i);
			Edit_File_List->Items->Add(changeString(fileN));
		}
		View_User_Page->SelectTab("edit_repo");
		label_sel_repo_name_show->Text = "Selected Repo: " + changeString(currUsrRepoName);
		label_file_remove->Visible = false;
		label_file_remove->Enabled = false;
		label_file_added_msg->Visible = false;
		label_file_added_msg->Enabled = false;
		if (Edit_Active_Repo->isPrivate) {
			label_repo_status->Text = "Status: Private";
		}
		else {
			label_repo_status->Text = "Status: Public";
		}
	}
	private: System::Void edit_add_cmit_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		string fileToAdd = changeString(edit_add_file_name->Text);
		string commitMsg = changeString(edit_cmit_add_msg->Text);

		if (fileToAdd == "") { return; }
		if (commitMsg == "") {
			commitMsg = fileToAdd + " was added.";
		}

		Edit_Active_Repo->files->Insert(fileToAdd);
		Edit_Active_Repo->commits->Insert(commitMsg);

		int files_count = Edit_Active_Repo->files->getNodeCount();
		Edit_File_List->Items->Clear();
		if (files_count < 0) { return; }

		for (int i = 0; i < files_count; i++) {
			string fileN = Edit_Active_Repo->files->getDataAt(i);
			Edit_File_List->Items->Add(changeString(fileN));
		}
		repoMangr[currUserIndex].SaveData();
		label_file_added_msg->Visible = true;
		label_file_added_msg->Enabled = true;
		
	}
	private: System::Void create_repo_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		Create_Repo_Box->Visible = true;
		Create_Repo_Box->Enabled = true;
	}
	private: System::Void confirm_create_Click(System::Object^ sender, System::EventArgs^ e) {
		if (create_repo_name->Text == "") { return; }
		string repo_name = changeString(create_repo_name->Text);

		repoMangr[currUserIndex].createRepo(repo_name);
		Create_Repo_Box->Visible = false;
		Create_Repo_Box->Enabled = false;
		repoMangr[currUserIndex].SaveData();
		log_out->PerformClick();
		login_btn->PerformClick();
	}
	private: System::Void repo_private_toggle_Click(System::Object^ sender, System::EventArgs^ e) {
		if (label_repo_status->Text == "Status: Private") {
			Edit_Active_Repo->isPrivate = false;
			label_repo_status->Text = "Status: Public";
		}
		else if (label_repo_status->Text == "Status: Public") {
			Edit_Active_Repo->isPrivate = true;
			label_repo_status->Text = "Status: Private";
		}
	}
	private: System::Void credits_back_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		credits_box->Enabled = false;
		credits_box->Visible = false;

		MainMenu->Enabled = true;
		MainMenu->Visible = true;
	}
	private: System::Void show_credits_btn_Click(System::Object^ sender, System::EventArgs^ e) {
		credits_box->Enabled = true;
		credits_box->Visible = true;

		MainMenu->Enabled = false;
		MainMenu->Visible = false;
	}
};
}
