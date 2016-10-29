//authors: Nishant Chandrashekar, Shyam Pinnipati

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	//recieve input from first 16 lines
	string RAM[4];
    bool hit[4] = {false, false, false, false};
    int clock = 0;
    string addresses[16];
    ifstream reading(argv[1]);
    ofstream out("vm-out.txt");
    for(int i = 0; i < 16; i++)
    {
        reading >> addresses[i];
        //addresses[i] = addresses[i].substr(5);
    }
    while(1)
    {
        bool replaced = false;
        bool noFault = false;
        string readIn;
        int currAddress = -1;
        reading >> readIn;
        if(reading.eof())
        {
            break;
        }
        for(int i = 0; i < 16; i++)
        {
            if(readIn.substr(0,readIn.size()-3) == addresses[i].substr(0,addresses[i].size()-3))
            {
                currAddress = i;
                break; //get the address
            }     
        }
        
        for(int i = 0; i< 4; i++) 
        {
            if(currAddress >= 0 && addresses[currAddress] == RAM[i])
            {
                noFault = true;
                hit[i] = true;
                break;
            }
        }
        
        if(noFault)
        {
            for(int i = 0; i< 4; i++) 
            {
                if(!RAM[i].empty())
                {
                    out << RAM[i];
                    if(i < 3)
                    {
                        out << " ";
                    }
                }
            }
            out << endl;
            continue;
        }
        
        int lastFilled;
        
        for(int i = 0; i< 4; i++) //fill anything empty
        {
            if(RAM[i].empty())
            {
                RAM[i] = addresses[currAddress];
                hit[i] = true;
                clock++;
                if(clock > 3)
                {
                    clock = 0;
                }
                replaced = true;
                lastFilled = i;
                break;
            }
        }
        
        if(replaced)
        {
            for(int i = 0; i< 4; i++)
            {
                if(!RAM[i].empty())
                {
                    out << RAM[i];
                    if(i < lastFilled)
                    {
                        out << " ";
                    }
                }
            }
            out << endl;
            continue;
        }
        
        bool replacenum = false;
        
        while(!replacenum)
        {
            if(hit[clock] == false)
            {
                RAM[clock] = addresses[currAddress];
                hit[clock] = true;
                clock++;
                if(clock > 3)
                {
                    clock = 0;
                }
                for(int i = 0; i< 4; i++)
                {
                    if(!RAM[i].empty())
                    {
                        out << RAM[i];
                        if(i < 3)
                        {
                            out << " ";
                        }
                    }
                }
                out << endl;
                replacenum = true;
            }
            else
            {
                hit[clock] = false;
                clock++;
                if(clock > 3)
                {
                    clock = 0;
                }
            }
        }
    }
    
    reading.close();
    out.close();
    
}