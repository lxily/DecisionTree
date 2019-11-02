#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

TreeNode* TreeGenerateWithoutPruning(vector<vector<string>>cars,vector<string>feature){
    /*û��ѵ���������������²����ܳ��ָ����*/
    if(cars.size()==0){
        return nullptr;
    }
    TreeNode *Node=new TreeNode();
    vector<pis> info=CheckInformationOfCars(cars);
    //ȫ������ͬһ���ǩ
    if(info.size()==1){
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
    //���򣬵�һ���ǳ��ִ������ı�ǩ
    sort(info.begin(),info.end());

    if(feature.size()==0||FeatureEqual(cars,feature)){
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
//    double EntD=CalculateEntD(cars);//��Ϣ��

    /**�������ţ���������id�Լ���Ӧ����Ϣ��**/
    pair<int,double> pid=ChooseTheMinEnt(cars,feature);
    int chooseFeature=pid.first;   //id
//    double MxEnt=EntD-pid.second;  //�����Ϣ����

    /**���ּ���D����ͬ���Ӽ�**/
    vector<vector<string>>subCars[MaxSubFeature];
    for(int i=0;i<(int)cars.size();i++){
        string subFeature=cars[i][chooseFeature];
        int subIdx=MapOfFeatureDetail[chooseFeature][subFeature];
        subCars[subIdx].push_back(cars[i]);
    }

    /**���õ�ǰ�������**/
    Node->feature=attribute[chooseFeature];

    /**ɾ����ѡ����**/
    vector<string>NextFeature=CancleNumber(feature,attribute[chooseFeature]);

    /**��ÿ���Ӽ�����һ���ӽڵ�**/
    for(int i=0;i<(int)features[chooseFeature].size();i++){//��������������
        if((int)subCars[i].size()==0){                     //DvΪ�գ�ֱ������ΪҶ���
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
        /*û��ѵ���������������²����ܳ��ָ����*/
        return nullptr;
    }
    TreeNode *Node=new TreeNode();
    vector<pis> info=CheckInformationOfCars(cars);
    if(info.size()==1){                 //ȫ������ͬһ���ǩ
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
    sort(info.begin(),info.end());      //���򣬵�һ���ǳ��ִ������ı�ǩ
    if(feature.size()==0||FeatureEqual(cars,feature)){
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
//    double EntD=CalculateEntD(cars);//��Ϣ��

    /**�������ţ���������id�Լ���Ӧ����Ϣ��**/
    pair<int,double> pid=ChooseTheMinEnt(cars,feature);
    int chooseFeature=pid.first;   //id
//    double MxEnt=EntD-pid.second;  //��Ϣ��

    /**���õ�ǰ�������**/
    Node->feature=attribute[chooseFeature];

    /**���ּ���D����ͬ���Ӽ�**/
    vector<vector<string>>subCars[MaxSubFeature];
    for(int i=0;i<(int)cars.size();i++){
        string subFeature=cars[i][chooseFeature];
        int subIdx=MapOfFeatureDetail[chooseFeature][subFeature];
        subCars[subIdx].push_back(cars[i]);
    }

    //�����֦����ý����Ҫ�����óɵı�ǩ
    string Lable=info[0].second;

    //ֱ�����óɽ�����֤������Ϣ
    info=CheckInformationOfCars(limit);
    sort(info.begin(),info.end());

    //����ǰ��֤����ȷ����Ŀ
    int CorrectBefore=info[0].first;
    int CorrectAfter=0;
    //ͳ�ƻ��ֺ���֤����ȷ����Ŀ
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

    //������ֺ�û�и��ţ���֦
    if(CorrectBefore>=CorrectAfter){
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }

    /**ɾ����ѡ����**/
    vector<string>NextFeature=CancleNumber(feature,attribute[chooseFeature]);

    /**��ÿ���Ӽ�����һ���ӽڵ�**/
    for(int i=0;i<(int)features[chooseFeature].size();i++){//��������������
        if(subCars[i].size()==0){                     //DvΪ�գ�ֱ������ΪҶ���
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
        /*û��ѵ���������������²����ܳ��ָ����*/
        return nullptr;
    }
    TreeNode *Node=new TreeNode();
    vector<pis> info=CheckInformationOfCars(cars);
    if(info.size()==1){                 //ȫ������ͬһ���ǩ
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
    sort(info.begin(),info.end());      //���򣬵�һ���ǳ��ִ������ı�ǩ
    if(feature.size()==0||FeatureEqual(cars,feature)){
        Node->isLeaf=true;
        Node->label=info[0].second;
        return Node;
    }
//    double EntD=CalculateEntD(cars);//��Ϣ��

    /**�������ţ���������id�Լ���Ӧ����Ϣ��**/
    pair<int,double> pid=ChooseTheMinEnt(cars,feature);
    int chooseFeature=pid.first;     //id
//    double MxEnt=EntD-pid.second;  //�����Ϣ����

    /**���ּ���D����ͬ���Ӽ�**/
    vector<vector<string>>subCars[MaxSubFeature];
    for(int i=0;i<(int)cars.size();i++){
        string subFeature=cars[i][chooseFeature];
        int subIdx=MapOfFeatureDetail[chooseFeature][subFeature];
        subCars[subIdx].push_back(cars[i]);
    }

    //���ֺ���֤������ͬ���Ӽ�
    vector<vector<string>>subLimit[MaxSubFeature];
    for(int i=0;i<(int)limit.size();i++){
        string subFeature=limit[i][chooseFeature];
        int subIdx=MapOfFeatureDetail[chooseFeature][subFeature];
        subLimit[subIdx].push_back(limit[i]);
    }

    /**���õ�ǰ�������**/
    Node->feature=attribute[chooseFeature];

    /**ɾ����ѡ����**/
    vector<string>NextFeature=CancleNumber(feature,attribute[chooseFeature]);

    /**��ÿ���Ӽ�����һ���ӽڵ�**/
    for(int i=0;i<(int)features[chooseFeature].size();i++){//��������������
        if(subCars[i].size()==0){                     //DvΪ�գ�ֱ������ΪҶ���
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

    //�Ѿ���Ҷ��㣬����Ҫ��֦
    if(Node->isLeaf)return Node;

    //�����֦����ý����Ҫ�����óɵı�ǩ
    string Lable=info[0].second;

    //ֱ�����óɽ�����֤������Ϣ
    info=CheckInformationOfCars(limit);
    sort(info.begin(),info.end());

    //����ǰ��֤����ȷ����Ŀ
    int CorrectBefore=info[0].first;
    //���ֺ���֤����ȷ����Ŀ
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

    //�������ǰ���ţ���֦
    if(CorrectBefore>CorrectAfter){
        Node->isLeaf=true;
        Node->label=Lable;
    }

    return Node;
}
int TreeCaculateNodeCnt(TreeNode *Model){
    //Ҷ�ӽڵ�
    if(Model==nullptr){
        /*��������²����ܳ��ָ����*/
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

/*��ѯ
1����ǰ��Ҷ�ӽ�㣬�ж��Ƿ���ȷ->���ؽ��
2���жϵ�ǰ����Ӧ��������(���Ǿ��������)
    ���磺��ǰ��������Ϊɫ�󣬵�����������ֵ(0->"����",1->"�ں�",2->"ǳ��")[��Ӧ�����ӽڵ�]
    �жϵ�ǰ��ѯ�ĵ�ǰ���Ծ���Ϊ�Ǹ�
    ���磺��ǰ��ѯ���ɫ������ֵΪ���̣���ݹ��ѯ��ǰ���ĵ�1��(0��)�ӽڵ�
    Ҳ��ÿ�����ֻ��Ҫ������Ӧ�����ԣ������¶�Ӧ��ֵ���Զ�Ӧ�������ı��
    ��Ȼ��Ҷ�ӽڵ���������ν
*/
bool TreeQuery(TreeNode *Model,vector<string>car){
    //Ҷ�ӽڵ�
    if(Model==nullptr){
        /*��������²����ܳ��ָ����*/
        return false;
    }
    if(Model->isLeaf){
        return Model->label==car[car.size()-1];
    }

    //�������idx
    int idx=MapOfFeature[Model->feature];
    //��ѯ������ֵ
    string subFeature=car[idx];
    //��ѯ������ֵ��Ӧid(��ѯ�ڼ�������)
    int son=MapOfFeatureDetail[idx][subFeature];

    return TreeQuery(Model->child[son],car);
}

#endif // MODEL_H_INCLUDED
