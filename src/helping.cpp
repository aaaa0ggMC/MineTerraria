#include "helping.h"

using namespace std;

template<class K,class V> int MapCheckKey(const map<K,V> & mapA,const K & key){
    typename map<K,V>::const_iterator iter = mapA.find(key);//冷知识
    return (iter == mapA.end()) ? 0 : 1;
}

void getFileNames(string path, vector<string>& files){
	//文件句柄
	//注意：我发现有些文章代码此处是long类型，实测运行中会报错访问异常
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,递归查找
			//如果不是,把文件绝对路径存入vector中
			if ((fileinfo.attrib & _A_SUBDIR))
			{
			    ///In this app,we do not need to do it
				//if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					//getFileNames(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

string getTime(){
    time_t rawtime;
    struct tm *ptminfo;
    string rt = "";
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    char * mdate = (char *)malloc(sizeof(char) * (1024));
    memset(mdate,0,sizeof(char) * (1024));
    sprintf(mdate,"%02d-%02d-%02d %02d:%02d:%02d",
    ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
    ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
    rt = mdate;
    free(mdate);
    return rt;
}
