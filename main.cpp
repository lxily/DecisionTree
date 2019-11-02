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

void SplitData(vector<vector<string>>allData,vector<vector<string>>&TrainData,vector<vector<string>>&testData,double TrainRate,bool ShuffData){

    //�����������˳��
    if(ShuffData)
        random_shuffle(allData.begin(),allData.end());

    int TrainCnt=ceil(TrainRate*allData.size());

    for(int i=0;i<TrainCnt;i++)
        TrainData.push_back(allData[i]);

    for(int i=TrainCnt;i<(int)allData.size();i++)
        testData.push_back(allData[i]);

}

void TestModel(TreeNode *Model,vector<vector<string>>TrainData,vector<vector<string>>TestData,char *s){

    printf("|*************%s*************|\n\n",s);

    printf("    The number of nodes:\t %d\n",TreeCaculateNodeCnt(Model));

    int CorrectOfTrain=0,CorrectOfTest=0;
    int DataCnt=TrainData.size()+TestData.size();

    for(int i=0;i<(int)TrainData.size();i++)
        CorrectOfTrain+=TreeQuery(Model,TrainData[i]);

    for(int i=0;i<(int)TestData.size();i++)
        CorrectOfTest+=TreeQuery(Model,TestData[i]);

    printf("    Accuracy of training data:\t %.5f\n",1.0*CorrectOfTrain/TrainData.size());
    printf("    Accuracy of test data:\t %.5f\n",1.0*CorrectOfTest/TestData.size());
    printf("    Accuracy of all data:\t %.5f\n\n\n",1.0*(CorrectOfTest+CorrectOfTrain)/DataCnt);

}
int main(){
    clock_t BegTime;
    clock_t EndTime;
    double CostTime;
    srand(time(0));

    //�����ļ�����λ��
    const char *fileName="car.txt";

    Init();

    ReadData(fileName);

    //����ѵ��������
    double TrainRate=0.75;
    //�����Ƿ��������˳��
    bool ShuffData=true;

    vector<vector<string>>TrainData;
    vector<vector<string>>TestData;

    //�ָ����ݼ�
    SplitData(allCar,TrainData,TestData,TrainRate,ShuffData);
    //���ɾ�����
    BegTime=clock();
    TreeNode *ModelWithoutPruning=TreeGenerateWithoutPruning(TrainData,attribute);
    EndTime=clock();
    CostTime=(double)(EndTime-BegTime)/CLOCKS_PER_SEC;
    printf("%.3f\n",CostTime);


    vector<vector<string>>SubTrainData=TrainData;
    vector<vector<string>>LimitData=TestData;
    //�ָ����ݼ���һ����Ϊ��֦���ж�����
    SplitData(TrainData,SubTrainData,LimitData,0.82,true);
    BegTime=clock();
    TreeNode *ModelWithPrepruning=TreeGenerateWithPrepruning(SubTrainData,LimitData,attribute);
    EndTime=clock();
    CostTime=(double)(EndTime-BegTime)/CLOCKS_PER_SEC;
    printf("%.3f\n",CostTime);

    BegTime=clock();
    TreeNode *ModelWithPostpruning=TreeGenerateWithPostpruning(SubTrainData,LimitData,attribute);
    EndTime=clock();
    CostTime=(double)(EndTime-BegTime)/CLOCKS_PER_SEC;
    printf("%.3f\n",CostTime);

    //������ȷ��
    TestModel(ModelWithoutPruning,TrainData,TestData,"ModelWithoutPruning");
    TestModel(ModelWithPrepruning,TrainData,TestData,"ModelWithPrepruning");
    TestModel(ModelWithPostpruning,TrainData,TestData,"ModelWithPostpruning");
    return 0;
}
