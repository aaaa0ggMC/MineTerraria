# 关于加载dll没有返回值会怎样

    今天发现如下代码会崩溃：

```cpp
//main.cpp exe
HMODULE md = LoadLibrary("ULSimpleLib.dll");
FUNC fn = (FUNC)GetProcAddress(md,"GetData");
Info info = fn(basicInfo);
cout << info.data << endl;
```

```cpp
//main.cpp dll
Info GetData(BasicInfo bf){
    //处理
    //这里没有返回值
}
```

    崩溃代码为0xC0000005,一看就是老伙计了！
    但是当时我脑子what了，没有判断字段null,因为字段本身不可能为null,Info结构如下

```cpp
//Struct Info
    struct Info{
    string author;
    string description;
    string version;
}
```

    于是我便采用了一些方法

## 方法

### 采用Try Catch

    于是我便用了Try Catch 来检测异常并尝试处理，可是结果却令人大吃一惊，第一次时，完美略过，没有出错。但是第二次时，程序又崩溃了。于是我便放弃了try catch，想到了另一个原因

### 判断引用内的字符串的值

```cpp
if(info.author.c_str() == NULL){
    cout << "Hello" << endl;
}
```

好的，果然，程序输出了hello,因此最后的结果就得出来了。

## 结论

    当函数不尝试返回值时，系统会默认设置值的指针为null,即结构体中有用数据为0，或者是垃圾数据。
