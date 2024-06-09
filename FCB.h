#include <string>
#define MAX_FCB_COUNT 10000 // 最大文件控制块数量为10000，最多有10个用户，每个用户占1000个文件控制块
#define MAX_CONTEXT 100
#define EVERY_USER_FCB_COUNT MAX_FCB_COUNT / 10
using namespace std;

class FCB
{
public:
    string isused;               // 该文件控制块是否被使用
    string name;                 // 文件名称
    string type;                 // 0表示文件，1表示目录
    int address;                 // 文件地址，用文件控制块的编号表示
    string dir;                  // 用来存储文件的目录位置
    int lock;                    // 文件锁
    int line;                    // 文件行数
    string content[MAX_CONTEXT]; // 文件内容
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
