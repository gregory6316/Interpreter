#include <iostream>
#include <fstream>
#include <string.h>
#include "Executer.h"

using namespace std;

int main ()
{
    
    int i;
    fin.open("program.txt");
    try
    {
        Interpretator A("program.txt");
        A.interpretation();
    }
    catch (char t) {}
    catch (Exception e)
    {
        e.error();
        return 5;
    }
	  cout << "\n";
	}

	
