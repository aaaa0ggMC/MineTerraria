#include <iostream>
#include <rapidjson.h>

using namespace std;
using namespace alib;

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

int main()
{
    testDyJSON();
    return 0;
}

/*

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
