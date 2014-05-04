#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>

using namespace std;

//CopyFile is a simple function that copies a file from arg1 to arg2
int CopyFile(string initialFilePath, string outputFilePath)
{
	
	ifstream initialFile(initialFilePath.c_str(), ios::in|ios::binary);
	ofstream outputFile(outputFilePath.c_str(), ios::out|ios::binary);
	//defines the size of the buffer
	initialFile.seekg(0, ios::end);
	long fileSize = initialFile.tellg();
	//Requests the buffer of the predefined size


	//As long as both the input and output files are open...
	if(initialFile.is_open() && outputFile.is_open())
	{
		short * buffer = new short[fileSize/2];
		//Determine the file's size
		//Then starts from the beginning
		initialFile.seekg(0, ios::beg);
		//Then read enough of the file to fill the buffer
		initialFile.read((char*)buffer, fileSize);
		//And then write out all that was read
		outputFile.write((char*)buffer, fileSize);
		delete[] buffer;
	}
	//If there were any problems with the copying process, let the user know
	else if(!outputFile.is_open())
	{
		cout<<"I couldn't open "<<outputFilePath<<" for copying!\n";
		return 0;
	}
	else if(!initialFile.is_open())
	{
		cout<<"I couldn't open "<<initialFilePath<<" for copying!\n";
		return 0;
	}
		
	initialFile.close();
	outputFile.close();

	return 1;
}