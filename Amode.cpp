#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;

float **createDataMatrix(int numElement, int numSample)
{
    // Create a 2D data matrix of size numElement and numSample 

    float **RFData;  //initialize double pointer variable
    RFData = new float* [numElement];  //allocate an array of pointer float variables

    for (int i=0; i<numElement; i++)
    {
        RFData[i] = new float[numSample]; //for each pointer in the array of pointers allocate an array of float variables
    } 


    return RFData; //return array
    
}

int loadRFData(float **RFData, const char *fileName, int numElement, int numSample)
{
    ifstream infile; //
    infile.open(fileName);
    char line[100];
    float numbers[numSample];



    if (infile.fail())
	{
        return -1;
	}

    for(int x=0; x<numElement; x++)
    {
        for(int i=0; i<numSample; i++)
        {
            infile.getline(line, 100);
            numbers[i]=atof(line);
        }

        RFData[x]=numbers;
    
    }

    infile.close();
    return 0;

    // Open the text file fileName, read the data and store into RFData
    // You can use the getline() command to extract the data lines from the txt files
}

// Create an array containing the depth location (in z-direction) for each pixel on the scanline
float *genScanlineLocation(int &numPixel)
{
    int depth;
//int pixels;

    cout << "What is the desired imaging depth?";
    cin >> depth;
    
    cout << "How many pixels is the scanline?";
    cin >> numPixel;

    float scanlineLocation[numPixel];

    float increment = (numPixel-1)/depth;
    float newDepth=0;
    
    for(int i=0; i<numPixel; i++)

    {
        scanlineLocation[i]=newDepth;
        newDepth+=increment;
    }


    return scanlineLocation;

}

// Create an array containing the element location (in x-direction) of the ultrasound transducer
float *genElementLocation(int numElement, float PITCH)
{
    float eleLocation[numElement];

    for (int n=0; n<numElement; n++)
    {
        eleLocation[n] = (n-(numElement-1)/2)*PITCH;
        cout << eleLocation[n];
    }
}

// Allocate memory to store the beamformed scanline
float *createScanline(int numPixel)
{
    float *scanline;
    scanline = new float[numPixel]
    return scanline;
}

// Beamform the A-mode scanline
void beamform(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel, float FS, float SoS)
{

    float tForward[numPixel];
    float tBackward[numPixel, numElement];
    float tTotal[numPixel, numElement];

    int **s;
    s = new int*[numPixel];

    float *pReal;
    pReal = new float [numPixel];
    float *pImag;
    pImag = new float [numPixel];


    for (int i = 0; i<numPixel; i++)
    {   
        pReal[i] = 0;
        pImag[i] = 0;

        tForward[i] = scanlinePosition[i]/SoS;
        
        for (int k = 0; k<numElement; k++)
        {
            tBackward[i][k] = (sqrt(pow(scanlinePosition[i], 2) + pow(elementPosition[k], 2)))/SoS;
            tTotal[i][k] = tForward[i] + tBackward[i][k];
            s[i][k]=floor(time[i][k]*FS);
            pReal[i] += realRFData[k][s[i][k]];
            pImag[i] += imagRFData[k][s[i][k]];
        }
        scanline[i] = sqrt(pow(pReal[i], 2) + pow(pImag[i], 2));
    }


    
    
}

// Write the scanline to a csv file
int outputScanline(const char *fileName, float *scanlinePosition, float *scanline, int numPixel)
{
    ofstream outfile;
    outfile.open(fileName);

    if (outfile.fail())
    {
        return -1;
    }

    for(int i = 0; i<numPixel; i++)
    {
        outfile << scanlinePosition[i];
        outfile << ", ";
        outfile << scanline[i];
        outfile << endl;
    }

    outfile.close();

    return 0;
}

// Destroy all the allocated memory
void destroyAllArrays(float *scanline, float **realRFData, float **imagRFData, float *scanlinePosition, float *elementPosition, int numElement, int numSample, int numPixel)
{
    delete scanline;
    delete scanlinePosition;
    delete elementPosition;

    for( int i=o; i<numElement; i++)
    {
        delete realRFData[i];
        delete imagRFData[i];
    }
}