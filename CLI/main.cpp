#include "Lib/HuffmanCode.cpp"
#include "Lib/HuffmanDecode.cpp"

int main(int argc, char *argv[])
{

    if (argc >= 1)
    {
        if (argc == 1)
        {
            cout << "���ߣ�20170333"
                 << "����˵����" << endl
                 << "HuffmanCoder -code �ļ���:        ѹ���ļ�" << endl
                 << "HuffmanCoder -decode �ļ���:        ��ѹ���ļ�" << endl;
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
                cout << "��������" << endl;
                system("pause");
            }
        }
    }
    else
    {
        cout << "��������" << endl;
    }

    return 0;
}