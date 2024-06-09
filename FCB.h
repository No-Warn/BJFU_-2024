#include <string>
#define MAX_FCB_COUNT 10000 // ����ļ����ƿ�����Ϊ10000�������10���û���ÿ���û�ռ1000���ļ����ƿ�
#define MAX_CONTEXT 100
#define EVERY_USER_FCB_COUNT MAX_FCB_COUNT / 10
using namespace std;

class FCB
{
public:
    string isused;               // ���ļ����ƿ��Ƿ�ʹ��
    string name;                 // �ļ�����
    string type;                 // 0��ʾ�ļ���1��ʾĿ¼
    int address;                 // �ļ���ַ�����ļ����ƿ�ı�ű�ʾ
    string dir;                  // �����洢�ļ���Ŀ¼λ��
    int lock;                    // �ļ���
    int line;                    // �ļ�����
    string content[MAX_CONTEXT]; // �ļ�����
    FCB(/* args */);
    ~FCB();
};

FCB::FCB()
{
    isused = "0";
    name = "null";
    type = "-1";
    address = -1;
    dir = "null_dir";
    lock = -1;
    line = 1;
    content[0] = "null_content";
}

FCB::~FCB()
{
}
