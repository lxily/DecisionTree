#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

TreeNode* TreeGenerateWithoutPruning(vector<vector<string>>cars,vector<string>feature){
    /*没有训练集：正常输入下不可能出现该情况*/
    if(cars.size()==0){
        return nullptr;
    }
    TreeNode *Node=new TreeNode();
    vector<pis> info=CheckInformationOfCars(cars);
    //全部属于同一类标签
    if(info.size()==1){
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
    //排序，第一个是出现次数最多的标签
    sort(info.begin(),info.end());

    if(feature.size()==0||FeatureEqual(cars,feature)){
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
//    double EntD=CalculateEntD(cars);//信息熵

    /**划分最优：返回属性id以及对应的信息熵**/
    pair<int,double> pid=ChooseTheMinEnt(cars,feature);
    int chooseFeature=pid.first;   //id
//    double MxEnt=EntD-pid.second;  //最大信息增益

    /**划分集合D到不同的子集**/
    vector<vector<string>>subCars[MaxSubFeature];
    for(int i=0;i<(int)cars.size();i++){
        string subFeature=cars[i][chooseFeature];
        int subIdx=MapOfFeatureDetail[chooseFeature][subFeature];
        subCars[subIdx].push_back(cars[i]);
    }

    /**设置当前结点属性**/
    Node->feature=attribute[chooseFeature];

    /**删除所选属性**/
    vector<string>NextFeature=CancleNumber(feature,attribute[chooseFeature]);

    /**给每个子集划分一个子节点**/
    for(int i=0;i<(int)features[chooseFeature].size();i++){//遍历所有子特征
        if((int)subCars[i].size()==0){                     //Dv为空，直接设置为叶结点
            TreeNode *newChild=new TreeNode();
            newChild->isLeaf=true;
            newChild->label=info[0].second;
            Node->child.push_back(newChild);
        }
        else{
            TreeNode *newChild=TreeGenerateWithoutPruning(subCars[i],NextFeature);
            Node->child.push_back(newChild);
        }
    }
    return Node;
}

TreeNode* TreeGenerateWithPrepruning(vector<vector<string>>cars,vector<vector<string>>limit,vector<string>feature){
    if(cars.size()==0){
        /*没有训练集：正常输入下不可能出现该情况*/
        return nullptr;
    }
    TreeNode *Node=new TreeNode();
    vector<pis> info=CheckInformationOfCars(cars);
    if(info.size()==1){                 //全部属于同一类标签
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
    sort(info.begin(),info.end());      //排序，第一个是出现次数最多的标签
    if(feature.size()==0||FeatureEqual(cars,feature)){
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
//    double EntD=CalculateEntD(cars);//信息熵

    /**划分最优：返回属性id以及对应的信息熵**/
    pair<int,double> pid=ChooseTheMinEnt(cars,feature);
    int chooseFeature=pid.first;   //id
//    double MxEnt=EntD-pid.second;  //信息熵

    /**设置当前结点属性**/
    Node->feature=attribute[chooseFeature];

    /**划分集合D到不同的子集**/
    vector<vector<string>>subCars[MaxSubFeature];
    for(int i=0;i<(int)cars.size();i++){
        string subFeature=cars[i][chooseFeature];
        int subIdx=MapOfFeatureDetail[chooseFeature][subFeature];
        subCars[subIdx].push_back(cars[i]);
    }

    //如果剪枝，则该结点需要被设置成的标签
    string Lable=info[0].second;

    //直接设置成结点后验证集的信息
    info=CheckInformationOfCars(limit);
    sort(info.begin(),info.end());

    //划分前验证集正确的数目
    int CorrectBefore=info[0].first;
    int CorrectAfter=0;
    //统计划分后验证集正确的数目
    vector<vector<string>>subLimit[MaxSubFeature];
    for(int i=0;i<(int)limit.size();i++){
        string subFeature=limit[i][chooseFeature];
        int subIdx=MapOfFeatureDetail[chooseFeature][subFeature];
        subLimit[subIdx].push_back(limit[i]);
    }
    for(int i=0;i<(int)features[chooseFeature].size();i++){
        info=CheckInformationOfCars(subLimit[i]);
        sort(info.begin(),info.end());
        CorrectAfter+=info[0].first;
    }

    //如果划分后没有更优，剪枝
    if(CorrectBefore>=CorrectAfter){
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }

    /**删除所选属性**/
    vector<string>NextFeature=CancleNumber(feature,attribute[chooseFeature]);

    /**给每个子集划分一个子节点**/
    for(int i=0;i<(int)features[chooseFeature].size();i++){//遍历所有子特征
        if(subCars[i].size()==0){                     //Dv为空，直接设置为叶结点
            TreeNode *newChild=new TreeNode();
            newChild->isLeaf=true;
            newChild->label=info[0].second;
            Node->child.push_back(newChild);
        }
        else{
            TreeNode *newChild=TreeGenerateWithPrepruning(subCars[i],subLimit[i],NextFeature);
            Node->child.push_back(newChild);
        }
    }
    return Node;
}

TreeNode* TreeGenerateWithPostpruning(vector<vector<string>>cars,vector<vector<string>>limit,vector<string>feature){
    if(cars.size()==0){
        /*没有训练集：正常输入下不可能出现该情况*/
        return nullptr;
    }
    TreeNode *Node=new TreeNode();
    vector<pis> info=CheckInformationOfCars(cars);
    if(info.size()==1){                 //全部属于同一类标签
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
    sort(info.begin(),info.end());      //排序，第一个是出现次数最多的标签
    if(feature.size()==0||FeatureEqual(cars,feature)){
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
//    double EntD=CalculateEntD(cars);//信息熵

    /**划分最优：返回属性id以及对应的信息熵**/
    pair<int,double> pid=ChooseTheMinEnt(cars,feature);
    int chooseFeature=pid.first;     //id
//    double MxEnt=EntD-pid.second;  //最大信息增益

    /**划分集合D到不同的子集**/
    vector<vector<string>>subCars[MaxSubFeature];
    for(int i=0;i<(int)cars.size();i++){
        string subFeature=cars[i][chooseFeature];
        int subIdx=MapOfFeatureDetail[chooseFeature][subFeature];
        subCars[subIdx].push_back(cars[i]);
    }

    //划分后验证集到不同的子集
    vector<vector<string>>subLimit[MaxSubFeature];
    for(int i=0;i<(int)limit.size();i++){
        string subFeature=limit[i][chooseFeature];
        int subIdx=MapOfFeatureDetail[chooseFeature][subFeature];
        subLimit[subIdx].push_back(limit[i]);
    }

    /**设置当前结点属性**/
    Node->feature=attribute[chooseFeature];

    /**删除所选属性**/
    vector<string>NextFeature=CancleNumber(feature,attribute[chooseFeature]);

    /**给每个子集划分一个子节点**/
    for(int i=0;i<(int)features[chooseFeature].size();i++){//遍历所有子特征
        if(subCars[i].size()==0){                     //Dv为空，直接设置为叶结点
            TreeNode *newChild=new TreeNode();
            newChild->isLeaf=true;
            newChild->label=info[0].second;
            Node->child.push_back(newChild);
        }
        else{
            TreeNode *newChild=TreeGenerateWithPostpruning(subCars[i],subLimit[i],NextFeature);
            Node->child.push_back(newChild);
        }
    }

    //已经是叶结点，不需要剪枝
    if(Node->isLeaf)return Node;

    //如果剪枝，则该结点需要被设置成的标签
    string Lable=info[0].second;

    //直接设置成结点后验证集的信息
    info=CheckInformationOfCars(limit);
    sort(info.begin(),info.end());

    //划分前验证集正确的数目
    int CorrectBefore=info[0].first;
    //划分后验证集正确的数目
    int CorrectAfter=0;
    for(int i=0;i<(int)Node->child.size();i++){
        if(Node->child[i]->isLeaf){
            string childLabel=Node->child[i]->label;
            for(int j=0;j<(int)subLimit[i].size();j++){
                if(!subLimit[i][j].size())continue;
                int idx=subLimit[i][j].size()-1;
                string Clabel=subLimit[i][j][idx];
                if(childLabel==Clabel){
                    CorrectAfter++;
                }
            }
        }
        else{
            info=CheckInformationOfCars(subLimit[i]);
            sort(info.begin(),info.end());
            CorrectAfter+=info[0].first;
        }
    }

    //如果划分前更优，剪枝
    if(CorrectBefore>CorrectAfter){
        Node->isLeaf=true;
        Node->label=Lable;
    }

    return Node;
}
int TreeCaculateNodeCnt(TreeNode *Model){
    //叶子节点
    if(Model==nullptr){
        /*正常情况下不可能出现该情况*/
        return 0;
    }
    if(Model->isLeaf){
        return 1;
    }

    int NodeCnt=0;
    for(int i=0;i<(int)Model->child.size();i++){
        NodeCnt+=TreeCaculateNodeCnt(Model->child[i]);
    }
    return NodeCnt;
}

/*查询
1、当前是叶子结点，判断是否正确->返回结果
2、判断当前结点对应哪种属性(不是具体的属性)
    比如：当前结点的属性为色泽，但是其有三个值(0->"青绿",1->"乌黑",2->"浅白")[对应三个子节点]
    判断当前查询的当前属性具体为那个
    比如：当前查询项的色泽属性值为青绿，则递归查询当前结点的第1个(0号)子节点
    也即每个结点只需要保留对应的属性，属性下对应的值可以对应到子树的编号
    当然，叶子节点属性无所谓
*/
bool TreeQuery(TreeNode *Model,vector<string>car){
    //叶子节点
    if(Model==nullptr){
        /*正常情况下不可能出现该情况*/
        return false;
    }
    if(Model->isLeaf){
        return Model->label==car[car.size()-1];
    }

    //结点属性idx
    int idx=MapOfFeature[Model->feature];
    //查询的属性值
    string subFeature=car[idx];
    //查询的属性值对应id(查询第几颗子树)
    int son=MapOfFeatureDetail[idx][subFeature];

    return TreeQuery(Model->child[son],car);
}

#endif // MODEL_H_INCLUDED
