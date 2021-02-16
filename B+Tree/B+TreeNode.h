#include<iostream>
#include<string>
#include<map>
#include<vector>

template<typename Nodetype, typename Datatype>
union entry{
    Nodetype * BPTnodeEntry;
    Datatype * BPTdataEntry;
};

template<typename Ktype, typename Vtype>
class BPTnode{

    public:
        BPTnode();
        BPTnode(int msize,int thres);
        BPTnode(int msize,int thres,bool leaf,bool root);
        BPTnode(int msize,int thres,bool leaf,bool root,BPTnode<Ktype,Vtype> * lef,BPTnode<Ktype,Vtype> * rig,BPTnode<Ktype,Vtype> * par);
        ~BPTnode();
        bool isFull();//判断是否已满，能否再插入节点
        bool isOverflow();//判断当前节点是否溢出
        bool isLeast();//判断当前节点是否低于最少节点
        bool isUnderflow();//判断当前节点是否下溢出
 
        bool isroot();//判断当前节点是否是根节点
        bool isleaf();//判断当前节点是否是叶子节点

        void setroot(bool isroot);//设置当前节点的root属性
        void setleaf(bool isleaf);//设置当前节点的isleaf属性

        Ktype getFirstKey();
        entry<BPTnode<Ktype,Vtype>,Vtype> getFirstEntry();

        Ktype getMinKey();
        entry<BPTnode<Ktype,Vtype>,Vtype> getMinEntry();
        bool isMinDirty();
        void setMinDirty();
        bool isInBPT();
        void setInBPT(bool inbpt);    
        void refreshMinDirty();
        void updateMinKeyandEntry();
        

        BPTnode<Ktype,Vtype> * getParentEntry();//获得当前节点的父节点指针
        BPTnode<Ktype,Vtype> * getLeftEntry();//获得当前节点的左兄弟指针
        BPTnode<Ktype,Vtype> * getRightEntry();//获得当前节点的右兄弟指针
        void setParentEntry(BPTnode<Ktype,Vtype> * parentEntry);//设置当前节点的父节点
        void setLeftEntry(BPTnode<Ktype,Vtype> * leftEntry);//设置当前节点的左兄弟
        void setRightEntry(BPTnode<Ktype,Vtype> * rightEntry);//设置当前节点的右兄弟

        int insertPairintoNode(Ktype nodeKey,BPTnode<Ktype,Vtype> * nodeEntry);//在当前节点插入键值对,返回插入的位置
        int insertPairintoNode(Ktype dataKey,Vtype * dataEntry);//在当前节点插入键值对（重载）,返回插入的位置
        int insertPairintoNode(Ktype dataKey,entry<BPTnode<Ktype,Vtype>,Vtype>  Entry);//在当前节点插入键值对（重载）,返回插入的位置
        
        int deletePairfromNode(Ktype nodeKey,BPTnode<Ktype,Vtype> * nodeEntry);//在当前节点删除键值对,返回删除的位置
        int deletePairfromNode(Ktype dataKey,Vtype * dataEntry);//在当前节点删除键值对,返回删除的位置

        bool mergeIntoNode(BPTnode<Ktype,Vtype> * desNode);
        bool devideRightIntoNode(BPTnode<Ktype,Vtype> * desNode);

        
        void printNode();
    private:

        int maxsize;//最多存储多少个关键字
        int threshold;//最少需要存储多少个关键字的门槛值
        int keynum;//当前节点存储有多少个关键字
        bool isLeaf;//当前节点是否是叶子节点
        bool isRoot;//当前节点是否是根节点
        bool inBPT;
        BPTnode<Ktype,Vtype> * left;//当前节点的左兄弟
        BPTnode<Ktype,Vtype> * right;//当前节点的右兄弟
        BPTnode<Ktype,Vtype> * parent;//当前节点的父亲兄弟
        std::vector<Ktype> * keys;//存储关键字(在创建时根据关键字类型创建)
        std::vector<entry<BPTnode<Ktype,Vtype>,Vtype>> * entries;//存储关键字对应的数据入口(在创建时根据当前节点是否是叶子节点创建)
        entry<BPTnode<Ktype,Vtype>,Vtype> minEntry;
        Ktype minKey;
        bool minDirty;
};

template<typename Ktype, typename Vtype>
BPTnode<Ktype,Vtype>::BPTnode()
    :maxsize(0),threshold(0),keynum(0),
    isLeaf(false),isRoot(false),
    left(NULL),right(NULL),parent(NULL),
    inBPT(false),minDirty(false)
{
    keys = new std::vector<Ktype>();
    entries = new std::vector<entry<BPTnode<Ktype,Vtype>,Vtype>>();
}


template<typename Ktype, typename Vtype>
BPTnode<Ktype,Vtype>::BPTnode(int msize,int thres)
    :maxsize(msize),threshold(thres),keynum(0),
    isLeaf(false),isRoot(false),
    left(NULL),right(NULL),parent(NULL),
    inBPT(false),minDirty(false)
{
    keys = new std::vector<Ktype>();
    entries = new std::vector<entry<BPTnode<Ktype,Vtype>,Vtype>>();
}

template<typename Ktype, typename Vtype>
BPTnode<Ktype,Vtype>::BPTnode(int msize,int thres,bool leaf,bool root)
    :maxsize(msize),threshold(thres),keynum(0),
    isLeaf(leaf),isRoot(root),
    left(NULL),right(NULL),parent(NULL),
    inBPT(false),minDirty(false)
{
    keys = new std::vector<Ktype>();
    entries = new std::vector<entry<Ktype,Vtype>,Vtype>();
}

template<typename Ktype, typename Vtype>
BPTnode<Ktype,Vtype>::BPTnode(int msize,int thres,bool leaf,bool root, BPTnode<Ktype,Vtype> * lef, BPTnode<Ktype,Vtype> * rig, BPTnode<Ktype,Vtype> * par)
    :maxsize(msize),threshold(thres),keynum(0),
    isLeaf(leaf),isRoot(root),
    left(lef),right(rig),parent(par),
    inBPT(false),minDirty(false)
{
    keys = new std::vector<Ktype>();
    entries = new std::vector<entry<Ktype,Vtype>,Vtype>();
}

template<typename Ktype, typename Vtype>
bool BPTnode<Ktype,Vtype>::isFull(){
    if (keynum==maxsize) return true;
    return false;
}

template<typename Ktype, typename Vtype>
bool BPTnode<Ktype,Vtype>::isOverflow(){
    if (keynum>maxsize) return true;
    return false;
}

template<typename Ktype, typename Vtype>
bool BPTnode<Ktype,Vtype>::isLeast(){
     if (keynum==threshold) return true;
    return false;
}

template<typename Ktype, typename Vtype>
bool BPTnode<Ktype,Vtype>::isUnderflow(){
    if (keynum<threshold) return true;
    return false;
}

template<typename Ktype, typename Vtype>
bool BPTnode<Ktype,Vtype>::isroot(){
     return isRoot;
}

template<typename Ktype, typename Vtype>
bool BPTnode<Ktype,Vtype>::isleaf(){
     return isLeaf;
}

template<typename Ktype, typename Vtype>
void BPTnode<Ktype,Vtype>::setroot(bool root){
    isRoot = root;
    return ;
}

template<typename Ktype, typename Vtype>
void BPTnode<Ktype,Vtype>::setleaf(bool leaf){
    isLeaf = leaf;
    return ;
}


template<typename Ktype, typename Vtype>
bool BPTnode<Ktype,Vtype>::isMinDirty(){
    return minDirty;
}

template<typename Ktype, typename Vtype>
void BPTnode<Ktype,Vtype>::setMinDirty(){
    minDirty = true;
    return ;
}

template<typename Ktype, typename Vtype>
bool BPTnode<Ktype,Vtype>::isInBPT(){
    return inBPT;
}

template<typename Ktype, typename Vtype>
void BPTnode<Ktype,Vtype>::setInBPT(bool inbpt){
    inBPT = inbpt;
    return ;
}


template<typename Ktype, typename Vtype>
void BPTnode<Ktype,Vtype>::refreshMinDirty(){
    minDirty = false;
    return ;
}

template<typename Ktype, typename Vtype>
Ktype BPTnode<Ktype,Vtype>::getFirstKey(){
    return keys->front();
}
        
template<typename Ktype, typename Vtype>
entry<BPTnode<Ktype,Vtype>,Vtype> BPTnode<Ktype,Vtype>::getFirstEntry(){
    return entries->front();
}

template<typename Ktype, typename Vtype>
void BPTnode<Ktype,Vtype>::updateMinKeyandEntry(){
    minKey = keys->front();
    minEntry = entries->front();
    return ;
}



template<typename Ktype, typename Vtype>
BPTnode<Ktype,Vtype> * BPTnode<Ktype,Vtype>::getParentEntry(){
    return parent;
}

template<typename Ktype, typename Vtype>
BPTnode<Ktype,Vtype> * BPTnode<Ktype,Vtype>::getLeftEntry(){
    return left;
}

template<typename Ktype, typename Vtype>
BPTnode<Ktype,Vtype> * BPTnode<Ktype,Vtype>::getRightEntry(){
    return right;
}

template<typename Ktype, typename Vtype>
void BPTnode<Ktype,Vtype>::setParentEntry(BPTnode<Ktype,Vtype> * parententry){
    parent = parententry;
    return ;
}

template<typename Ktype, typename Vtype>
void BPTnode<Ktype,Vtype>::setLeftEntry(BPTnode<Ktype,Vtype> * leftentry){
    left = leftentry;
    return ;
}

template<typename Ktype, typename Vtype>
void BPTnode<Ktype,Vtype>::setRightEntry(BPTnode<Ktype,Vtype> * rightentry){
    right = rightentry;
    return ;
}

//inseret 不会造成最小值改变,因为每次插入时保证最小值会小于等于当前值
template<typename Ktype, typename Vtype>
int BPTnode<Ktype,Vtype>::insertPairintoNode(Ktype nodekey,BPTnode<Ktype,Vtype> * nodeEntry){
    typename std::vector<Ktype>::iterator itkeys;
    typename std::vector<entry<BPTnode<Ktype,Vtype>,Vtype>>::iterator itentry;
    int i = 0;
    for(itkeys = keys->begin(),itentry = entries->begin(),i=0;
        itkeys!=keys->end()&&itentry!=entries->end();
        itkeys++,itentry++,i++)
    {
        if(*itkeys>=nodekey) 
            break;
    }
    keys->insert(itkeys,nodekey);
    entry<BPTnode<Ktype,Vtype>,Vtype> e;
    e.BPTnodeEntry = nodeEntry;
    entries->insert(itentry,e);
    keynum++;
    
    return i;
}

template<typename Ktype, typename Vtype>
int BPTnode<Ktype,Vtype>::insertPairintoNode(Ktype dataKey,Vtype * dataEntry){
    typename std::vector<Ktype>::iterator itkeys;
    typename std::vector<entry<BPTnode<Ktype,Vtype>,Vtype>>::iterator itentry;
    int i = 0;
    for(itkeys = keys->begin(),itentry = entries->begin(),i = 0;
        itkeys!=keys->end()&&itentry!=entries->end();
        itkeys++,itentry++,i++)
    {
        if(*itkeys>=dataKey) 
            break;
    }
    keys->insert(itkeys,dataKey);
    entry<BPTnode<Ktype,Vtype>,Vtype> e;
    e.BPTdataEntry = dataEntry;
    entries->insert(itentry,e);
    keynum++;
    return i;
}

template<typename Ktype, typename Vtype>
int BPTnode<Ktype,Vtype>::insertPairintoNode(Ktype dataKey,entry<BPTnode<Ktype,Vtype>,Vtype> Entry){
    typename std::vector<Ktype>::iterator itkeys;
    typename std::vector<entry<BPTnode<Ktype,Vtype>,Vtype>>::iterator itentry;
    int i = 0;
    for(itkeys = keys->begin(),itentry = entries->begin(),i = 0;
        itkeys!=keys->end()&&itentry!=entries->end();
        itkeys++,itentry++,i++)
    {
        if(*itkeys>=dataKey) 
            break;
    }
    keys->insert(itkeys,dataKey); 
    entries->insert(itentry,Entry);
    keynum++;
    
    return i;
}

template<typename Ktype, typename Vtype>
int BPTnode<Ktype,Vtype>::deletePairfromNode(Ktype nodeKey,BPTnode<Ktype,Vtype> * nodeEntry){
    typename std::vector<Ktype>::iterator itkeys;
    typename std::vector<entry<BPTnode<Ktype,Vtype>,Vtype>>::iterator itentry;
    int i = 0;
    for(itkeys = keys->begin(),itentry = entries->begin(),i=0;
        itkeys!=keys->end()&&itentry!=entries->end();
        itkeys++,itentry++,i++)
    {
        if(*itkeys==nodeKey /*&& nodeEntry==*itentry*/){
            break;
        }
            
        if(*itkeys>nodeKey){
            return -1;
        }
    }
    if(itkeys==keys->end()){
        return -1;
    }
    keys->erase(itkeys);
    entries->erase(itentry);
    keynum--;
    if(i==0){
        setMinDirty();
    }
    return i;
}

template<typename Ktype, typename Vtype>        
int BPTnode<Ktype,Vtype>::deletePairfromNode(Ktype dataKey,Vtype * dataEntry){
    typename std::vector<Ktype>::iterator itkeys;
    typename std::vector<entry<BPTnode<Ktype,Vtype>,Vtype>>::iterator itentry;
    int i = 0;
    for(itkeys = keys->begin(),itentry = entries->begin(),i = 0;
        itkeys!=keys->end()&&itentry!=entries->end();
        itkeys++,itentry++,i++)
    {
        if(*itkeys==dataKey /*&& dataEntry==*itentry*/){
            break;
        }
            
        if(*itkeys>dataKey){
            return -1;
        }
    }
    if(itkeys==keys->end()){
        return -1;
    }
    keys->erase(itkeys);
    entries->erase(itentry);
    keynum--;
    if(i==0){
        setMinDirty();
    }
    return i;
}

template<typename Ktype, typename Vtype>  
bool BPTnode<Ktype,Vtype>::mergeIntoNode(BPTnode<Ktype,Vtype> * desNode){

}

template<typename Ktype, typename Vtype>  
bool BPTnode<Ktype,Vtype>::devideRightIntoNode(BPTnode<Ktype,Vtype> * desNode){
    typename std::vector<Ktype>::iterator itkeys;
    typename std::vector<entry<BPTnode<Ktype,Vtype>,Vtype>>::iterator itentry;
    for(itkeys = keys->begin()+(keynum>>1),itentry = entries->begin()+(keynum>>1);
        itkeys!=keys->end()&&itentry!=entries->end();
        itkeys++,itentry++)
    {
        desNode->insertPairintoNode(*itkeys,(*itentry));
    }

    itkeys = keys->begin() + (keynum>>1);
    itentry = entries->begin()+(keynum>>1);
    while (itkeys!=keys.end() && itentry!=entries.end()){
        itkeys=keys.erase(itkeys);
        itentry = entries.erase(itentry);
   }
}

template<typename Ktype, typename Vtype>        
void BPTnode<Ktype,Vtype>::printNode(){
    typename std::vector<Ktype>::iterator itkeys;
    typename std::vector<entry<BPTnode<Ktype,Vtype>,Vtype>>::iterator itentry;
    for(itkeys = keys->begin(),itentry = entries->begin();
        itkeys!=keys->end()&&itentry!=entries->end();
        itkeys++,itentry++)
    {
        std::cout<<*itkeys<<" ";
    }
    std::cout<<std::endl;
}