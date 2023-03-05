//ctool.cpp ctool.h的实现，需要自取，大部分代码为网上找的，只作为一个工具包，没有商业用途
#include "./ctool.h"
#include <iostream>


using namespace std;
using namespace cck;
using namespace strps;
using namespace fileIO;
using namespace strps::encoding;
using namespace num::random;
using namespace num::vectors;

string strps::GetTranslateString(string in){
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


int fileIO::file_size(char* filename){
    struct stat statbuf;
    int ret;
    ret = stat(filename,&statbuf);//调用stat函数
    if(ret != 0) return -1;//获取失败。
    return statbuf.st_size;//返回文件大小。
}

int fileIO::writeAll(string fth,string s){
    ofstream of;
    of.open(fth);
    if(!of.is_open())return -1;
    of.write(s.c_str(),s.length());
    of.close();
    return 0;
}

string fileIO::readAll(ifstream & reader){
    string rt = "";
    string tmp = "";
    while(!reader.eof()){
        getline(reader,tmp);
        rt += tmp + "\n";
    }
    return rt;
}

string strps::Trim(string & str){
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

void strps::split(vector<string> & vct,const string & line,const char sep){
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

void strps::Stringsplit(string str, string splits, vector<string>& res){
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

bool fileIO::check_exists(char* name) {
  struct stat buffer;
  return (stat (name, &buffer) == 0);
}

string strps::encoding::GBKToUTF8(const string &strGBK){
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

string strps::encoding::UTF8ToGBK(const string &strUTF8){
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

void num::random::ShakeInit(num::random::ShakeSt & st,float timePerRound,float minv,float maxv,float start,int startDiection){
    st.insideClock.Stop();
    st.shakeValue = start;
    if(maxv > minv){
        st.mn = minv;
        st.mx = maxv;
    }else{
        st.mn = maxv;
        st.mx = minv;
    }
    st.stDirection = startDiection;
    st.timePerRound = timePerRound;
    st.time = 0;
    st.insideClock.Start();
}

void num::random::Shake(num::random::ShakeSt & st){
    float nowOff =  ((float)st.insideClock.GetOffset())/((float)1000);
    //cout << nowOff << endl;
    float off = (float)(st.mx - st.mn) * ((float)nowOff / (float)st.timePerRound);
    float off1 = st.shakeValue + off * st.stDirection;
    if(off1 >= st.mx){
        off1 = st.mx;
        st.stDirection = -1;
    }else if(off1 <= st.mn){
        off1 = st.mn;
        st.stDirection = 1;
    }
    st.shakeValue = off1;
}

num::vectors::Vector num::vectors::Normalize(num::vectors::Vector v){
    float sum = v.x*v.x + v.y*v.y + v.z*v.z;
    float len = sqrt(sum);
    return Vector(v.x / len,v.y / len,v.z / len);
}

num::vectors::Vector::Vector(float v0,float v1,float v2){
    this->x = v0;
    this->y = v1;
    this->z = v2;
}
