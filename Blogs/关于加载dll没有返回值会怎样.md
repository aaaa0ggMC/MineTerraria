#���ڼ���dllû�з���ֵ������
    ���췢�����´���������
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
            //����
            //����û�з���ֵ
        }
    '''
    ��������Ϊ0xC0000005,һ�������ϻ���ˣ�
    ���ǵ�ʱ������what�ˣ�û���ж��ֶ�null,��Ϊ�ֶα�������Ϊnull,Info�ṹ����
    '''C++
        //Struct Info
        struct Info{
            string author;
            string description;
            string version;
        }
    '''
    �����ұ������һЩ����

