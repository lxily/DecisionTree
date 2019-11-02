#决策树
    1、使用C++实现了决策树的建立与查询
    2、在实现了有无剪枝的决策树：无剪枝、预剪枝与后剪枝


#实验环境
    1、codeblocks 16.01
    2、-std=g++11
    *说明：笔者在codeblocks下建的工程并测试，用其余的工具进行一定的修改也是可行的

#项目使用说明
    1、笔者给出了实验所用的测试数据文件car.txt，此时在init.h中初始化也是对应的数据来进行的
    2、故直接运行主函数main.cpp，得到的是对于数据集car.txt的测试结果
    3、如果想改为其它的数据集，需要按照相应的方法在init.h文件中修改(文件中已经给出了西瓜数据集2.0的初始化方法)

#文件说明
    1、init.h：初始化数据之间的关系，在主函数中会调用Init()函数
    2、node.h：定义了决策树结点的数据结构
    3、func.h：实现了基本的操作函数，具体来说有：
        ①CheckInformationOfCars(cars)：统计数据集中有多少种不同的结果(标签)及对应数目，返回结果为所有的数据对
        ②FeatureEqual(cars,feature)：判断当前数据集cars在属性feature上的值是否全部一样
        ③CalculateEntD(cars)：计算当前数据集的信息熵
        ④CancleNumber(feature,cancle)：从属性集feature种删除属性cancle
        ⑤ChooseTheMinEnt(cars,feature)：计算将数据集car根据剩余属性分类的结果得到的信息熵的总和，返回一个最小值及对应的属性编号
    4、model.h：实现了决策树无剪枝、预剪枝、后剪枝三种情况下的建立：
        ①TreeGenerateWithoutPruning(cars,feature):无剪枝情况，car为初始数据集，feature为属性的集合
        ②TreeGenerateWithPrepruning(cars,limit,feature):预剪枝情况，car为数据集，limit为用于剪枝的验证集，feature为属性的集合
        ③TreeGenerateWithPostpruning(cars,limit,feature):后剪枝情况，car为数据集，limit为用于剪枝的验证集，feature为属性的集合
        ④TreeCaculateNodeCnt(*model):统计决策树的结点个数，用于对比剪枝与无剪枝的两种情况
        ⑤TreeQuery(*model,car):在model查询并判断对car类型的判定是否正确，正确返回true，反之则反之
    5、main.h:主函数，进行了文件的输入，以及各个函数的调用，其细节再代码中已有注释