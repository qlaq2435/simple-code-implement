#include"B+Tree.h"
#include<iostream>

int main(){
    
    int aaa[20];
    for (int i = 0; i < 20; i++){
        aaa[i] = i*i %11;
        std::cout<<aaa[i]<<" ";
    }
    std::cout<<std::endl<<std::endl;
    
    BPT<int,int> t(5);

    for(int i = 0;i<8;i++){
        std::cout<<aaa[i]<<std::endl;
        t.insertPair(aaa[i],aaa+i);
        t.printBPT();
        std::cout<<"-----------"<<std::endl;
    }



    return 0;
}
