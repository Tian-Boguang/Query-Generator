#pragma once

#ifndef PATHGENERATOR_H_
#define PATHGENERATOR_H_

#include <iostream>
#include "DIGraph.h"
#include "DataRead.h"
#include <time.h>
#include <string.h>

using namespace std;


template <class VLabelType, class ELabelType>
class PathGenerator
{
public:
    DIGRAPH<VLabelType, ELabelType>* graph_ptr;
    bool flag;
    typedef unordered_map<VertexID, VLabelType> VLabels;
	PathGenerator(DataRead<VLabelType, ELabelType> *, string, int, int, string, string);
    void BuildPath(int**, int, int, int, int, VertexID);
  	~PathGenerator();
};

template <class VLabelType, class ELabelType>
PathGenerator<VLabelType, ELabelType>::PathGenerator(DataRead<VLabelType, ELabelType> *Graph, string GraphName, int pathlength, int pathnum, string No, string labelsize)
{   
    //int size[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    this->graph_ptr = Graph->graph_ptr;   
    VertexID root, vertex;
    int MaxNum, Number;
    string OutFileName;
    string PathName;
    string S = "Query-Slashdot/S"+ No + "-" + labelsize;
    string D = "Query-DBLP/D"+ No + "-" + labelsize;
    string T = "Query-Twitter/T"+ No + "-" + labelsize;
    if(GraphName == S)
        PathName = "Path-Slashdot/S";
    if(GraphName == D)
        PathName = "Path-DBLP/D";
    if(GraphName == T)
        PathName = "Path-Twitter/T";
    OutFileName = PathName + No + "-" + labelsize;
    cout << "The outfile name is " << OutFileName <<endl;

    
    int **Path = new int*[pathnum];
    for(int i = 0; i < pathnum; i++){
        Path[i] = new int[pathlength];
    }

    for(int i =0; i <pathnum; i++){
        this->flag = false;
        BuildPath(Path, pathlength, pathnum, 0, i, -1);
        cout << "Path " << i << " finished!" <<endl;
    }
    

    ofstream OutFile(OutFileName);
    OutFile << "# Undirected path: ../../PathGen-"<< No <<"\r\n";
    OutFile << "# Path " << "\r\n";
    OutFile << "# Nodes:  Edges: " <<"\r\n";
    OutFile << "# pathnum\tlabelnum\tlabel";
    for(int i = 0; i < pathnum; i++){
        OutFile << "\r\n" << i << "\t" << pathlength;
        for(int j = 0; j < pathlength; j++){
            OutFile << "\t" << Path[i][j];
        }
    }
    OutFile.close();
    

    for(int i = 0; i < pathnum; i++)
        delete[] Path[i];
    delete[] Path;
}

template <class VLabelType, class ELabelType>
void PathGenerator<VLabelType, ELabelType>::BuildPath(int** Path, int pathlength, int pathnum, int templength, int tempnum, VertexID tempvertex){
    if(this->flag)
        return;


    if(templength == pathlength){
        bool test1 = true;
        for(int i = 0; i < tempnum; i++){
            bool test2 = false;
            for(int j = 0; j < pathlength; j++){
                if(Path[i][j] != Path[tempnum][j])
                    test2 = true;
            }
            if(!test2)
                test1 = false;
        }
        if(test1)
            this->flag = true;
    }



    if(templength == 0){
        for(auto it = this->graph_ptr->getVLabel().begin();it != this->graph_ptr->getVLabel().end();it++){
            Path[tempnum][0] = it->second;            
            BuildPath(Path, pathlength, pathnum, templength+1, tempnum, it->first);
            if(this->flag)
                return;
            

        }
    }else{   
        for(auto it2 = this->graph_ptr->getOutEdge()[tempvertex].begin(); it2 != this->graph_ptr->getOutEdge()[tempvertex].end(); it2++){
            VLabelType templabel = this->graph_ptr->getVLabel()[it2->first];
            bool test = false;
            for(int i = 0; i < templength; i++){
                if(templabel == Path[tempnum][i])
                    test = true;
            }
            if(test)
                return;

            Path[tempnum][templength] = templabel;
            BuildPath(Path, pathlength, pathnum, templength+1, tempnum, it2->first);
            if(this->flag)
                return;
        }
    }




}

template <class VLabelType, class ELabelType>
PathGenerator<VLabelType, ELabelType>::~PathGenerator(){}

#endif