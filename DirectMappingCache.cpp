//Authors: Nishant Chandrashekar, Shyam Pinnipati

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

struct CacheLine
{
	int tag;
	char data[16];
	int  dirty;
	CacheLine(): tag(0), dirty(0) {}
};

int nToB10(const char source[], const int &source_size, const int &source_base);

int main(int argc,char *argv[])
{

	//input from file recieved all at one time

	vector <string> CacheIAddress;
	vector <string> CacheIWrite;
	vector <string> CacheIData;
	
	
	char RAM[0x20000];
	for(int i = 0; i < 0x20000; i++)
	{
		RAM[i] = '0';
	}
	ifstream cFile (argv[1]);
	ofstream fFile ("dm-out.txt");

	while(!cFile.eof())
	{
		string stril = "";
		cFile>>stril;
		CacheIAddress.push_back(stril);
		cFile>>stril;
		CacheIWrite.push_back(stril);
		cFile>>stril;
		CacheIData.push_back(stril);

	}
	CacheIAddress.pop_back();

	//Close the unneeded input fible
	cFile.close();

	CacheLine Cache[64];
	
	for(int i = 0; i < 63; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			Cache[i].data[j] = '0';
		}
	}


	int hit = 0;//if false hit = 0
	vector<string>::iterator j = CacheIWrite.begin();
	vector<string>::iterator k = CacheIData.begin();
	for(vector<string>::iterator i = CacheIAddress.begin(); i != CacheIAddress.end(); i++)
	{
		int index = (nToB10(i->c_str(), 4, 16) >> 3) & 63;
		int off = (nToB10(i->c_str(), 4, 16))& 7 ;
		int tag = (nToB10(i->c_str(), 4, 16) >> 9) & 127;
		int dbit = 0;
		//cout << tag << ' ' << index << ' ' << off << endl;


		if(Cache[index].tag == tag)
		{
			hit = 1;
		}
		else {hit =0;}
		dbit = Cache[index].dirty;
		if(hit ==0)
		{
			int ramLoc = (nToB10(i->c_str(), 4, 16) >> 3)<<3;
			

			if(Cache[index].dirty== 1)//check dirty bit equals 1
			{
				int oldRamLoc =(Cache[index].tag<<9)|(index << 3);
				memcpy(&RAM[oldRamLoc],&Cache[index].data,16);	
			}

			//dbit = Cache[index].dirty;
			Cache[index].dirty= 0; // clear dirty bit
			Cache[index].tag = tag; //upload new tag
			memcpy(&Cache[index].data, &RAM[ramLoc], 16);


		}
		//load request line***


		if(*j == "FF")
		{
			Cache[index].dirty = 1;


			Cache[index].data[16-(2*off)-1] = (*k)[1];
			Cache[index].data[16-(2*off)-2] = (*k)[0];

		} else if(*j == "00") {
			fFile<<*i<<" ";
			for(int i = 0; i < 16; i++)
			{
				fFile << Cache[index].data[i];
			}
			fFile<<" "<< hit<<" "<<dbit<<endl;
		}
		else {}
		j++;
		k++;
	}


	//File Close after operation
	fFile.close();

}

int nToB10(const char source[], const int &source_size, const int &source_base)
{

	int value = 0;
	for(int i = 0; i < source_size; i++)
	{
		if ((source[i]) < 58)
			value = value * source_base + (source[i] - 48);
		else
			value = value * source_base + (source[i] - 55);
	}
	return value;
}