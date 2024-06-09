#include <time.h>
#include "file.h"
#include <windows.h>
#define MAX_USR_COUNT 3 // 最大用户数量为10
using namespace std;

/*
代码中的文件说明：
    disk.txt用于模拟虚拟磁盘，其中存储着文件系统的文件控制块
    usr_pwd文件用于存储已经注册的用户名和密码
    main.cpp文件为系统的入口
    dirent.h文件主要用于保存文件目录的信息
    file.h文件用于保存文件的相关信息
*/

int seek = 0;          // 用于保存文件指针偏移量
string current_dirent; // 全局变量用来存储当前所在目录
int file_pos;          // 全局变量用来存储打开文件后的FCB编号

typedef struct usr_pwd
{
    // 用于保存用户名和密码
    string name;
    string password;
    int no; // 用于存储用户编号
} usr_pwd;

usr_pwd login()
{
    usr_pwd currentname; // 当前用户
    // 注册、登录
    int count = 0;
    usr_pwd user[MAX_USR_COUNT]; // 创建用户与密码对应的对象
    // 打开存储账号密码的文件
    ifstream file("usr_pwd.txt");
    if (!file)
    {
        perror("Error opening file!");
    }
    else
    {
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            if (!(iss >> user[count].name >> user[count].password >> user[count].no))
            {
                // 如果失败则读取下一行
                continue;
            }
            // cout << user[count].name << " " << user[count].password << " " << user[count].no << endl;
            count++;
        }
    }
    file.close();
    // 选择登录或者注册
    cout << "Please select Login (1) or Register an Account (2):" << endl;
    int select;
    cin >> select;
    if (select == 1)
    {
        // 用户登录
        int flag1 = 1;
        while (flag1 == 1)
        {
            string name;
            cout << "Username: ";
            cin >> name;
            int i = 0;
            for (; i < MAX_USR_COUNT; i++)
            {
                if (user[i].name == name)
                    break;
            }
            string pwd;
            cout << "Password: ";
            cin >> pwd;
            // 确定账号和密码是否正确
            if (user[i].password == pwd)
            {
                flag1 = 0; // 终止循环
                cout << "Login successful! Initializing file system..." << endl;
                currentname = user[i];
            }
            else
            {
                cout << "Wrong username or password!" << endl;
            }
        }
    }
    else if (select == 2)
    {
        // 注册用户
        if (count > MAX_USR_COUNT - 1)
        {
            // 用户数量过多
            cout << "The current number of users has exceeded the maximum!" << endl;
        }
        else
        {
            int flag2 = 1;
            string name; // 用户名
            while (flag2 == 1)
            {
                cout << "Please enter your registered username:" << endl;
                cin >> name;
                int i = 0;
                for (; i < MAX_USR_COUNT; i++)
                {
                    if (user[i].name == name)
                    {
                        cout << "The username already exists!" << endl;
                        break;
                    }
                }
                if (i == MAX_USR_COUNT)
                {
                    flag2 = 0;
                }
            }
            cout << "Password: ";
            string pwd;
            cin >> pwd;
            cout << "The registration is successful, and the file system is about to enter..." << endl;
            // 将注册的用户名以及密码保存到文件中
            user[count].name = name;
            user[count].password = pwd;
            user[count].no = count;
            currentname = user[count];
            // 将新增的用户追加到文件结尾并且在虚拟磁盘disk.txt中分配相关空间
            // 在usr_pwd文件中追加内容
            ofstream file;
            file.open("usr_pwd.txt", ios::app);
            file << user[count].name << " " << user[count].password << " " << user[count].no << endl;
            file.close();
            count++; // 更新用户数量
        }
    }
    return currentname; // 返回当前用户
}

int Space_count(string str)
{
    // 用来查看输入的指令中的空格数量
    int space_count = 0;
    // 遍历字符串
    for (char c : str)
    {
        // 检查字符是否是空格
        if (c == ' ')
        {
            space_count++; // 如果是空格，增加计数器
        }
    }
    return space_count;
}

void help()
{
    // 文件系统的帮助文档
    cout << "               文件系统帮助文档               " << endl;
    cout << "文件操作" << endl;
    cout << "create   -[filename]                 创建文件" << endl;
    cout << "delete   -[filename]                 删除文件" << endl;
    cout << "close    -[filename]                 关闭文件" << endl;
    cout << "open     -[filename]                 打开文件" << endl;
    cout << "read                                   读文件" << endl;
    cout << "write                                  写文件" << endl;
    cout << "move     -[filename] -[path]         移动文件" << endl;
    cout << "copy     -[filename] -[filename]     拷贝文件" << endl;
    cout << "flock    -[filename]                 文件加锁" << endl;
    cout << "head  -[num] -[filename]       显示文件前num行" << endl;
    cout << "tail  -[num] -[filename]       显示文件后num行" << endl;
    cout << "lseek    -[filename]          文件读写指针移动" << endl;
    cout << "目录操作" << endl;
    cout << "cd       -[path]                  改变工作目录" << endl;
    cout << "dir                             列出目录下文件" << endl;
    cout << "mkdir    -[direntname]                创建目录" << endl;
    cout << "rmdir    -[direntname]                删除目录" << endl;
    cout << "import   -[path]             导入文件到当前目录" << endl;
    cout << "export -[filename] -[path]   导出文件到指定目录" << endl;
}

// 程序入口主函数
int main()
{
    cout << "Whether or not to use the system for the first time[Y/N]" << endl;
    char num;
    cin >> num;
    if (num == 'Y' || num == 'y')
    {
        write_to_disk();
    }
    else if (num == 'N' || num == 'n')
    {
        open_file_system();
    }
    usr_pwd temp;
    temp.name = "";
    while (temp.name == "")
    {
        temp = login();
        Sleep(1000);
        system("cls"); // 清空之前的登录信息
    }
    system("cls");
    current_dirent = temp.name;                          // 设置当前目录为用户登录名所指向的目录
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除输入缓冲区中的换行符，防止输出两次 FileSystem@job:~$
    while (1)
    {
        // current_dirent="root";
        if (current_dirent == temp.name)
            cout << temp.name << "@FileOS" << ":~$ ";
        else
            cout << temp.name << "@FileOS:~/" << current_dirent << "$ ";
        string code;
        getline(cin, code);
        // 调试输出信息
        //  int space=Space_count(code);
        //  cout<<space<<endl;
        //  cout<<endl;
        //  cout<<code.find("delete")<<endl;

        if (code == "clear")
        {
            system("cls"); // 清空之前输出的信息
        }
        else if (code == "exit")
        {
            cout << "The file system is shutting down..." << endl;
            Sleep(1000);
            exit(1);
        }
        else if (code == "help")
        {
            help();
        }
        else if (code.substr(0, 7) == "create " && (code.find("create") != string::npos) && Space_count(code) == 1)
        {
            string key, filename;
            istringstream iss(code);
            iss >> key >> filename;
            create(filename, current_dirent, temp.no); // 调用create函数创建文件
        }
        else if (code.substr(0, 7) == "delete " && (code.find("delete") != string::npos) && Space_count(code) == 1)
        {
            string key, filename;
            // cout << filename << endl;
            istringstream iss(code);
            iss >> key >> filename;
            Delete(current_dirent, filename, temp.no); // 调用delete函数来删除文件
        }
        else if (code.substr(0, 5) == "open " && (code.find("open") != string::npos) && Space_count(code) == 1)
        {
            if (file_pos > 0)
            {
                cout << "Already has file opened!" << endl;
                continue;
            }
            else
            {
                string key, filename;
                istringstream iss(code);
                iss >> key >> filename;
                file_pos = open(current_dirent, filename, temp.no); // 调用open函数来打开文件
                // if (file_pos < 0)
                // {
                //     cout << "Open file failed" << endl;
                // }
            }
        }
        else if (code.substr(0, 4) == "read" && (code.find("read") != string::npos) && Space_count(code) == 0)
        {
            read(file_pos, seek);
        }
        else if (code.substr(0, 5) == "write" && (code.find("write") != string::npos) && Space_count(code) == 0)
        {
            write(file_pos, seek);
        }
        else if (code.substr(0, 5) == "close" && (code.find("close") != string::npos) && Space_count(code) == 0)
        {
            file_pos = close();
            seek = 0; // 重置文件指针偏移量
        }
        else if (code.substr(0, 5) == "move " && (code.find("move") != string::npos) && Space_count(code) == 2)
        {
            string key, filename, path;
            istringstream iss(code);
            iss >> key >> filename >> path;
            move(current_dirent, filename, path, temp.no);
        }
        else if (code.substr(0, 5) == "copy " && (code.find("copy") != string::npos) && Space_count(code) == 2)
        {
            string key, file1, file2;
            istringstream iss(code);
            iss >> key >> file1 >> file2;
            copy(current_dirent, file1, file2, temp.no);
        }
        else if (code.substr(0, 6) == "flock " && (code.find("flock") != string::npos) && Space_count(code) == 1)
        {
            string key, filename;
            istringstream iss(code);
            iss >> key >> filename;
            flock(current_dirent, filename, temp.no);
        }
        else if (code.substr(0, 5) == "head " && (code.find("head") != string::npos) && Space_count(code) == 2)
        {
            string key, filename;
            int num;
            istringstream iss(code);
            iss >> key >> num >> filename;
            head_num(current_dirent, num, filename, temp.no);
        }
        else if (code.substr(0, 5) == "tail " && (code.find("tail") != string::npos) && Space_count(code) == 2)
        {
            string key, filename;
            int num;
            istringstream iss(code);
            iss >> key >> num >> filename;
            tail_num(current_dirent, num, filename, temp.no);
        }
        else if (code.substr(0, 6) == "lseek " && (code.find("lseek") != string::npos) && Space_count(code) == 1)
        {
            istringstream iss(code);
            string key;
            int num;
            iss >> key >> num;
            seek += num;
        }
        else if (code.substr(0, 7) == "import " && (code.find("import") != string::npos) && Space_count(code) == 1)
        {
            string key, path;
            istringstream iss(code);
            iss >> key >> path;
            import(path, current_dirent, temp.no);
        }
        else if (code.substr(0, 7) == "export " && (code.find("export") != string::npos) && Space_count(code) == 2)
        {
            string key, path, filename;
            istringstream iss(code);
            iss >> key >> filename >> path;
            Export(path, current_dirent, filename, temp.no);
        }
        else if (code.substr(0, 6) == "mkdir " && (code.find("mkdir") != string::npos) && Space_count(code) == 1)
        {
            string key, pathname;
            istringstream iss(code);
            iss >> key >> pathname;
            mkdir(pathname, current_dirent, temp.no);
        }
        else if (code == "dir")
        {
            dir(current_dirent, temp.no);
        }
        else if (code.substr(0, 3) == "cd " && (code.find("cd") != string::npos) && Space_count(code) == 1)
        {
            string key, dirent;
            istringstream iss(code);
            iss >> key >> dirent;
            string temp_dirent = current_dirent;
            current_dirent = cd(temp_dirent, dirent, temp.no);
            // cout<<"debug info"<<current_dirent<<endl;
            if (current_dirent == "null")
            {
                // 当没找到指定路径时
                cout << "No such file or directory!" << endl;
                current_dirent = temp_dirent;
            }
        }
        else if (code.substr(0, 6) == "rmdir " && (code.find("rmdir") != string::npos) && Space_count(code) == 1)
        {
            string key, dirent;
            istringstream iss(code);
            iss >> key >> dirent;
            cout << "Whether you are sure to delete the current directory and all files in the directory:[Y/N]:";
            string select;
            getline(cin, select);
            if (select == "y" || select == "Y")
            {
                open_file_system();
                rmdir(current_dirent, dirent, temp.no);
                write_to_disk();
                cout << "Dirent was successfully deleted!" << endl;
            }
            else
                continue;
        }
        else if (code.empty())
        {
            continue;
        }
        else
        {
            cout << code << ": command not found" << endl;
        }
    }
    return 0;
}