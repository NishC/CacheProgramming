//authors: Nishant Chandrashekar, Shyam Pinnipati

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
	int used;
	CacheLine(): tag(0), used(0), dirty(0) 
	{
		for(int a=0; a<16;a++)
		{		
			data[a]= '0';
		}
	}
};

int nToB10(const char source[], const int &source_size, const int &source_base);

int minc(int, int, int, int);

int main(int argc,char *argv[])
{

	//input from file recieved all at one time

	vector <string> CacheIAddress;
	vector <string> CacheIWrite;
	vector <string> CacheIData;

	int counter = 1;

	char RAM[0x20000];
	for(int i = 0; i < 0x20000; i++)
	{
		RAM[i] = '0';
	}
	ifstream cFile (argv[1]);
	ofstream fFile ("sa-out.txt");

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

	CacheLine Cache[16][4];

	for(int i = 0; i < 16; i++)
	{

		for (int h =0; h<4; h++)
		{
			for(int j = 0; j < 15; j++)
			{
				Cache[i][h].data[j] = '0';
			}
		}
	}


	int hit = 0;//if false hit = 0
	vector<string>::iterator j = CacheIWrite.begin();
	vector<string>::iterator k = CacheIData.begin();
	for(vector<string>::iterator i = CacheIAddress.begin(); i != CacheIAddress.end(); i++)
	{
		int index = (nToB10(i->c_str(), 4, 16) >> 3) & 15;
		int off = (nToB10(i->c_str(), 4, 16))& 7 ;
		unsigned int tag = (nToB10(i->c_str(), 4, 16) >> 7) & 511;
		int dbit = 0;

		int dbt;
		for(int u = 0; u < 4; u++)
		{

			if(Cache[index][u].tag == tag)
			{
				dbt = u;
				hit = 1;
				break;
			}
			else {hit =0;}
		}



		dbit = Cache[index][dbt].dirty;

		if(hit ==0)
		{

			dbt = minc(Cache[index][0].used,Cache[index][1].used,Cache[index][2].used,Cache[index][3].used);
			dbit = Cache[index][dbt].dirty;
			int ramLoc = (nToB10(i->c_str(), 4, 16) >> 3)<<3;
			ramLoc = 2*ramLoc;
			if(Cache[index][dbt].dirty== 1)//check dirty bit equals 1
			{
				int oldRamLoc = 2*((Cache[index][dbt].tag<<7)|(index << 3));
				memcpy(&RAM[oldRamLoc],&Cache[index][dbt].data,16);  
			}


			Cache[index][dbt].dirty= 0; // clear dirty bit
			Cache[index][dbt].tag = tag; //upload new tag
			memcpy(&Cache[index][dbt].data, &RAM[ramLoc], 16);
		}
		//load request line***

		Cache[index][dbt].used=counter;
		counter++;

		if(*j == "FF")
		{
			Cache[index][dbt].dirty = 1;
			Cache[index][dbt].data[16-(2*off)-1] = (*k)[1];
			Cache[index][dbt].data[16-(2*off)-2] = (*k)[0];

		} else if(*j == "00") {
			fFile<<*i<<" ";
			for(int i = 0; i < 16; i++)
			{
				fFile << Cache[index][dbt].data[i];
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

int minc(int u1, int u2, int u3, int u4)
{
	int minz;
	int minimumV = min(min(min(u1,u2),u3),u4);
	if(u1 == minimumV){minz = 0;}
	else if(u2 == minimumV){minz = 1;}
	else if(u3 == minimumV){minz = 2;}
	else if(u4 == minimumV){minz = 3;}
	else {cout<<minimumV<<"problem"<<u1<<" "<<u2<<" "<<u3<<" "<<u4<<endl;}

	return minz;
}