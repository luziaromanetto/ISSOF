/*
 * Copyright (C) Luzia de Menezes Romanetto 2015 <luziaromanetto at gmail dot com>
 * Copyright (C) Luis Gustavo Nonato 2015 <gnonato at icmc dot usp dot br>
 * Copyright (C) Afonso Paiva Neto 2015 < apneto at icmc dot usp dot br> 
 *
 * OFISS (Isosuface Stuffing based on OF) is free software
 */
#include <iostream>
#include <sstream>
#include <functional>

#include "of/of.h"

using namespace std;
using namespace of;

#ifndef IPT_H
#define IPT_H

class Point{
	private:
		int type;        // type==0 é um ponto de canto, 
						 // type==1 é um ponto de centro.
		double value;    // function value.
	public:
		Point(){
			type = -1;
			value = 0;
		};
		
		Point(int t, double v){
			type = t;
			value = v;			
		};
};

#endif


#ifndef ISS_H
#define ISS_H

class ofISS3D{
	private:
		int levels; 					// Number of degree levels
		double **bbox,					// Bounding box for build the mesh
				alpha_short, alpha_long,// Isosurface Stuffing parameters
				dx;						// Minimal mesh size
		ofMesh<ofDefault3D> *mesh;  	// Pointer to mesh
		function<double(double*)> f;    // Level function
		char output[50];
		
	public:
	
		ofISS3D(ofMesh<ofDefault3D> *_mesh){
			levels = 1;
			bbox = new double*[2];
			for( int i=0; i<3; i++){
				bbox[i] = new double[3];
			}
			alpha_short = 0.35900;
			alpha_long = 0.21509;
			mesh = _mesh;
		};
		
		bool readConfigurationFile(char fileName[]);
};

bool ofISS3D::readConfigurationFile(char fileName[]){
	ifstream fin(fileName);
    string line;
    istringstream sin;
    string aux;
    
    if (!fin) std::cerr << "Could not open the file!" << std::endl;

    string tag[7] = {"-xi ", "-xf ", "-le ", "-dx ", "-al ", "-as ", "-out "};
    bool tagFound[7] = {false,false,false,false,false,false,false};

    while (getline(fin, line) ) {
        sin.str(line.substr(line.find("=")+1));
        //cout << line << endl;
        if (line.find(tag[0]) != std::string::npos) {
            sin >> bbox[0][0] >> bbox[0][1] >> bbox[0][2];
            tagFound[0]=true;
        }
        else if(line.find(tag[1]) != std::string::npos){
            sin >> bbox[1][0] >> bbox[1][1] >> bbox[1][2];
            tagFound[1]=true;
        }
        else if(line.find(tag[2]) != std::string::npos){
            sin >> levels;
            tagFound[2]=true;
        }
        else if(line.find(tag[3]) != std::string::npos){
            sin >> dx;
            tagFound[3]=true;
        }
        else if(line.find(tag[4]) != std::string::npos){
            sin >> alpha_long;
            tagFound[4]=true;
        }
        else if(line.find(tag[5]) != std::string::npos){
            sin >> alpha_short;
            tagFound[5]=true;
        }
        else if(line.find(tag[6]) != std::string::npos){
            sin >> output;
            tagFound[6]=true;
        }

        sin.clear();
    }

    for( int i=0; i<7; i++){
        if( !tagFound[i] ){
            cout << "ERROR: tag \""<< tag[i] << "\" was not found!!" << endl;
        }
    }
    
	return true;
};

#endif
