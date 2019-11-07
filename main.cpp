#include "init.h"
#include "node.h"
#include "func.h"
#include "model.h"

void ReadData(const char *fileName){

    freopen(fileName,"r",stdin);
    string carInfomation;
    while(cin>>carInfomation){
        string tmp="";
        vector<string>vcar;
        carInfomation+=',';
        int len=carInfomation.length();
        for(int i=0;i<len;i++){
            if(carInfomation[i]==','){
                vcar.push_back(tmp);tmp="";
            }
            else tmp+=carInfomation[i];
        }
        allCar.push_back(vcar);
    }
}

void SplitData(vector<vector<string>>allData,
               vector<vector<string>>&TrainData,
               vector<vector<string>>&testData,
               double TrainRate,bool ShuffData){

    //随机打乱数据顺序
    if(ShuffData)
        random_shuffle(allData.begin(),allData.end());

    int TrainCnt=ceil(TrainRate*allData.size());

    for(int i=0;i<TrainCnt;i++)
        TrainData.push_back(allData[i]);

    for(int i=TrainCnt;i<(int)allData.size();i++)
        testData.push_back(allData[i]);

}

pii TestModel(TreeNode *Model,vector<vector<string>>TrainData,
                              vector<vector<string>>TestData){

    int CorrectOfTrain=0,CorrectOfTest=0;
    for(int i=0;i<(int)TrainData.size();i++)
        CorrectOfTrain+=TreeQuery(Model,TrainData[i]);

    for(int i=0;i<(int)TestData.size();i++)
        CorrectOfTest+=TreeQuery(Model,TestData[i]);

    return mkp(CorrectOfTrain,CorrectOfTest);
}

void PrintResult(pii Result,pii AllCnt,int NodeCnt,char *s){
    int c1=Result.first,c2=Result.second;
    int a1=AllCnt.first,a2=AllCnt.second;
    printf("|*************%s*************|\n\n",s);
    printf("    The number of nodes:\t %d\n",NodeCnt);
    printf("    Accuracy of training data:\t %.5f\n",1.0*c1/a1);
    printf("    Accuracy of test data:\t %.5f\n",1.0*c2/a2);
    printf("    Accuracy of all data:\t %.5f\n\n\n",1.0*(c1+c2)/(a1+a2));
}


template<class Ty1,class Ty2>
inline const pair<Ty1,Ty2>operator+(const pair<Ty1,Ty2>&p1,const pair<Ty1,Ty2>&p2)
{
    return make_pair(p1.first+p2.first,p1.second+p2.second);
}

int main(){

    srand(time(0));

    //读入文件所在位置
    const char *fileName="car.txt";

    Init();
    ReadData(fileName);

    //设置训练集比例
    double TrainRate=0.95;
    double LimitRate=0.82;
    //设置是否打乱数据顺序
    bool ShuffData=true;
    //求值的平均次数
    int TestTimes=20;

    pii AllCnt=mkp(0,0);
    pii Result1=mkp(0,0);
    pii Result2=mkp(0,0);
    pii Result3=mkp(0,0);
    int NodeCNt1=0,NodeCNt2=0,NodeCNt3=0;
    for(int i=0;i<TestTimes;i++){
        vector<vector<string>>TrainData;
        vector<vector<string>>TestData;
        vector<vector<string>>SubTrainData;
        vector<vector<string>>LimitData;

//        分割数据集
        SplitData(allCar,TrainData,TestData,TrainRate,ShuffData);
//        分割训练集,作为剪枝的判断条件
        SplitData(TrainData,SubTrainData,LimitData,LimitRate,ShuffData);
//        生成决策树
        TreeNode *ModelWithoutPruning=TreeGenerateWithoutPruning(TrainData,attribute);
        TreeNode *ModelWithPrepruning=TreeGenerateWithPrepruning(SubTrainData,LimitData,attribute);
        TreeNode *ModelWithPostpruning=TreeGenerateWithPostpruning(SubTrainData,LimitData,attribute);
//        得到测试信息:pair<训练集正确数目，测试集正确数目>
        pii ResultOfModel1=TestModel(ModelWithoutPruning,TrainData,TestData);
        pii ResultOfModel2=TestModel(ModelWithPrepruning,TrainData,TestData);
        pii ResultOfModel3=TestModel(ModelWithPostpruning,TrainData,TestData);

//        累加
        Result1=Result1+ResultOfModel1;
        Result2=Result2+ResultOfModel2;
        Result3=Result3+ResultOfModel3;
        AllCnt.first+=TrainData.size();
        AllCnt.second+=TestData.size();

        NodeCNt1+=TreeCaculateNodeCnt(ModelWithoutPruning);
        NodeCNt2+=TreeCaculateNodeCnt(ModelWithPrepruning);
        NodeCNt3+=TreeCaculateNodeCnt(ModelWithPostpruning);
    }
//    输出
    PrintResult(Result1,AllCnt,NodeCNt1/TestTimes,"ModelWithoutPruning");
    PrintResult(Result2,AllCnt,NodeCNt2/TestTimes,"ModelWithPrepruning");
    PrintResult(Result3,AllCnt,NodeCNt3/TestTimes,"ModelWithPostpruning");
    return 0;
}
