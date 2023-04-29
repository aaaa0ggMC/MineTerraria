#include "aaa_util.h"
#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <sys/stat.h>
#include <psapi.h>

using namespace std;
using namespace alib;

int Util::colorfulPrint(const string message,int color){
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),(WORD)color);
    int rt = printf("%s",message.c_str());
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),info.wAttributes);
    return rt;
}

string Util::getTime(){
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

string Util::Windows_getCPUInfo(){
    long lRet;
	HKEY hKey;
	CHAR tchData[1024];
	DWORD dwSize;

	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,TEXT("Hardware\\Description\\System\\CentralProcessor\\0"),0,KEY_QUERY_VALUE,&hKey);

	if(lRet == ERROR_SUCCESS){
		dwSize = 1024;
		lRet = RegQueryValueEx(hKey,TEXT("ProcessorNameString"),0,0,(LPBYTE)tchData,&dwSize);

		tchData[dwSize] = '\0';
		RegCloseKey(hKey);
		if(lRet == ERROR_SUCCESS){
			return string(tchData);
		}else{
			return "Unknown";
		}
	}
	return "Unknown";
}

void Util::getFileNames(string path, vector<string>& files){
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

string Util::GetTranslateString(string in){
    string out = "";
    for(size_t i = 0;i < in.length();i++){
        if(in[i] == '\\'){
            switch(in[++i]){
            case 'n'://New Line
                out += '\n';
                break;
            case '\\'://Backslash
                out += '\\';
                break;
            case 'v'://vertical
                out += '\v';
                break;
            case 't'://tab
                out += '\t';
                break;
            case 'r'://return
                out += '\r';
                break;
            case 'a'://alll
                out += '\007';
                break;
            default:
                i--;
                out += in[i];
                break;
            }
        }else{
            out += in[i];
        }
    }
    return out;
}


int Util::file_size(char* filename){
    struct stat statbuf;
    int ret;
    ret = stat(filename,&statbuf);//调用stat函数
    if(ret != 0) return -1;//获取失败。
    return statbuf.st_size;//返回文件大小。
}

int Util::writeAll(string fth,string s){
    ofstream of;
    of.open(fth);
    if(!of.is_open())return -1;
    of.write(s.c_str(),s.length());
    of.close();
    return 0;
}

string Util::readAll(ifstream & reader){
    string rt = "";
    string tmp = "";
    while(!reader.eof()){
        getline(reader,tmp);
        rt += tmp + "\n";
    }
    return rt;
}

string Util::Trim(string & str){
    string blanks("\f\v\r\t\n ");
    string temp;
    for(int i = 0;i < (int)str.length();i++){
        if(str[i] == '\0')
            str[i] = '\t';
    }
    str.erase(0,str.find_first_not_of(blanks));
    str.erase(str.find_last_not_of(blanks) + 1);
    temp = str;
    return temp;
}

void Util::split(vector<string> & vct,const string & line,const char sep){
    const size_t size = line.size();
    const char* str = line.c_str();
    int start = 0,end = 0;
    for(int i = 0;i < (int)size;i++){
        if(str[i] == sep){
            string st = line.substr(start,end);
            Trim(st);
            vct.push_back(st);
            start = i + 1;
            end = 0;
        }else
            end++;
    }
    if(end > 0){
        string st = line.substr(start,end);
        Trim(st);
        vct.push_back(st);
    }
}

void Util::Stringsplit(string str, string splits, vector<string>& res){
    if (str == "")		return;
    //在字符串末尾也加入分隔符，方便截取最后一段
    string strs = str + splits;
    size_t pos = strs.find(splits);
    int step = splits.size();

    // 若找不到内容则字符串搜索函数返回 npos
    while (pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        res.push_back(temp);
        //去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + step, strs.size());
        pos = strs.find(splits);
    }
}

bool Util::check_exists(char* name) {
  struct stat buffer;
  return (stat (name, &buffer) == 0);
}

string Util::GBKToUTF8(const string &strGBK){
	string strOutUTF8 = "";
	WCHAR *str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(),  - 1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(),  - 1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1,  - 1, NULL, 0, NULL, NULL);
	char *str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1,  - 1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete [] str1;
	str1 = NULL;
	delete [] str2;
	str2 = NULL;
	return strOutUTF8;
}

string Util::UTF8ToGBK(const string &strUTF8){
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(),  - 1, NULL, 0);
	WCHAR *wszGBK = new WCHAR[len + 1];
	memset(wszGBK, 0, (len+1)*sizeof(WCHAR));
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUTF8.c_str(),  - 1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK,  - 1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK,  - 1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	string strTemp(szGBK);
	delete [] szGBK;
	szGBK = NULL;
	delete [] wszGBK;
	wszGBK = NULL;
	return strTemp;
}

string Util::translateSeconds(int msecs){
    int sec = msecs%60;
    msecs /= 60;
    int min = msecs % 60;
    msecs /= 60;
    int hour = msecs % 60;
    msecs /= 60;
    int day = msecs % 24;
    msecs /= 24;
    int year = msecs % 356;
    string ret = "";
    if(year != 0)ret += to_string(year) + "y ";
    if(day != 0)ret += to_string(day) + "d ";
    if(hour != 0)ret += to_string(hour) + "h ";
    if(min != 0)ret += to_string(min) + "m ";
    if(sec != 0)ret += to_string(sec) + "s";
    return ret;
}

MemTp Util::GetCurrentMemoryUsage(){
    uint64_t mem = 0, vmem = 0;
    PROCESS_MEMORY_COUNTERS pmc;

    // get process hanlde by pid
    HANDLE process = GetCurrentProcess();
    if (GetProcessMemoryInfo(process, &pmc, sizeof(pmc)))
    {
        mem = pmc.WorkingSetSize;
        vmem = pmc.PagefileUsage;
    }
    CloseHandle(process);

    // use GetCurrentProcess() can get current process and no need to close handle

    // convert mem from B to MB
    return {(float)(mem / 1024.0 / 1024.0),(float)(vmem/1024.0/1024.0)};
}

GlMem Util::GetGlobalMemoryUsage(){
    MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	DWORDLONG physical_memory = statex.ullTotalPhys / (1024 * 1024);
	DWORDLONG virtual_memory = (DWORD)statex.ullAvailVirtual / (1024 * 1024);
	DWORDLONG usePhys = physical_memory - (statex.ullAvailPhys / (1024 * 1024));

	float percent_memory = (float)usePhys / (float)physical_memory;
	return {percent_memory,(float)physical_memory,(float)virtual_memory,(float)usePhys};
}

CPUInfo::CPUInfo(){
    #ifdef BUILD_ON_WINDOWS
    this->CpuID = _Windows_getCPUInfo();
    #endif // BUILD_ON_WINDOWS
}
