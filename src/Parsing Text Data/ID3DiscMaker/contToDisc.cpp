#include<iostream>
#include<fstream>
#include<list>
#include<math.h>

using namespace std;

float Entropy=0.552011;
float totalLess=24720;
float totalGreat=7841;
float totalInstances=32561;

typedef struct{
    int continiousVariable;
    int label;
}element;

typedef struct{
    float class0; //!instances with label=2;
    float class1; //! instance with label=1
    float total; //! Total instances in Set
}set;

set First,Second;
bool compareContiniousVariable(const element &a,const element &b){
    return  a.continiousVariable < b.continiousVariable;
    
}

float entropy(set S1){
    
    // cout<<S1.class0<<" "<<S1.class1<<" "<<S1.total<<endl;
    float p0=(S1.class0/S1.total);
    float p1=(S1.class1/S1.total);
    
    float ent= -1* (p0*log(p0) + p1*log(p1));
    return ent;
}

float InformationGain(){
    float term0= (First.total / totalInstances) * entropy(First);
    
    float term1= (Second.total / totalInstances) * entropy(Second);
    //cout<<"Information Gain "<<term0<<" "<<term1<<endl;
    float IG= Entropy- (term0+term1);
    return IG;
    
}


int main(int argc,char *argv[]){
    ifstream dataFile,labelFile;
    ofstream ofile;
    ofile.open("../../../Data/ID3BoolSeperators.txt");
    ofile << "Generated by conttoDisc.cpp in src/Parsing Text Data/ID3DiscMaker\n";
    
    dataFile.open("../../../Data/trainData.txt");
    labelFile.open("../../../Data/trainOutput.txt");
    int instances=32561;
    
    std::list<element> variables;
    std::list<int> contIndices;
    
    contIndices.push_back(0);
    contIndices.push_back(2);
    contIndices.push_back(4);
    contIndices.push_back(10);
    contIndices.push_back(11);
    contIndices.push_back(12);
    
    element temp;
    int dummy;
    
    for( int index : contIndices){
        
        variables.clear();
        dataFile.seekg(ios_base::beg);
        labelFile.seekg(ios_base::beg);
        
        for(int j=0;j<instances;j++){
            
            for(int i=0;i<14;i++){
                dataFile >> dummy;
                if(i==index)
                    temp.continiousVariable=dummy;
            }
            labelFile >> temp.label;
            
            variables.push_back(temp);
        }
        
        variables.sort(&compareContiniousVariable);
        
        while(variables.front().continiousVariable == -1)
            variables.pop_front();
        
        int prevLabel=variables.front().label;
        int prevValue=variables.front().continiousVariable;
        // cout<<prevLabel<< " " <<prevValue<<endl;
        
        float sepValue=prevValue;
        float maxIG=0;
        
        First.class0=0;
        First.class1=0;
        First.total=0;
        
        Second.class0=totalLess;
        Second.class1=totalGreat;
        Second.total=totalInstances;
        
        
        for(element n : variables){
            
            First.total++;
            Second.total--;
            
            if(n.label==0){
                First.class0++;
                Second.class0--;
            }
            else{
                First.class1++;
                Second.class1--;
            }
            
            if(n.label!=prevLabel){
                //cout<<prevValue<<" "<<n.continiousVariable<<endl;
                prevValue=n.continiousVariable;
                prevLabel=n.label;
                float IG=InformationGain();
                if(isnan(IG))
                    IG=0;
                if(IG>maxIG){
                    maxIG=IG;
                    sepValue=(prevValue+n.continiousVariable)/2;
                }
            }
            
        }
        ofile<<"Index ["<<index<<"] will be seperated at "<<sepValue<<endl;
    }
    
    ofile.close();
    dataFile.close();
    labelFile.close();
    return 0;
}