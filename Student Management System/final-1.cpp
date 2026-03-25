#include<iostream>
#include<cstdlib>
using namespace std;
int main()
{
	int choice = 0, roll_no = 0, i_count = 0, check = 0;
	const  int size = 101;
	int stu[size] = { 0 };
	int new_sem = 0, new_sec = 0, cc = 0; float new_cgpa = 0.0, cgpa[size] = { 0.0 };
	float quiz1[size] = { 0.0 }, quiz2[size] = { 0.0 }, quiz3[size] = { 0.0 }, mid_term[size] = { 0.0 };
	int semester[size] = { 0 }; int high = 0, low = 0; float q1 = 0.0, q2 = 0.0, q3 = 0.0, mid1 = 0.0;
	int section[size] = { 0 }, sec = 0, c_c = 0;
	for (int i = 0; i < size; i++){
		quiz1[i] = rand() % 11;
		quiz2[i] = rand() % 11;
		quiz3[i] = rand() % 11;
		cgpa[i] = rand() % (4 - 1 + 1) + 1;
		mid_term[i] = rand() % (100 - 10 + 1) + 10;
		semester[i] = rand() % (8 - 1 + 1) + 1;
		section[i] = rand() % (12 - 10 + 1) + 10;
	}
	for (int i = 1; i < 34; i++)
	{
		stu[i] = 10213;
	}
	for (int i = 34; i < 70; i++)
	{
		stu[i] = 10811;
	}
	for (int i = 70; i <= 100; i++)
	{
		stu[i] = 23011;
	}
	do{
		cout << "-----------------------------------WELCOME TO STUDENT RECORD MANAGEMENT SYSTEM--------------------------------" << endl;
		cout << "MENU:\n(Note = Kindly select from following options.)" << endl;
		cout << "1-  View a student record" << endl;
		cout << "2-  Insert a new record" << endl;
		cout << "3-  Alter a record" << endl;
		cout << "4-  Show records of a respective section" << endl;
		cout << "5-  Show records of a respective course" << endl;
		cout << "6-  Show student with highest CGPA" << endl;
		cout << "7-  Show student with lowest CGPA" << endl;
		cout << "8-  Show students with mid-term marks less than 50" << endl;
		cout << "9-  Show students with zero marks in Quiz 2" << endl;
		cout << "10- Show students having a prob (prob means CGPA less than 2.0)" << endl;
		cout << "11- Alter the mid-term marks of a student only" << endl;
		cout << "12- Alter the Quiz 1 marks of a student only" << endl;
		cout << "13- Alter the Quiz 2 marks of a student only." << endl;
		cout << "14- Alter the Quiz 3 marks of a student only." << endl;
		cout << "15- Show all the records." << endl;
		cout << "16- Exit" << endl;
		do{
			cout << "Please enter the choice:";
			cin >> choice;
		} while (choice<1 || choice>16);
		// VIEW STUDENT RECORD
		if (choice == 1){
			do{
			cout << "Enter Roll Number of Student = ";
			cin >> roll_no;
			} while (roll_no<1 || roll_no>100);
			cout << "Roll Number = " << roll_no << endl;
			cout << "Semester = " << semester[roll_no] << endl;
			if(semester[roll_no]==1)
			continue;
			cout << "CGPA = " << cgpa[roll_no] << endl;
			cout << "Section = " << section[roll_no] << endl;
			cout << "Course code = " << stu[roll_no] << endl;
			cout << "Quiz 1 marks = " << quiz1[roll_no] << endl;
			cout << "Quiz 2 marks = " << quiz2[roll_no] << endl;
			cout << "Quiz 3 marks = " << quiz3[roll_no] << endl;
			cout << "Mid-term marks = " << mid_term[roll_no] << endl;
		}
		//INSERT NEW RECORD
		if (choice == 2){
			cout << "Enter Roll Number of Student:";
			cin >> roll_no;
			cout << "Roll Number = " << roll_no << endl;
			cout << "Enter the Semester:";
			cin >> new_sem;
			semester[roll_no] = new_sem;
			cout << "Enter the CGPA = ";
			cin >> new_cgpa;
			cgpa[roll_no] = new_cgpa;
			cout << "Enter the Section:";
			cin >> new_sec;
			section[roll_no] = new_sec;
			cout << "Enter the Course code:";
			cin >> cc;
			stu[roll_no] = cc;
			cout << "Enter Quiz 1 marks:";
			cin >> q1;
			quiz1[roll_no] = q1;
			cout << "Enter Quiz 2 marks:";
			cin >> q2;
			quiz2[roll_no] = q2;
			cout << "Enter Quiz 3 marks:";
			cin >> q3;
			quiz3[roll_no] = q3;
			cout << "Enter mid term marks:";
			cin >> mid1;
			mid_term[roll_no] = mid1;
			cout << "Added Successfully" << endl;
		} //ALTER RECORD
		if (choice == 3){
			do{
				cout << "Enter Roll Number of Student:";
				cin >> roll_no;
			} while (roll_no<1 || roll_no>101);
			cout << "Roll Number = " << roll_no << endl;
			cout << "Enter the Semester:";
			cin >> new_sem;
			semester[roll_no] = new_sem;
			cout << "Enter the CGPA = ";
			cin >> new_cgpa;
			cgpa[roll_no] = new_cgpa;
			cout << "Enter the Section:";
			cin >> new_sec;
			section[roll_no] = new_sec;
			cout << "Enter the Course code:";
			cin >> cc;
			stu[roll_no] = cc;
			cout << "Enter Quiz 1 marks:";
			cin >> q1;
			quiz1[roll_no] = q1;
			cout << "Enter Quiz 2 marks:";
			cin >> q2;
			quiz2[roll_no] = q2;
			cout << "Enter Quiz 3 marks:";
			cin >> q3;
			quiz3[roll_no] = q3;
			cout << "Enter mid term marks:";
			cin >> mid1;
			mid_term[roll_no] = mid1;
			cout << "Record Updated Successfully" << endl;
		}//RESPECTIVE SECTIONS
		if (choice == 4)
		{
			do{
				cout << "Enter the section(10,11,12):";
				cin >> sec;
			} while (sec<10 || sec>12);
			if (sec == 10)
			{
				for (int i = 1; i < 33; i++)
				{
					cout << "Roll Number = " << i << endl;
					cout << "Semester = " << semester[i] << endl;
					cout << "CGPA = " << cgpa[i] << endl;
					cout << "Course code = " << stu[i] << endl;
					cout << "Quiz 1 marks = " << quiz1[i] << endl;
					cout << "Quiz 2 marks = " << quiz2[i] << endl;
					cout << "Quiz 3 marks = " << quiz3[i] << endl;
					cout << "Mid-term marks = " << mid_term[i] << endl;
				}
			}
			if (sec == 11)
			{
				for (int i = 33; i < 66; i++)
				{
					cout << "Roll Number = " << i << endl;
					cout << "Semester = " << semester[i] << endl;
					cout << "CGPA = " << cgpa[i] << endl;
					cout << "Course code = " << stu[i] << endl;
					cout << "Quiz 1 marks = " << quiz1[i] << endl;
					cout << "Quiz 2 marks = " << quiz2[i] << endl;
					cout << "Quiz 3 marks = " << quiz3[i] << endl;
					cout << "Mid-term marks = " << mid_term[i] << endl;
				}
			}
			if (sec == 12)
			{
				for (int i = 66; i <= 100; i++)
				{
					cout << "Roll Number = " << i << endl;
					cout << "Semester = " << semester[i] << endl;
					cout << "CGPA = " << cgpa[i] << endl;
					cout << "Course code = " << stu[i] << endl;
					cout << "Quiz 1 marks = " << quiz1[i] << endl;
					cout << "Quiz 2 marks = " << quiz2[i] << endl;
					cout << "Quiz 3 marks = " << quiz3[i] << endl;
					cout << "Mid-term marks = " << mid_term[i] << endl;
				}
			}
		}
		//RESPECTIVE COURSE
		if (choice == 5)
		{
			cout << "Enter the COURSE(10213,10811,23011):";
			cin >> c_c;//c_c : course code
			if (c_c == 10213)
			{
				for (int i = 1; i <= 33; i++)
				{
					cout << "Roll Number = " << i << endl;
					cout << "Semester = " << semester[i] << endl;
					cout << "Section = " << section[i] << endl;
					cout << "CGPA = " << cgpa[i] << endl;
					cout << "Quiz 1 marks = " << quiz1[i] << endl;
					cout << "Quiz 2 marks = " << quiz2[i] << endl;
					cout << "Quiz 3 marks = " << quiz3[i] << endl;
					cout << "Mid-term marks = " << mid_term[i] << endl;
				}
			}
			if (c_c == 10811)
			{
				for (int i = 34; i <= 66; i++)
				{
					cout << "Roll Number = " << i << endl;
					cout << "Semester = " << semester[i] << endl;
					cout << "Section = " << section[i] << endl;
					cout << "CGPA = " << cgpa[i] << endl;
					cout << "Quiz 1 marks = " << quiz1[i] << endl;
					cout << "Quiz 2 marks = " << quiz2[i] << endl;
					cout << "Quiz 3 marks = " << quiz3[i] << endl;
					cout << "Mid-term marks = " << mid_term[i] << endl;
				}
			}
			if (c_c == 23011){
				for (int i = 67; i <= 100; i++)
				{
					cout << "Roll Number = " << i << endl;
					cout << "Semester = " << semester[i] << endl;
					cout << "Section = " << section[i] << endl;
					cout << "CGPA = " << cgpa[i] << endl;
					cout << "Quiz 1 marks = " << quiz1[i] << endl;
					cout << "Quiz 2 marks = " << quiz2[i] << endl;
					cout << "Quiz 3 marks = " << quiz3[i] << endl;
					cout << "Mid-term marks = " << mid_term[i] << endl;
				}
			}
		}
		//HIGHEST CGPA
		if (choice == 6)
		{
			for (int i = 1; i < size-1; ++i)
			{
				if (cgpa[1] < cgpa[i+1])
				{
					cgpa[1] = cgpa[i];
					high = cgpa[1];
					i_count = i;
				}
			}cout << "Student " << i_count << " has highest CGPA that is " << high << endl;
		}//LOWEST CGPA
		if (choice == 7)
		{
			cgpa[1] = { 2 };
			for (int i = 2; i < size; ++i)
			{
				if (cgpa[1] > cgpa[i+1])
				{
					cgpa[1] = cgpa[i+1];
					low = cgpa[1];
					i_count = i;
				}
			}cout << "Student " << i_count << " has LOWEST CGPA that is " << low << endl;
		}
		//MID TERM MARKS LESS THAN 50
		if (choice == 8)
		{
			int marks = 50;
			for (int i = 1; i < size; i++)
			{
				if (mid_term[i] < marks)
				{
					cout << "Student " << i << " has marks less than 50 in mid term " << endl;
					break;
				}
			}
		}
		//QUIZ MARKS = ZERO
		if (choice == 9)
		{
			for (int i = 1; i < size; i++)
			{
				if (quiz2[i] == 0)
				{
					cout << "Student " << i << " has zero marks in Quiz 2" << endl;
				}
			}
		}
		//PROB
		if (choice == 10)
		{
			for (int i = 1; i < size; i++)
			{
				if (cgpa[i] < 2)
				{
					cout << "Student " << i << " has prob " << endl;
				}

			}
		}//ALTER MID MARKS 
		if (choice == 11)
		{
			cout << "Enter Roll Number of Student:";
			cin >> roll_no;
			cout << "Enter mid term marks:";
			cin >> mid1;
			mid_term[roll_no] = mid1;
			cout << "Record Updated Successfully" << endl;
		}//ALTER QUIZ1 MARKS
		if (choice == 12)
		{
			cout << "Enter Roll Number of Student:";
			cin >> roll_no;
			cout << "Enter Quiz 1 marks:";
			cin >> q1;
			quiz1[roll_no] = q1;
			cout << "Record Updated Successfully" << endl;
		}//ALTER QUIZ2 MARKS
		if (choice == 13)
		{
			cout << "Enter Roll Number of Student:";
			cin >> roll_no;
			cout << "Enter Quiz 2 marks:";
			cin >> q2;
			quiz2[roll_no] = q2;
			cout << "Record Updated Successfully" << endl;
		}//ALTER QUIZ3 MARKS
		if (choice == 14)
		{
			cout << "Enter Roll Number of Student:";
			cin >> roll_no;
			cout << "Enter Quiz 3 marks:";
			cin >> q3;
			quiz3[roll_no] = q3;
			cout << "Record Updated Successfully" << endl;
		}//ALL RECORDS
		if (choice == 15)
		{
			for (int i = 1; i < size; i++)
			{
				cout << "Roll Number = " << i << endl;
				cout << "Semester = " << semester[i] << endl;
				cout << "CGPA = " << cgpa[i] << endl;
				cout << "Section = " << section[i] << endl;
				cout << "Course code = " << stu[i] << endl;
				cout << "Quiz 1 marks = " << quiz1[i] << endl;
				cout << "Quiz 2 marks = " << quiz2[i] << endl;
				cout << "Quiz 3 marks = " << quiz3[i] << endl;
				cout << "Mid-term marks = " << mid_term[i] << endl;
			}
		}
		cout << "Do you want to run agaain?(0/1):";
		cin >> check;
	}while (check == 1);
		//EXIT
		if (choice == 16)
		{
			cout << "---------------------------------GOODBYE--------------------------------" << endl;
			return 0;
		}
		
	system("pause");
	return 0;
}
