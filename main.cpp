#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iomanip>

using namespace std;

void readFile(vector<float>& id, vector<float>& attrib, vector<float>& attrib_val, vector<int>& Class_label);
void setSizeMatrix(int row, int col, vector< vector<float> > &matrix);
void setupMatrix_value(vector<float>& id, vector<float>& attrib, vector<float>& attrib_val, vector<int>& num, int row, int col, vector< vector<float> >&matrix);
void check_attrib_num(vector<float>& id,vector<int>& num);
void WriteFile( int row, int col, vector< vector<float> > &matrix, vector<int>& Class_label);
void FindSameAttrib(vector<float>& attrib, vector<int>& common_attrib, vector<float>& common_value, vector<float>& attrib_val, vector<int>& attrib_num);

int main()
{
	int initial_row = 1842;
	int initial_col = 26428;
	vector<float> ID;
	vector<float> Attrib;
	vector<float> Attrib_val;
	vector<int>Attrib_num; // # of attribute per each ID
	vector<int>Common_Attrib; // contain only common attribute
	vector<int>class_label;
	vector<float>Common_value = {0}; // ignore this one

	vector<vector<float> > Matrix; 

	readFile(ID, Attrib, Attrib_val, class_label); // Read input files
	check_attrib_num(ID, Attrib_num); // Check how many attributes each ID has
	FindSameAttrib(Attrib, Common_Attrib, Common_value, Attrib_val, Attrib_num); // Find common Attribute.
	setSizeMatrix(initial_row, initial_col, Matrix); // setup size of Matrix

	setupMatrix_value(ID, Attrib, Attrib_val, Attrib_num, initial_row, initial_col, Matrix); // fill the values for the matrix

	WriteFile(initial_row,initial_col,Matrix, class_label); // export a output file 

	return 0;
}

void readFile(vector<float>& id, vector<float>& attrib, vector<float>& attrib_val, vector<int>& Class_label)
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

void WriteFile(int row,  int col, vector< vector<float> > &matrix, vector<int>& Class_label)
{
	fstream write;
	write.open("Output.txt", ios::out);
	for (int j = 0; j < col; ++j)
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

/*
	for (int i = 0; i < row; ++i)  
	{
		for (int j = 0; j < common_attrib.size(); ++j)
		{
			write << fixed << setprecision(2) << setw(7) << matrix[i][j] << ",";
		}
		write << endl;
	}
	write.close();
*/
}

void FindSameAttrib(vector<float>& attrib, vector<int>& common_attrib, vector<float>& common_value, vector<float>& attrib_val, vector<int>& attrib_num)
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

			if (count == 1841) // if the # of attrib is 1841 = common attribute
			{	
				//cout << attrib[i] << " " << attrib_val[i] << endl;
				common_attrib.push_back(attrib[i]);
				//dataset.push_back(make_pair(temp, temp2));

				count = 0;
			}
		}
	}

}
