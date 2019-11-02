#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include<bits/stdc++.h>
#define pis pair<int,string>
#define mkp(a,b) make_pair(a,b)
#define MaxLabel 4
#define MaxFeature 6
#define MaxSubFeature 10
using namespace std;

vector<vector<string>>allCar;
vector<string>labels{"unacc", "acc", "good", "vgood"};
vector<string>attribute{"buying", "maint", "doors", "persons", "lug_boot", "safety"};
vector<string>features[MaxFeature]{
    {"v-high","high","med","low"},
    {"v-high","high","med","low"},
    {"2","3","4","5-more"},
    {"2","4","more"},
    {"small","med","big"},
    {"low","med","high"}
};
//#define MaxLabel 2
//#define MaxFeature 6
//#define MaxSubFeature 10
//vector<vector<string>>allCar;
//vector<string>labels{"ÊÇ", "·ñ"};
//vector<string>attribute{"É«Ôó", "¸ùµÙ", "ÇÃÉù", "ÎÆÀí", "Æê²¿", "´¥¸Ð"};
//vector<string>features[MaxFeature]{
//    {"ÇàÂÌ","ÎÚºÚ","Ç³°×"},
//    {"òéËõ","ÉÔòé","Ó²Í¦"},
//    {"×ÇÏì","Çå´à","³ÁÃÆ"},
//    {"ÇåÎú","ÉÔºý","Ä£ºý"},
//    {"°¼ÏÝ","ÉÔ°¼","Æ½Ì¹"},
//    {"Ó²»¬","ÈíÕ³"}
//};
/*
    0¡¢buying:   v-high, high, med, low->0¡¢1¡¢2¡¢3
    1¡¢maint:    v-high, high, med, low->0¡¢1¡¢2¡¢3
    2¡¢doors:    2, 3, 4, 5-more       ->0¡¢1¡¢2¡¢3
    3¡¢persons:  2, 4, more            ->0¡¢1¡¢2
    4¡¢lug_boot: small, med, big       ->0¡¢1¡¢2
    5¡¢safety:   low, med, high        ->0¡¢1¡¢2
*/
map<string,int>MapOfLabel;
map<string,int>MapOfFeature;
map<string,int>MapOfFeatureDetail[MaxFeature];
void Init(){
    for(int i=0;i<(int)labels.size();i++)
        MapOfLabel[labels[i]]=i;
    for(int i=0;i<(int)attribute.size();i++)
        MapOfFeature[attribute[i]]=i;
    for(int i=0;i<MaxFeature;i++){
        for(int j=0;j<(int)features[i].size();j++){
            MapOfFeatureDetail[i][features[i][j]]=j;
        }
    }
}

#endif // INIT_H_INCLUDED
