#include <bits/stdc++.h>
#include <fcntl.h>
#include <windows.h>
#include "FCB.h"
using namespace std;

/*
    该文件用于保存文件的相关结构以及相关操作
    create		创建文件
    delete		删除文件
    open		打开文件
    close		关闭文件
    read		读文件
    write		写文件
    move        移动文件
    copy  		拷贝文件
    flock       文件加锁，需要实现加锁和解锁功能
    head –num  	显示文件的前num行
    tail  -num  显示文件尾巴上的num行
    lseek       文件读写指针的移动，从文件指针当前位置处向后移动 offset，负数时向前移动offset
*/

// 初始化文件系统，创建MAX_FCB_COUNT个文件控制块
FCB FCB[MAX_FCB_COUNT];

void write_to_disk()
{
    // 将相关信息写入disk的相关位置
    ofstream file("disk.txt");
    if (!file)
    {
        perror("Open file failed!");
        return;
    }
    else
    {
        // 将文件控制块写入文件中
        for (int i = 0; i < MAX_FCB_COUNT; i++)
        {
            file << FCB[i].isused << " " << FCB[i].name << " " << FCB[i].type << " " << FCB[i].address << " " << FCB[i].dir << " " << FCB[i].lock << " " << FCB[i].line << endl;
            for (int k = 0; k < FCB[i].line; k++)
            {
                file << FCB[i].content[k] << endl;
            }
            file << "#" << endl;
        }
    }
    file.close();
}

void open_file_system()
{
    ifstream file("disk.txt");
    if (!file)
    {
        perror("Open file failed!");
        return;
    }
    string line;
    int i = 0;
    while (getline(file, line) && i < MAX_FCB_COUNT)
    {
        if (line == "#")
        {
            ++i;
            continue;
        }
        istringstream iss(line);
        iss >> FCB[i].isused >> FCB[i].name >> FCB[i].type >> FCB[i].address >> FCB[i].dir >> FCB[i].lock >> FCB[i].line;
        for (int k = 0; k < FCB[i].line; k++)
        {
            getline(file, line);
            FCB[i].content[k] = line;
        }
    }
    // 调试输出
    //  for (int j = 0; j < MAX_FCB_COUNT; j++)
    //  {
    //      cout << FCB[j].isused << endl;
    //      cout << FCB[j].name << endl;
    //      cout << FCB[j].type << endl;
    //      cout << FCB[j].address << endl;
    //      cout << FCB[j].dir << endl;
    //      cout<<FCB[j].lock<<endl;
    //      cout<<FCB[j].line<<endl;
    //      for(int k=0;k<FCB[j].line;k++)
    //      {
    //         cout<<FCB[j].content[k]<<endl;
    //      }
    //  }
    file.close();
}

void create(const string filename, const string dirent, int no)
{
    open_file_system();
    // 在每个用户分配的文件系统中查找空闲的FCB
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].isused == "1" && FCB[i].name == filename && FCB[i].dir == dirent)
        {
            cout << filename << " is already exit" << endl;
            return;
        }
        if (FCB[i].isused == "0")
            break;
    }
    if (i == no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT)
    {
        // 文件控制块已经用完
        cout << "Memory is full!" << endl;
        return;
    }
    // 找到编号为i的FCB为空，则对文件控制块进行编辑
    FCB[i].isused = "1";
    FCB[i].name = filename;
    FCB[i].type = "0";
    FCB[i].address = i;
    FCB[i].dir = dirent;
    FCB[i].lock = 0;
    // cout << FCB[i].isused << endl;
    // 将做出的更改写入文件中
    write_to_disk();
    cout << "File " << filename << " was successfully created!" << endl;
}

void Delete(const string current_dirent, const string filename, int no)
{
    // 打开文件系统读取相关信息
    open_file_system();
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].name == filename && FCB[i].dir == current_dirent)
        {
            // cout << i << endl;
            break;
        }
    }
    // 确认用户选择
    cout << "Are you sure you want to delete the file?[Y/N]" << endl;
    string select;
    cin >> select;
    if (select == "Y" || select == "y")
    {
        // 清除FCB相关信息
        FCB[i].isused = "0";
        FCB[i].name = "null";
        FCB[i].type = "-1";
        FCB[i].address = -1;
        FCB[i].dir = "null_dir";
        FCB[i].content[0] = "null_content";
    }
    // if(select=="N"||select="n")
    else
    {
        return;
    }
    write_to_disk();
    cout << filename << " is already deleted!" << endl;
}

int open(const string current_dirent, const string filename, int no)
{
    open_file_system();
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        // 查找到文件并且文件没有被锁
        if (FCB[i].name == filename && FCB[i].dir == current_dirent && FCB[i].type == "0")
        {
            break;
        }
    }
    // if (FCB[i].type == "1")
    // {
    //     cout << "Can not open dirent as file!" << endl;
    //     return -1;
    // }
    if (FCB[i].lock == 1)
    {
        cout << "Permission denined! File was locked!" << endl;
        return -1;
    }
    else if (i == no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT)
    {
        cout << "File is not exit!" << endl;
        return -1;
    }
    else
    {
        cout << "File " << filename << " is already open, file was saved in FCB" << "[" << i << "]." << endl;
        return i;
    }
}

int get_length(int pos)
{
    int length = 0;
    for (int i = 0; i < FCB[pos].line; i++)
    {
        length += FCB[pos].content[i].length();
    }
    return length;
}

void read(int pos, int seek)
{
    if (seek == 0)
    {
        if (pos == -1)
        {
            cout << "Please open file first!" << endl;
            return;
        }
        // 输出打开文件的文件内容
        if (FCB[pos].content[0] == "null_content")
        {
            cout << "This file is empty!" << endl;
        }
        else
        {
            cout << "Content is here:" << endl;
            for (int i = 0; i < FCB[pos].line; i++)
            {
                cout << FCB[pos].content[i] << endl;
            }
        }
    }
    else
    {
        int length = get_length(pos);
        if (seek > length)
        {
            cout << "File is already closed!" << endl;
            cout << "Please move to the left" << endl;
            return;
        }
        else
        {
            int i = 0;
            for (; i < FCB[pos].line; i++)
            {
                seek -= FCB[pos].content[i].length();
                if (seek <= 0)
                    break;
            }
            // 从第i行的指定位置输出相关信息
            seek = FCB[pos].content[i].length() + seek;
            // 先输出不完整的一行
            cout << "Content here:" << endl;
            size_t sta_pos = (size_t)seek;
            string substr = FCB[pos].content[i].substr(sta_pos);
            cout << substr << endl;
            for (int j = i + 1; j < FCB[pos].line; j++)
            {
                cout << FCB[pos].content[j] << endl;
            }
        }
    }
}

void write(int pos, int seek)
{
    if (seek == 0)
    {
        if (pos == -1)
        {
            cout << "Please open file first!" << endl;
            return;
        }
        string temp_content;
        string select;
        cout << "Please select module:[-a/-m]" << endl;
        getline(cin, select);
        // 追加
        if (select == "-a")
        {
            // 文件一开始为空
            cout << "Write content here: " << endl;
            if (FCB[pos].content[0] == "null_content")
            {
                while (temp_content != "-s")
                {
                    getline(cin, temp_content);
                    FCB[pos].content[FCB[pos].line - 1] = temp_content;
                    FCB[pos].line++;
                }
                FCB[pos].line -= 2; // 减去一开始的1和最后输入的-s的一行
                cout << FCB[pos].line << endl;
            }
            else
            {
                // 文件一开始不为空
                while (temp_content != "-s")
                {
                    getline(cin, temp_content);
                    FCB[pos].content[FCB[pos].line] = temp_content;
                    FCB[pos].line++;
                }
                FCB[pos].line -= 1; //
                cout << FCB[pos].line << endl;
            }
        }
        else if (select == "-m")
        {
            // 覆盖
            cout << "Are you sure to cover it![Y/N]" << endl;
            string select1;
            getline(cin, select1);
            if (select1 == "y" || select1 == "y")
            {
                cout << "Write content here:" << endl;
                if (FCB[pos].content[0] == "null_content")
                {
                    while (temp_content != "-s")
                    {
                        getline(cin, temp_content);
                        FCB[pos].content[FCB[pos].line - 1] = temp_content;
                        FCB[pos].line++;
                    }
                    FCB[pos].line -= 2; // 减去一开始的1和最后输入的-s的一行
                    cout << FCB[pos].line << endl;
                }
                else
                {
                    // 文件一开始不为空,先将文件内容清空
                    FCB[pos].line = 1;
                    FCB[pos].content[0] = "null_content";
                    while (temp_content != "-s")
                    {
                        getline(cin, temp_content);
                        FCB[pos].content[FCB[pos].line] = temp_content;
                        FCB[pos].line++;
                    }
                    // cout<<FCB[pos].line<<endl;
                }
            }
        }
    }
    else
    {
        int length = get_length(pos);
        if (length > seek)
        {
            cout << "Write here:" << endl;
            string insert_line;
            getline(cin, insert_line);
            int i = 0;
            for (; i < FCB[pos].line; i++)
            {
                seek -= FCB[pos].content[i].length();
                if (seek <= 0)
                    break;
            }
            // 在第i行的指定位置插入相关信息
            seek = FCB[pos].content[i].length() + seek;
            FCB[pos].content[i].insert(seek, insert_line);
            cout << "Insert successfully!" << endl;
        }
        else
        {
            cout << "Seek was longer than file content" << endl;
            cout << "Please move to the left" << endl;
            return;
        }
    }
    write_to_disk();
}

int close()
{
    cout << "File is already closed!" << endl;
    return -1;
}

void move(const string current_dirent, const string filename, const string path, int no)
{
    open_file_system();
    // 确保要移动的目录下没有与文件同名的文件
    int a = no * EVERY_USER_FCB_COUNT;
    for (; a < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; a++)
    {
        if (FCB[a].name == filename && FCB[a].dir == path)
        {
            cout << filename << " was already exit!" << endl;
            return;
            break;
        }
    }
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].name == filename && FCB[i].dir == current_dirent)
        {
            // 查找到相关文件
            break;
        }
    }
    int j = no * EVERY_USER_FCB_COUNT;
    for (; j < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; j++)
    {
        if (FCB[j].type == "1" && FCB[j].name == path)
            break;
    }
    // 找不到指定路径
    if (j == no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT)
    {
        cout << "No such directory!" << endl;
    }
    // 找不到指定文件
    if (i == no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT)
    {
        cout << "No such file!" << endl;
    }
    else
    {
        FCB[i].dir = path;
    }
    write_to_disk();
}

void copy(const string current_dirent, const string filename1, const string filename2, int no)
{
    // 用来标记是否找到两个文件
    int flag = 0;
    int file1_num, file2_num;
    open_file_system();
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].name == filename1 && FCB[i].type == "0" && FCB[i].dir == current_dirent)
        {
            // 获取到文件的FCB编号
            file1_num = i;
            flag++;
        }
        else if (FCB[i].name == filename2 && FCB[i].type == "0" && FCB[i].dir == current_dirent)
        {
            // 获取到文件的FCB编号
            file2_num = i;
            flag++;
        }
        if (flag == 2)
        {
            // 表示找到了两个文件
            break;
        }
    }
    if (i == no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT)
    {
        cout << "No such file!" << endl;
    }
    else
    {
        // 将filename1的内容拷贝到filename2中
        FCB[file2_num].lock = FCB[file1_num].lock;
        FCB[file2_num].line = FCB[file1_num].line;
        for (int j = 0; j < FCB[file1_num].line; j++)
        {
            // 拷贝文件内容
            FCB[file2_num].content[j] = FCB[file1_num].content[j];
        }
        write_to_disk();
        cout << "File is successfully copyed!" << endl;
    }
}

void flock(const string current_dirent, const string filename, int no)
{
    open_file_system();
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].name == filename && FCB[i].dir == current_dirent)  
        {
            break;
        }
    }
    if (i == EVERY_USER_FCB_COUNT * no + EVERY_USER_FCB_COUNT)
    {
        cout << "No such file" << endl;
        return;
    }
    cout << "Select lock or unlock[L/U]:";
    string select;
    getline(cin, select);
    if (select == "L" || select == "l")
    {
        FCB[i].lock = 1;
    }
    else
    {
        FCB[i].lock = 0;
    }
    write_to_disk();
}

void head_num(const string current_dirent, int num, const string filename, int no)
{
    open_file_system();
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].name == filename && FCB[i].dir == current_dirent)
        {
            cout << "debug" << i << endl;
            // 查找到当前目录的当前文件
            break;
        }
    }
    if (FCB[i].lock == 1)
    {
        cout << "File was locked!" << endl;
    }
    else
    {
        for (int j = 0; j < num; j++)
        {
            cout << FCB[i].content[j] << endl;
        }
    }
}

void tail_num(const string current_dirent, int num, const string filename, int no)
{
    open_file_system();
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].name == filename && FCB[i].dir == current_dirent)
        {
            // 查找到当前目录的当前文件
            break;
        }
    }
    if (FCB[i].lock == 1)
    {
        cout << "File was locked!" << endl;
    }
    else
    {
        for (int j = FCB[i].line - num; j < FCB[i].line; j++)
        {
            cout << FCB[i].content[j] << endl;
        }
    }
}

void import(const string path, const string current_dirent, int no)
{
    // 获取不带路径的文件名
    string::size_type iPos = path.find_last_of('\\') + 1;
    string filename = path.substr(iPos, path.length() - iPos);
    // 创建相关文件
    create(filename, current_dirent, no);
    int num = open(current_dirent, filename, no);
    // 打开指定目录下的文件并将文件内容写入当前FCB中
    ifstream file;
    file.open(path);
    if (!file)
    {
        perror("error");
    }
    else
    {
        int count = 0;
        string file_line;
        while (getline(file, file_line))
        {
            // cout << file_line << endl;
            // 将文件内容写入FCB中
            FCB[num].content[count++] = file_line;
            FCB[num].line++;
        }
        FCB[num].line -= 1;
    }
    write_to_disk();
    close(num);
}

void Export(const string path, const string current_dirent, const string filename, int no)
{
    // 找到指定文件
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < EVERY_USER_FCB_COUNT * no + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].name == filename && FCB[i].type == "0" && FCB[i].dir == current_dirent)
        {
            break;
        }
    }
    if (i == EVERY_USER_FCB_COUNT * no + EVERY_USER_FCB_COUNT)
    {
        cout << "No such file!" << endl;
    }
    else
    {
        // 找到了相关文件,将路径跟文件名称合并
        string ult_path;
        ult_path = path + filename;
        // 在路径下打开文件，若不存在则打开
        ofstream file(ult_path);
        if (file.is_open())
        {
            // 若在指定的目录下打开了文件则说明文件已经存在
            for (int j = 0; j < FCB[i].line; j++)
            {
                file << FCB[i].content[j] << endl;
            }
        }
        else if (!file.is_open())
        {
            perror("error");
        }
    }
}

/******************************************************************************************************************
******************************************************************************************************************
******************************************************************************************************************/

string cd(const string current_dirent, const string dest_dirent, int no)
{
    string result;
    // 返回最终要改变的目录
    if (dest_dirent == "..")
    {
        // 获取到当前目录的父目录
        open_file_system();
        int i = no * EVERY_USER_FCB_COUNT;
        for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
        {
            if (FCB[i].name == current_dirent && FCB[i].type == "1")
            {
                // 查找到路径,返回父目录
                // cout<<"Debug info:"<<FCB[i].name<<" "<<FCB[i].dir<<endl;
                result = FCB[i].dir;
            }
        }
    }
    else
    {
        // 在文件系统目录中查找相关目录
        int j = no * EVERY_USER_FCB_COUNT;
        for (; j < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; j++)
        {
            if (FCB[j].name == dest_dirent && FCB[j].type == "1")
            {
                // cout<<"Debug info:"<<FCB[j].name<<" "<<FCB[j].dir<<endl;
                result = FCB[j].name;
                break;
            }
        }
        if (j == EVERY_USER_FCB_COUNT * no + EVERY_USER_FCB_COUNT)
        {
            result = "null";
        }
    }
    return result;
}

void mkdir(const string direntname, const string current_path, int no)
{
    // 打开虚拟磁盘目录
    open_file_system();
    // 获取空白FCB
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].isused == "1" && FCB[i].name == direntname && FCB[i].dir == current_path)
        {
            cout << "Directory is already exit!" << endl;
            return;
        }
        if (FCB[i].isused == "0")
        {
            break;
        }
    }
    // 查找到空的文件控制块
    FCB[i].isused = "1";
    FCB[i].name = direntname;
    FCB[i].type = "1"; // 写入文件类型为目录类型
    FCB[i].address = i;
    FCB[i].dir = current_path; // 设置父目录
    FCB[i].lock = 0;
    FCB[i].line = 1; // 目录文件不用存储内容
    cout << "Creat dirent successfully!" << endl;
    write_to_disk();
}

void dir(const string path, int no)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // 显示当前目录的所有内容
    open_file_system();
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].isused == "1" && FCB[i].dir == path && FCB[i].type == "1")
        {
            // 设置文件字体为绿色
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            cout << FCB[i].name << " ";
        }
        else if (FCB[i].isused == "1" && FCB[i].dir == path && FCB[i].type == "0")
        {
            // 设置文件字体为默认
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            cout << FCB[i].name << " ";
        }
    }
    cout << endl;
    // 结束函数之前更改字体为默认颜色
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void rmdir(const string current_dirent, const string dirent, int no)
{
    int current_dir_num;
    // 确保删除的目录在当前目录下
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < EVERY_USER_FCB_COUNT * no + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].name == dirent && FCB[i].type == "1" && FCB[i].dir == current_dirent)
        {
            cout << "Debug info " << FCB[i].name << endl;
            current_dir_num = i;
            int j = no * EVERY_USER_FCB_COUNT;
            for (; j < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; j++)
            {
                // 若在要删除的文件夹之下还存在文件夹则进行递归操作
                if (FCB[j].type == "1" && FCB[j].dir == dirent)
                {
                    cout << "debug info: " << FCB[j].name << " " << FCB[j].type << " " << FCB[j].dir << endl;
                    rmdir(FCB[j].dir, FCB[j].name, no);
                }
                else if (FCB[j].type == "0" && FCB[j].dir == dirent)
                {
                    // 将文件控制块中的数据清空
                    FCB[j].isused = "0";
                    FCB[j].name = "null";
                    FCB[j].type = "-1";
                    FCB[j].address = -1;
                    FCB[j].dir = "null_dir";
                    FCB[j].lock = -1;
                    FCB[j].line = 1;
                    FCB[j].content[0] = "null_content";
                }
            }
        }
    }
    // 将当前文件夹相关信息清空
    FCB[current_dir_num].isused = "0";
    FCB[current_dir_num].name = "null";
    FCB[current_dir_num].type = "-1";
    FCB[current_dir_num].address = -1;
    FCB[current_dir_num].dir = "null_dir";
    FCB[current_dir_num].lock = -1;
    FCB[current_dir_num].line = 1;
    FCB[current_dir_num].content[0] = "null_content";
}
