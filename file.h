#include <bits/stdc++.h>
#include <fcntl.h>
#include <windows.h>
#include "FCB.h"
using namespace std;

/*
    ���ļ����ڱ����ļ�����ؽṹ�Լ���ز���
    create		�����ļ�
    delete		ɾ���ļ�
    open		���ļ�
    close		�ر��ļ�
    read		���ļ�
    write		д�ļ�
    move        �ƶ��ļ�
    copy  		�����ļ�
    flock       �ļ���������Ҫʵ�ּ����ͽ�������
    head �Cnum  	��ʾ�ļ���ǰnum��
    tail  -num  ��ʾ�ļ�β���ϵ�num��
    lseek       �ļ���дָ����ƶ������ļ�ָ�뵱ǰλ�ô�����ƶ� offset������ʱ��ǰ�ƶ�offset
*/

// ��ʼ���ļ�ϵͳ������MAX_FCB_COUNT���ļ����ƿ�
FCB FCB[MAX_FCB_COUNT];

void write_to_disk()
{
    // �������Ϣд��disk�����λ��
    ofstream file("disk.txt");
    if (!file)
    {
        perror("Open file failed!");
        return;
    }
    else
    {
        // ���ļ����ƿ�д���ļ���
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
    // �������
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
    // ��ÿ���û�������ļ�ϵͳ�в��ҿ��е�FCB
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
        // �ļ����ƿ��Ѿ�����
        cout << "Memory is full!" << endl;
        return;
    }
    // �ҵ����Ϊi��FCBΪ�գ�����ļ����ƿ���б༭
    FCB[i].isused = "1";
    FCB[i].name = filename;
    FCB[i].type = "0";
    FCB[i].address = i;
    FCB[i].dir = dirent;
    FCB[i].lock = 0;
    // cout << FCB[i].isused << endl;
    // �������ĸ���д���ļ���
    write_to_disk();
    cout << "File " << filename << " was successfully created!" << endl;
}

void Delete(const string current_dirent, const string filename, int no)
{
    // ���ļ�ϵͳ��ȡ�����Ϣ
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
    // ȷ���û�ѡ��
    cout << "Are you sure you want to delete the file?[Y/N]" << endl;
    string select;
    cin >> select;
    if (select == "Y" || select == "y")
    {
        // ���FCB�����Ϣ
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
        // ���ҵ��ļ������ļ�û�б���
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
        // ������ļ����ļ�����
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
            // �ӵ�i�е�ָ��λ����������Ϣ
            seek = FCB[pos].content[i].length() + seek;
            // �������������һ��
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
        // ׷��
        if (select == "-a")
        {
            // �ļ�һ��ʼΪ��
            cout << "Write content here: " << endl;
            if (FCB[pos].content[0] == "null_content")
            {
                while (temp_content != "-s")
                {
                    getline(cin, temp_content);
                    FCB[pos].content[FCB[pos].line - 1] = temp_content;
                    FCB[pos].line++;
                }
                FCB[pos].line -= 2; // ��ȥһ��ʼ��1����������-s��һ��
                cout << FCB[pos].line << endl;
            }
            else
            {
                // �ļ�һ��ʼ��Ϊ��
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
            // ����
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
                    FCB[pos].line -= 2; // ��ȥһ��ʼ��1����������-s��һ��
                    cout << FCB[pos].line << endl;
                }
                else
                {
                    // �ļ�һ��ʼ��Ϊ��,�Ƚ��ļ��������
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
            // �ڵ�i�е�ָ��λ�ò��������Ϣ
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
    // ȷ��Ҫ�ƶ���Ŀ¼��û�����ļ�ͬ�����ļ�
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
            // ���ҵ�����ļ�
            break;
        }
    }
    int j = no * EVERY_USER_FCB_COUNT;
    for (; j < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; j++)
    {
        if (FCB[j].type == "1" && FCB[j].name == path)
            break;
    }
    // �Ҳ���ָ��·��
    if (j == no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT)
    {
        cout << "No such directory!" << endl;
    }
    // �Ҳ���ָ���ļ�
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
    // ��������Ƿ��ҵ������ļ�
    int flag = 0;
    int file1_num, file2_num;
    open_file_system();
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].name == filename1 && FCB[i].type == "0" && FCB[i].dir == current_dirent)
        {
            // ��ȡ���ļ���FCB���
            file1_num = i;
            flag++;
        }
        else if (FCB[i].name == filename2 && FCB[i].type == "0" && FCB[i].dir == current_dirent)
        {
            // ��ȡ���ļ���FCB���
            file2_num = i;
            flag++;
        }
        if (flag == 2)
        {
            // ��ʾ�ҵ��������ļ�
            break;
        }
    }
    if (i == no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT)
    {
        cout << "No such file!" << endl;
    }
    else
    {
        // ��filename1�����ݿ�����filename2��
        FCB[file2_num].lock = FCB[file1_num].lock;
        FCB[file2_num].line = FCB[file1_num].line;
        for (int j = 0; j < FCB[file1_num].line; j++)
        {
            // �����ļ�����
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
            // ���ҵ���ǰĿ¼�ĵ�ǰ�ļ�
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
            // ���ҵ���ǰĿ¼�ĵ�ǰ�ļ�
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
    // ��ȡ����·�����ļ���
    string::size_type iPos = path.find_last_of('\\') + 1;
    string filename = path.substr(iPos, path.length() - iPos);
    // ��������ļ�
    create(filename, current_dirent, no);
    int num = open(current_dirent, filename, no);
    // ��ָ��Ŀ¼�µ��ļ������ļ�����д�뵱ǰFCB��
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
            // ���ļ�����д��FCB��
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
    // �ҵ�ָ���ļ�
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
        // �ҵ�������ļ�,��·�����ļ����ƺϲ�
        string ult_path;
        ult_path = path + filename;
        // ��·���´��ļ��������������
        ofstream file(ult_path);
        if (file.is_open())
        {
            // ����ָ����Ŀ¼�´����ļ���˵���ļ��Ѿ�����
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
    // ��������Ҫ�ı��Ŀ¼
    if (dest_dirent == "..")
    {
        // ��ȡ����ǰĿ¼�ĸ�Ŀ¼
        open_file_system();
        int i = no * EVERY_USER_FCB_COUNT;
        for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
        {
            if (FCB[i].name == current_dirent && FCB[i].type == "1")
            {
                // ���ҵ�·��,���ظ�Ŀ¼
                // cout<<"Debug info:"<<FCB[i].name<<" "<<FCB[i].dir<<endl;
                result = FCB[i].dir;
            }
        }
    }
    else
    {
        // ���ļ�ϵͳĿ¼�в������Ŀ¼
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
    // ���������Ŀ¼
    open_file_system();
    // ��ȡ�հ�FCB
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
    // ���ҵ��յ��ļ����ƿ�
    FCB[i].isused = "1";
    FCB[i].name = direntname;
    FCB[i].type = "1"; // д���ļ�����ΪĿ¼����
    FCB[i].address = i;
    FCB[i].dir = current_path; // ���ø�Ŀ¼
    FCB[i].lock = 0;
    FCB[i].line = 1; // Ŀ¼�ļ����ô洢����
    cout << "Creat dirent successfully!" << endl;
    write_to_disk();
}

void dir(const string path, int no)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // ��ʾ��ǰĿ¼����������
    open_file_system();
    int i = no * EVERY_USER_FCB_COUNT;
    for (; i < no * EVERY_USER_FCB_COUNT + EVERY_USER_FCB_COUNT; i++)
    {
        if (FCB[i].isused == "1" && FCB[i].dir == path && FCB[i].type == "1")
        {
            // �����ļ�����Ϊ��ɫ
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            cout << FCB[i].name << " ";
        }
        else if (FCB[i].isused == "1" && FCB[i].dir == path && FCB[i].type == "0")
        {
            // �����ļ�����ΪĬ��
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            cout << FCB[i].name << " ";
        }
    }
    cout << endl;
    // ��������֮ǰ��������ΪĬ����ɫ
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void rmdir(const string current_dirent, const string dirent, int no)
{
    int current_dir_num;
    // ȷ��ɾ����Ŀ¼�ڵ�ǰĿ¼��
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
                // ����Ҫɾ�����ļ���֮�»������ļ�������еݹ����
                if (FCB[j].type == "1" && FCB[j].dir == dirent)
                {
                    cout << "debug info: " << FCB[j].name << " " << FCB[j].type << " " << FCB[j].dir << endl;
                    rmdir(FCB[j].dir, FCB[j].name, no);
                }
                else if (FCB[j].type == "0" && FCB[j].dir == dirent)
                {
                    // ���ļ����ƿ��е��������
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
    // ����ǰ�ļ��������Ϣ���
    FCB[current_dir_num].isused = "0";
    FCB[current_dir_num].name = "null";
    FCB[current_dir_num].type = "-1";
    FCB[current_dir_num].address = -1;
    FCB[current_dir_num].dir = "null_dir";
    FCB[current_dir_num].lock = -1;
    FCB[current_dir_num].line = 1;
    FCB[current_dir_num].content[0] = "null_content";
}
