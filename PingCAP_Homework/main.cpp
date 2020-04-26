//
//  main.cpp
//  PingCAP_Homework
//
//  这里都采用1000进制而不是1024进制
//  并且规定每一个URL都是1000字节
//
//  Created by Jack Sun on 2020/4/26.
//  Copyright © 2020 Jack Sun. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <ctime>
using namespace std;

void fileGeneratorNGB(int n);//生成NGB的URL文件
char* URL_Generator(int size_in_megabyte);//生成size_in_megabyte
const int URL_SIZE=1000;
const int GEGA_BYTE=1000000000;

struct node{     //计算用的结构体
    string URL;  //存放URL和次数
    int time;
    node(){}
    node(int time,string url){
        this->time=time;
        this->URL=url;
    }
    bool friend operator < (const node &a,const node &b){
        if(a.time==b.time){
            return a.URL<b.URL;
        }
        else{
            return a.time>b.time;
        }
    }
};

int main(int argc, const char * argv[]) {
    cout<<"Start Generate File:text.txt"<<endl;
    fileGeneratorNGB(1); //这里的生成函数效率不高    我的电脑只有50多g可用空间了 无法测试25g以上的样例
    cout<<"Generated:text.txt"<<endl;
    time_t start=clock();
    hash<string> stringHash;  //hasher
    
    cout<<"Start creating cache files"<<endl;
    FILE *HashFiles[5000];  //5000个cache文件
    for(int i=0;i<5000;i++){ //创建5000个cache文件 文件名为hash值 0-4999
        char fileName[10];
        sprintf(fileName, "%d",i);
        HashFiles[i] = fopen(fileName, "w+");
    }
    cout<<"Cache file generated"<<endl;
    
    cout<<"Start hashing to cache"<<endl;
    
    FILE *data = fopen("text.txt", "r"); //这里的数据是我的那个生成函数生成的文件
    if(data==nullptr){
        cout<<"can't open file!!"<<endl;
        cout<<"make some clean"<<endl;
        for(int i=0;i<5000;i++){
            char fileName[10];
            sprintf(fileName, "%d",i);
            remove(fileName);
            fclose(HashFiles[i]);
        }
        return 0;
    }
    char *buffer = (char*)malloc(GEGA_BYTE); //1GB 的buffer
    int finished=1;
    while(fread(buffer, GEGA_BYTE, 1, data)==1){  //每次读1GB文件 读到buffer中
        string temp;                           //这两个临时存URL
        char copyTemp[URL_SIZE];
        for(int i=0;i<GEGA_BYTE/URL_SIZE;i++){    //把1GB的Buffer中的URL分成单个URL 并且计算hash值 存入对应文件
            strncpy(copyTemp, buffer+i*URL_SIZE, URL_SIZE); //根据偏移把第i个url存到copyTemp中
            temp=copyTemp;
            unsigned long long hashValue=stringHash(temp);   //计算hash
            unsigned long long arrayIndex=hashValue%5000;    //计算下标
            fwrite(copyTemp, URL_SIZE, 1, HashFiles[arrayIndex]);  //存到cache中
           // cout<<hashValue<<endl;
        }
        cout<<(finished++)<<"GB Finished"<<endl;
    }
    free(buffer);
    cout<<"Finish!!!"<<endl;
    
    cout<<"Start calculate every hash cache file"<<endl;
    vector<node> result[5000];   //这个存每个下标的前100url
    for(int i=0;i<5000;i++){    //对5000个hash cache文件遍历
        map<string,int> rec;    //这个暂存 url-次数
        char temp[1000];
        rewind(HashFiles[i]);  //把指针移至文件头
        while(fread(temp, URL_SIZE, 1, HashFiles[i])==1){  //每次读一个URL
            string toRec=temp;
            rec[toRec]++;
        }
        priority_queue<node> q;    //小顶堆 用C++内部的优先队列
        for(map<string, int>::iterator it=rec.begin();it!=rec.end();it++){//遍历结果
            if(q.size()<100){               //维护100个元素小顶堆 即可得出前100大
                q.push(node(it->second,it->first));
            }
            else{     //多于100就放进去再弹出最小的
                q.push(node(it->second,it->first));
                q.pop();
            }
        }
        while(q.size()){   //将堆中元素存到数组
            result[i].push_back(q.top());
            q.pop();
        }
    }
    cout<<"Finish!!!"<<endl;
    
    cout<<"Start calculate final result"<<endl;
    priority_queue<node> finalResult;  //统计最终结果的小顶堆
    for(int i=0;i<5000;i++){
        for(int j=0;j<result[i].size();j++){  //同样要维护100元素小顶堆
            if(finalResult.size()<100){
                finalResult.push(result[i][j]);
            }
            else{
                finalResult.push(result[i][j]);
                finalResult.pop();
            }
        }
    }
    time_t end=clock();
    cout<<"Finish!! time use:"<<(((double)end-start)/(double)CLOCKS_PER_SEC)<<endl;
    
    cout<<"That's the Result!!! from small to big"<<endl;
    while(finalResult.size()){   //输出结果
        node out=finalResult.top();
        finalResult.pop();
        for(int i=0;i<out.URL.length();i++){
            if(out.URL[i]==' '){
                break;
            }
            cout<<out.URL[i];
        }
        cout<<" "<<out.time<<endl;
    }
    cout<<"make some clean"<<endl;
    for(int i=0;i<5000;i++){
        char fileName[10];
        sprintf(fileName, "%d",i);
        remove(fileName);
        fclose(HashFiles[i]);
    }
    remove("text.txt");
    cout<<"Bye"<<endl;
    return 0;
}

void fileGeneratorNGB(int n){
    FILE *file = fopen("text.txt", "w");
    if(file==NULL){
        cout<<"can't create"<<endl;
        return;
    }
    int mega=100;
    int times=n*1000/mega;
    for(int i=0;i<times;i++){
        char *ptr=URL_Generator(mega);
        fwrite(ptr, mega*1000000, 1, file);
        free(ptr);
    }
    fclose(file);
}

char* URL_Generator(int size_in_megabyte){
    srand(time(NULL));
    char *ptr = (char*)malloc(size_in_megabyte*1000*1000);
    //printf("%lld",size_in_megabyte*1000*1000);
    memset(ptr, ' ', size_in_megabyte*1000*1000);
    long long urlCount=size_in_megabyte*1000;
    char prefix[30]="https://www.testURL.com/";
    int lenPrefix=strlen(prefix);
    for(int i=0;i<urlCount;i++){
        for(int j=0;j<lenPrefix;j++){
            ptr[j+i*1000]=prefix[j];
        }
        int rad1=rand()%10,rad2=rand()%10,rad3=rand()%10;
        ptr[24+i*1000]=rad1+'0';
        ptr[25+i*1000]=rad2+'0';
        ptr[26+i*1000]=rad3+'0';
        ptr[999+i*1000]='\n';
    }
    return ptr;
}
