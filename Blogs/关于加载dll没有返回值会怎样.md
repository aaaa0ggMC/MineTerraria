#关于加载dll没有返回值会怎样
    今天发现如下代码会崩溃：
    '''C++
        //main.cpp exe
        HMODULE md = LoadLibrary("ULSimpleLib.dll");
        FUNC fn = (FUNC)GetProcAddress(md,"GetData");
        Info info = fn(basicInfo);
        cout << info.data << endl;
    '''
    '''C++
        //main.cpp dll
        Info GetData(BasicInfo bf){
            //处理
            //这里没有返回值
        }
    '''
    崩溃代码为0xC0000005,一看就是老伙计了！
    但是当时我脑子what了，没有判断字段null,因为字段本身不可能为null,Info结构如下
    '''C++
        //Struct Info
        struct Info{
            string author;
            string description;
            string version;
        }
    '''
    于是我便采用了一些方法

