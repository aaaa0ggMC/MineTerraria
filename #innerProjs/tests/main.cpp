#include <iostream>
#include <rapidjson.h>
#include <CClock.h>
#include <spdlog.h>

using namespace std;
using namespace alib;

void testSeri(){

}

int main()
{
    testSeri();
    //testCClock();
    //testDyJSON();
    //testLog();
    system("pause");
    return 0;
}

/*
void testLog(){
    LogSaver ls;
    ls << "Hello world!";
    ls.setLevel(LOG_CRITI);
    ls << "NMSL";
    ls.SetFileOutput("D:/log0.log","SB");
    ls << "HAHA";
}

void testCClock(){
    cck::Clock clk;
    Sleep(1000);
    cout << clk.Now().all << endl;
}
void testDyJSON(){
    string str =
    R"({
    "item0":{"item1":"2233"},"i2":"22"
    })";
    cout << str << "\n";
    Document doc;
    cout << doc.Parse(str) << "\n";
    cout << doc.GetValue("item0");
    doc.ForEach([](string a,string b,void* reserved){
        //string * str = static_cast<string*>(reserved);
        cout << a << " " << b << endl;
    },NULL);
}
void testJSON(){
    string str =
    R"({
    "item0":{"item1":"2233"},"i2":"22"
    })";
    cout << str << "\n";
    Document doc;
    cout << doc.Parse(str) << "\n";
    cout << doc.GetValue("item0");
    doc.ForEach([](string a,string b,void* reserved){
        //string * str = static_cast<string*>(reserved);
        cout << a << " " << b << endl;
    },NULL);
}

*/
