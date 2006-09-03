#include <iostream>
#include <string>
#include <zlib.h>

#include "ciso.h"

using namespace std;

int main(int argc, char *argv[])
{
	int level;
	int result;
	string filenameIn;
	string filenameOut;

	if(argc != 4)
	{
		cout << "Usage: ciso level infile outfile" << endl;
		cout << "  level: 1-9 compress ISO to CSO (1=fast/arge - 9=small/slow" << endl;
		cout << "         0   decompress CSO to ISO" << endl;
		return 0;
	}
	
	level = argv[1][0] - '0';
	if(level < 0 || level > 9)
	{
        cerr << "Unknown mode: " << argv[1][0] << endl;
		return 1;
	}

	filenameIn = argv[2];
	filenameOut = argv[3];

	CIso isoCompressor;

    try
    {
    	if(level==0)
    	{
    		isoCompressor.decompress(filenameIn, filenameOut);
    		cout << "ciso decompress completed" << endl;
        }
    	else
    	{
    		isoCompressor.compress(filenameIn, filenameOut, level);
    		cout << "ciso compress completed" << endl;
    	}
    }
    catch(CIsoException& e)
    {
        cout << "Error: " << e.what() << endl;
    }

	return 0;
}
