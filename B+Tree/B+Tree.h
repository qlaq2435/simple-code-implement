#include"B+TreeNode.h"
#include<vector>

template<typename Ktype, typename Vtype>
class BPT{

    public:
        BPT(int ord);
        ~BPT();
        bool insertPair(Ktype key,Vtype * data);
        bool deletePair(Ktype key,Vtype * data);
        bool openScan();
        Vtype * getNextEntry();
        
    
    private: 
        BPTnode<Ktype,Vtype> * root;
        BPTnode<Ktype,Vtype> * firstleaf;
        BPTnode<Ktype,Vtype> * findLeaf(Ktype key,Vtype * data);
        bool checkUp(BPTnode<Ktype,Vtype> * nowNode);
        void maintainMinEntry(BPTnode<Ktype,Vtype> * nowNode);
        void mergeNode(BPTnode<Ktype,Vtype> * nowNode);
        void devideNode(BPTnode<Ktype,Vtype> * nowNode);
        
        int order;
        int nodenum;
};

template<typename Ktype, typename Vtype>
BPT<Ktype,Vtype>::BPT(int ord){
    order = ord;
    root = new BPTnode<Ktype,Vtype>(order,order>>1,true,true);
    root->setInBPT(true);
    nodenum = 1;
    firstleaf = root;
}

template<typename Ktype, typename Vtype>
BPT<Ktype,Vtype>::~BPT(){
    // 遍历整个B+树将所有节点释放
}

template<typename Ktype, typename Vtype>
bool BPT<Ktype,Vtype>::insertPair(Ktype key,Vtype * data){
    BPTnode<Ktype,Vtype> * leafLocate = findLeaf(key, data);
    int changedindex = leafLocate->insertPairintoNode(key,data);
    
    checkUp(leafLocate);
}

template<typename Ktype, typename Vtype>
bool BPT<Ktype,Vtype>::deletePair(Ktype key,Vtype * data){
    BPTnode<Ktype,Vtype> * leafLocate = findLeaf(key, data);
    int changedindex = leafLocate->deletePairfromNode(key,data);
    maintainMinEntry(leafLocate);
    checkUp(leafLocate);
}

template<typename Ktype, typename Vtype>
void BPT<Ktype,Vtype>::maintainMinEntry(BPTnode<Ktype,Vtype> * nowNode){
    if(nowNode==NULL) return ;
    while(nowNode!=NULL){
        
        if(nowNode->isroot()){
            nowNode->updateMinKeyandEntry();
            nowNode->refreshMinDirty();
            break;
        }
        

        if(nowNode->isMinDirty()){
            BPTnode<Ktype,Vtype> * parentNode  = nowNode->getParentEntry();
            parentNode.deletePairfromNode(nowNode->getMinKey(),nowNode);
            nowNode->updateMinKeyandEntry();
            nowNode->refreshMinDirty();
            parentNode->insertPairintoNode(nowNode->getMinKey(),nowNode);
            nowNode = parentNode;
        }
        else{
            break;
        }
    }
    
}


template<typename Ktype, typename Vtype>
bool BPT<Ktype,Vtype>::checkUp(BPTnode<Ktype,Vtype> * nowNode){
    if(nowNode->isOverflow()){
        devideNode(nowNode);
    }
    else if(nowNode->isUnderflow){
        mergeNode(nowNode);
    }
}

template<typename Ktype, typename Vtype>
void BPT<Ktype,Vtype>::devideNode(BPTnode<Ktype,Vtype> * nowNode){
    
    while(nowNode->isOverflow()){
        if(nowNode->isroot()){
            BPTnode<Ktype,Vtype> * rootNode = new BPTnode<Ktype,Vtype>(order,order>>1,false,true);
            nowNode->setroot(false);

            BPTnode<Ktype,Vtype> * newNode = new BPTnode<Ktype,Vtype>(  order,order>>1,
                                                                        nowNode->isleaf(),nowNode->isroot(),
                                                                        nowNode,nowNode->getRightEntry(),rootNode);
            nowNode->setRightEntry(newNode);
            nowNode->devideRightIntoNode(newNode);

            rootNode->insertPairintoNode(nowNode->getMinKey(),nowNode);
            rootNode->insertPairintoNode(newNode->getMinKey(),newNode);

            root = rootNode;
            nowNode = rootNode;

        }
        else{
            
            BPTnode<Ktype,Vtype> * parentNode = nowNode->getParentEntry();

            BPTnode<Ktype,Vtype> * newNode = new BPTnode<Ktype,Vtype>(  order,order>>1,
                                                                        nowNode->isleaf(),nowNode->isroot(),
                                                                        nowNode,nowNode->getRightEntry(),parentNode);
            nowNode->setRightEntry(newNode);
            nowNode->devideRightIntoNode(newNode);

            parentNode->insertPairintoNode(newNode->getMinKey(),newNode);
            
            nowNode = parentNode;
        }
    }
}

template<typename Ktype, typename Vtype>
void BPT<Ktype,Vtype>::mergeNode(BPTnode<Ktype,Vtype> * nowNode){
    
}
