#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iomanip>

using namespace std;

void readFile(vector<float>& id, vector<float>& attrib, vector<float>& attrib_val, vector<float>& id_test, vector<float>& attrib_test, vector<float>& attrib_val_test, vector<int>& Class_label, vector<float>& id_sample, vector<float>& attrib_sample, vector<float>& attrib_val_sample);
void setSizeMatrix(int row, int col, vector< vector<float> > &matrix);
void setupMatrix_value(vector<float>& id, vector<float>& attrib, vector<float>& attrib_val, vector<int>& num, int row, int col, vector< vector<float> >&matrix);
void check_attrib_num(vector<float>& id,vector<int>& num);
void WriteFile( int row, int col, vector< vector<float> > &matrix, vector<int>& Class_label, int choice);
void FindSameAttrib(vector<float>& id, vector<float>& attrib, vector<int>& common_attrib, vector<float>& common_value, vector<float>& attrib_val, vector<int>& attrib_num);
int FindHighestNum(vector<float>& attrib);
void insertion_sort(vector<float>& input, int length);

int main()
{
	int initial_row = 0;
	int initial_col = 0;
	/*******Training set*********/
	vector<float> ID;
	vector<float> Attrib;
	vector<float> Attrib_val;
	vector<int>Attrib_num; // # of attribute per each ID
	vector<int>Common_Attrib; // contain only common attribute
	vector<int>class_label;
	vector<int>class_label_test;
	vector<float>Common_value = {0}; // ignore this one

	/*******Test set*********/
	vector<float> ID_testing;
	vector<float> Attrib_testing;
	vector<float> Attrib_val_testing;
	vector<int>Attrib_num_test; // # of attribute per each ID
	vector<int>Common_Attrib_test; // contain only common attribute

	/*******Sample set*********/
	vector<float> ID_Sample;
	vector<float> Attrib_Sample;
	vector<float> Attrib_val_Sample;
	vector<int>Attrib_num_Sample; // # of attribute per each ID

	vector<vector<float> > Matrix; 
	vector<vector<float> > Matrix_Test;
	vector<vector<float> > Matrix_Sample; 

	readFile(ID, Attrib, Attrib_val, ID_testing, Attrib_testing, Attrib_val_testing, class_label, ID_Sample, Attrib_Sample, Attrib_val_Sample); // Read input files

	int choice = 0;

	do
	{
		cout << "Which task do you want to perform? " << endl;
		cout << "1. generate Matrix by Training set" << endl;
		cout << "2. generate Matrix by Testing set" << endl;
		cout << "3. generate Matrix by Sample set" << endl;
		cout << "4. EXIT" << endl;

		cin >> choice;
		cout << endl;
		while(choice < 1 || choice > 4)
		{
			cout << "Please Type Valid Operation" << endl;
			cin >> choice;
		}

	   switch(choice) 
	   {
	      case 1 :
	      	 	check_attrib_num(ID, Attrib_num); // Check how many attributes each ID has
				//FindSameAttrib(ID, Attrib, Common_Attrib, Common_value, Attrib_val, Attrib_num); // Find common Attribute.

				initial_row = ID.back();
				initial_col = FindHighestNum(Attrib) + 1;
				//cout << initial_row << " " << initial_col << endl;

				setSizeMatrix(initial_row, initial_col, Matrix); // setup size of Matrix
				setupMatrix_value(ID, Attrib, Attrib_val, Attrib_num, initial_row, initial_col, Matrix); // fill the values for the matrix

				WriteFile(initial_row,initial_col,Matrix, class_label, 1); // export a output file 

	         break;

	      case 2 :
				check_attrib_num(ID_testing, Attrib_num_test); // Check how many attributes each ID has
				//FindSameAttrib(ID_testing, Attrib_testing, Common_Attrib_test, Common_value, Attrib_val_testing, Attrib_num_test); // Find common Attribute.

				initial_row = ID_testing.back();
				initial_col = FindHighestNum(Attrib_testing) + 1;
				//cout << initial_row << " " << initial_col << endl;
				setSizeMatrix(initial_row, initial_col, Matrix_Test); // setup size of Matrix
				setupMatrix_value(ID_testing, Attrib_testing, Attrib_val_testing, Attrib_num_test, initial_row, initial_col, Matrix_Test); // fill the values for the matrix
				WriteFile(initial_row,initial_col,Matrix_Test, class_label, 2); // export a output file 

	         break;

	      case 3 :
  				check_attrib_num(ID_Sample, Attrib_num_Sample); // Check how many attributes each ID has
				//FindSameAttrib(ID_testing, Attrib_testing, Common_Attrib_test, Common_value, Attrib_val_testing, Attrib_num_test); // Find common Attribute.

				initial_row = ID_Sample.back();
				initial_col = FindHighestNum(Attrib_Sample) + 1;
				//cout << initial_row << " " << initial_col << endl;
				setSizeMatrix(initial_row, initial_col, Matrix_Sample); // setup size of Matrix
				setupMatrix_value(ID_Sample, Attrib_Sample, Attrib_val_Sample, Attrib_num_Sample, initial_row, initial_col, Matrix_Sample); // fill the values for the matrix
				WriteFile(initial_row,initial_col,Matrix_Sample, class_label, 3); // export a output file 

	         break;

	      case 4 :

	      	 break;
	    }

	}while(choice != 4);

	return 0;
}

void readFile(vector<float>& id, vector<float>& attrib, vector<float>& attrib_val, vector<float>& id_test, vector<float>& attrib_test, vector<float>& attrib_val_test, vector<int>& Class_label, vector<float>& id_sample, vector<float>& attrib_sample, vector<float>& attrib_val_sample)
{
	fstream reader;
	reader.open("training.txt", ios::in);
	string temp;
	string temp2;
	string temp3;

	while(reader)
	{
		getline(reader, temp, ' '); //get ID
		id.push_back(stof(temp));
		//cout << temp << " ";
		getline(reader, temp2, ' '); // get Attribute
		attrib.push_back(stof(temp2));
		//cout << temp2 << " ";
		getline(reader, temp3, '\n'); // get Feature value
		attrib_val.push_back(stof(temp3));
		//cout << temp3 << endl;
	}

	reader.close();

	reader.open("label_training.txt", ios::in);

	while(reader)
	{
		getline(reader, temp, '\n');
		Class_label.push_back(stoi(temp));
	}
	reader.close();

	reader.open("testing.txt", ios::in);
	
	while(reader)
	{
		getline(reader, temp, ' '); //get ID for testing
		id_test.push_back(stof(temp));
		//cout << temp << " ";
		getline(reader, temp2, ' '); // get Attribute for testing
		attrib_test.push_back(stof(temp2));
		//cout << temp2 << " ";
		getline(reader, temp3, '\n'); // get Feature value for testing
		attrib_val_test.push_back(stof(temp3));
		//cout << temp3 << endl;
	}
	reader.close();

	reader.open("test_sample.csv", ios::in);
	while(reader)
	{
		getline(reader, temp, ','); //get ID for testing
		id_sample.push_back(stof(temp));
		//cout << temp << " ";
		getline(reader, temp2, ','); // get Attribute for testing
		attrib_sample.push_back(stof(temp2));
		//cout << temp2 << " ";
		getline(reader, temp3, '\n'); // get Feature value for testing
		attrib_val_sample.push_back(stof(temp3));
		//cout << temp3 << endl;
	}
	reader.close();
}

void setSizeMatrix(int row, int col, vector<vector<float>>& matrix)
{
	matrix.resize(row);
	for (int i = 0; i < row; ++i)
	{
		matrix[i].resize(col);
	}

	for (int i = 0; i < row; ++i) // set up all default values are 0 in the beginning. Later, I replace 0 with the corresponding value in the setupMatrix_value funciton.
	{
		for (int j = 0; j < col; ++j)
		{
			matrix[i][j] = 0;
		}
	}

}

void setupMatrix_value(vector<float>& id, vector<float>& attrib, vector<float>& attrib_val, vector<int>& num, int row, int col, vector<vector<float>>& matrix)
{
	vector<float> temp = {0};
	float temp2 = 0;
	float temp3 = 0;

	for (int i = 0; i < num.size(); ++i)
	{
		temp.push_back(num[i]);
	}

	for (int i = 0; i < row-1; ++i)
	{
		matrix[i][0] = i+1; // all the first entry are ID.
		temp2 += temp[i];
		temp3 = temp2 + temp[i+1];

		for (float j = temp2; j < temp3; ++j)
		{

			matrix[i][attrib[j]] = attrib_val[j]; // replace 0 with the corresponding value.
		}
	}

	temp2 += temp[row-1];
	temp3 = temp2 + num[row-1];
	//cout << temp2 << " " << temp3 << endl;

	matrix[row-1][0] = row; 

	for (int j = temp2; j < temp3; ++j)
	{
		matrix[row-1][attrib[j]] = attrib_val[j]; // replace 0 with the corresponding value.
	}


}

void check_attrib_num(vector<float>& id,vector<int>& num)
{
	int amount = 0;

	for (int i = 0; i < id.size(); ++i)
	{
		if (id[i] == id[i+1])
		{
			amount ++;
		}
		else
		{
			num.push_back(amount+1);
			amount = 0;
		}
	}
}

void WriteFile(int row,  int col, vector< vector<float> > &matrix, vector<int>& Class_label, int index)
{
	fstream write;

	switch(index)
	{
		case 1 :
				write.open("Output_training.csv", ios::out);
				write << fixed << setprecision(2) << setw(5) << "ID" << ",";

			for (int j = 1; j < col; ++j)
				{
					write << fixed << setprecision(2) << setw(7) << "Attr" << j <<",";
				}
				write << fixed << setprecision(2) << setw(7) << "Class_label";
				write << endl;

			for (int i = 0; i < row; ++i)  
			{
				for (int j = 0; j < col; ++j)
				{
					write << fixed << setprecision(2) << setw(7) << matrix[i][j]<<",";
				}
				write << fixed << setprecision(2) << setw(7) << "Class-" << Class_label[i];
				write << endl;
			}
			write.close();
		break;

		case 2 :
			write.open("Output_test.csv", ios::out);
			write << fixed << setprecision(2) << setw(5) << "ID" << ",";
		for (int j = 1; j < col; ++j)
			{
				write << fixed << setprecision(2) << setw(7) << "Attr" << j <<",";
			}
			//write << fixed << setprecision(2) << setw(7) << "Class_label";
			write << endl;

		for (int i = 0; i < row; ++i)  
		{
			for (int j = 0; j < col; ++j)
			{
				write << fixed << setprecision(2) << setw(7) << matrix[i][j]<<",";
			}
			//write << fixed << setprecision(2) << setw(7) << "Class-" << Class_label[i];
			write << endl;
		}
		write.close();
		break;

		case 3 :
			write.open("Output_sample.csv", ios::out);
			write << fixed << setprecision(2) << setw(5) << "ID" << ",";
		for (int j = 1; j < col; ++j)
			{
				write << fixed << setprecision(2) << setw(7) << "Attr" << j <<",";
			}
			//write << fixed << setprecision(2) << setw(7) << "Class_label";
			write << endl;

		for (int i = 0; i < row; ++i)  
		{
			for (int j = 0; j < col; ++j)
			{
				write << fixed << setprecision(2) << setw(7) << matrix[i][j]<<",";
			}
			//write << fixed << setprecision(2) << setw(7) << "Class-" << Class_label[i];
			write << endl;
		}
		write.close();
		break;

	}

}

void FindSameAttrib(vector<float>& id, vector<float>& attrib, vector<int>& common_attrib, vector<float>& common_value, vector<float>& attrib_val, vector<int>& attrib_num)
{
	int count;

	for (int i = 0; i < attrib_num[0]; ++i)
	{

		for (int j = 0; j < attrib.size(); ++j)
		{
			if (attrib[i] == attrib[j])
			{
				count ++;
			}

			if (count == (id.back()-1) ) // if the # of attrib is 1841 = common attribute
			{	
				//cout << attrib[i] << " " << attrib_val[i] << endl;
				common_attrib.push_back(attrib[i]);
				//dataset.push_back(make_pair(temp, temp2));

				count = 0;
			}
		}
	}

}

int FindHighestNum(vector<float>& attrib)
{
	string helper;
	int helper2;
	string expected_highest_value; // assume last element of the attribute value is the highest number.
	helper2 = attrib.back();
	expected_highest_value = to_string(helper2);
	char helper3;
	vector<float> temp;
	int highest_value = 0;

	for (int i = 0; i < attrib.size(); ++i)
	{
		helper2 = attrib[i];
		helper = to_string(helper2);
		helper3 = helper[2];
		helper2 = (int)helper3 -48;

		//cout << helper << " " << helper[0] << " " << helper[1] << endl;
 
		if (helper[0] == expected_highest_value[0] && helper[1] == expected_highest_value[1]) // check first, second digit are same
		{
			if (helper.size() >= expected_highest_value.size() && helper[2] > expected_highest_value[2]) // check string length is bigger or equal to the expected value with 3rd value.
			{
				temp.push_back(stof(helper)); // if the string length is bigger or equal, and 3rd value is bigger than 3rd value of expected value, put it into array.
			}
		}
	}
	if (temp.size() == 0)
	{
		highest_value = attrib.back();
	}
	else
	{
		insertion_sort(temp, temp.size()); // sort and find biggest value among values which are bigger than the expected value
		highest_value = temp.back();
	}	

	return highest_value;
}

void insertion_sort (vector<float>& input, int length)
{
 	float j, temp;
		
	for (int i = 0; i < length; i++)
	{
		j = i;
		
		while (j > 0 && input[j] < input[j-1])
		{
		  temp = input[j];
		  input[j] = input[j-1];
		  input[j-1] = temp;
		  j--;
	    }
	}
}
