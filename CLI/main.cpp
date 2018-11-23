#include "Lib/HuffmanCode.cpp"
#include "Lib/HuffmanDecode.cpp"

int main(int argc, char *argv[])
{

    if (argc >= 1)
    {
        if (argc == 1)
        {
            cout << "作者：20170333"
                 << "帮助说明：" << endl
                 << "HuffmanCoder -code 文件名:        压缩文件" << endl
                 << "HuffmanCoder -decode 文件名:        解压缩文件" << endl;
            system("pause");
        }
        else
        {
            if (strcmp(argv[1], "-code") == 0)
            {
                HuffmanCode(argv[2]);
            }
            else if (strcmp(argv[1], "-decode") == 0)
            {
                HuffmanDecode(argv[2]);
            }
            else
            {
                cout << "参数错误！" << endl;
                system("pause");
            }
        }
    }
    else
    {
        cout << "参数错误！" << endl;
    }

    return 0;
}