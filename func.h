#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

vector<pis> CheckInformationOfCars(vector<vector<string>>cars){
    vector<pis>ret;
    int CntOfCars=cars.size();
    for(int i=0;i<CntOfCars;i++){
        int foundTheLab=0;
        string lab=cars[i][cars[i].size()-1];   //当前需要存的标签
        for(int j=0;j<(int)ret.size();j++){
            string ftr=ret[j].second;           //已经存下来了的标签
            if(ftr==lab){                       //找到了直接+1
                ret[j].first++;
                foundTheLab=1;
                break;
            }
        }
        if(!foundTheLab){
            ret.push_back(mkp(1,lab));
        }
    }
    if(!ret.size())
        ret.push_back(mkp(0,"error"));
    return ret;
}
bool FeatureEqual(vector<vector<string>>cars,vector<string>feature){
    for(int i=1;i<(int)cars.size();i++){
        for(int j=0;j<(int)feature.size();j++){
            int idx=MapOfFeature[feature[j]];
            if(cars[i][idx]!=cars[i-1][idx]){
                return false;
            }
        }
    }
    return true;
}
double CalculateEntD(vector<vector<string>>cars){
    double EntD=0,cnt[MaxLabel];
    for(int i=0;i<MaxLabel;i++)
        cnt[i]=0;
    for(int i=0;i<(int)cars.size();i++){
        cnt[MapOfLabel[cars[i][cars[i].size()-1]]]++;
    }
    for(int i=0;i<MaxLabel;i++){
        double pk=cnt[i]/cars.size();
        if(cnt[i]>1e-5)EntD-=pk*log2(pk);
    }
    return EntD;
}
vector<string> CancleNumber(vector<string>feature,string cancle){
    for(int i=0;i<(int)feature.size();i++){
        if(feature[i]==cancle){
            for(int j=i+1;j<(int)feature.size();j++){
                feature[j-1]=feature[j];
            }
            break;
        }
    }
    feature.pop_back();
    return feature;
}
pair<int,double> ChooseTheMinEnt(vector<vector<string>>cars,vector<string>feature){
    vector<vector<string>>subCars[MaxSubFeature];
    int choose=-1;
    double MinEnt=1e9;
    for(int i=0;i<(int)feature.size();i++){   //从A中划分最优集合
        int idx=MapOfFeature[feature[i]];//第idx个属性
        for(int j=0;j<(int)features[idx].size();j++)
            subCars[j].clear();
        for(int j=0;j<(int)cars.size();j++){
            string subFeature=cars[j][idx];
            int subIdx=MapOfFeatureDetail[idx][subFeature];
            subCars[subIdx].push_back(cars[j]);
        }
        double tmpEnt=0;
        for(int j=0;j<(int)features[idx].size();j++){
            tmpEnt+=subCars[j].size()*CalculateEntD(subCars[j])/cars.size();

        }
        if(choose==-1||MinEnt>=tmpEnt){
            choose=idx;
            MinEnt=tmpEnt;
        }
    }
    return mkp(choose,MinEnt);
}


#endif // FUNC_H_INCLUDED
