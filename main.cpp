#include <time.h>
#include "file.h"
#include <windows.h>
#define MAX_USR_COUNT 3 // ����û�����Ϊ10
using namespace std;

/*
�����е��ļ�˵����
    disk.txt����ģ��������̣����д洢���ļ�ϵͳ���ļ����ƿ�
    usr_pwd�ļ����ڴ洢�Ѿ�ע����û���������
    main.cpp�ļ�Ϊϵͳ�����
    dirent.h�ļ���Ҫ���ڱ����ļ�Ŀ¼����Ϣ
    file.h�ļ����ڱ����ļ��������Ϣ
*/

int seek = 0;          // ���ڱ����ļ�ָ��ƫ����
string current_dirent; // ȫ�ֱ��������洢��ǰ����Ŀ¼
int file_pos;          // ȫ�ֱ��������洢���ļ����FCB���

typedef struct usr_pwd
{
    // ���ڱ����û���������
    string name;
    string password;
    int no; // ���ڴ洢�û����
} usr_pwd;

usr_pwd login()
{
    usr_pwd currentname; // ��ǰ�û�
    // ע�ᡢ��¼
    int count = 0;
    usr_pwd user[MAX_USR_COUNT]; // �����û��������Ӧ�Ķ���
    // �򿪴洢�˺�������ļ�
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
                // ���ʧ�����ȡ��һ��
                continue;
            }
            // cout << user[count].name << " " << user[count].password << " " << user[count].no << endl;
            count++;
        }
    }
    file.close();
    // ѡ���¼����ע��
    cout << "Please select Login (1) or Register an Account (2):" << endl;
    int select;
    cin >> select;
    if (select == 1)
    {
        // �û���¼
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
            // ȷ���˺ź������Ƿ���ȷ
            if (user[i].password == pwd)
            {
                flag1 = 0; // ��ֹѭ��
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
        // ע���û�
        if (count > MAX_USR_COUNT - 1)
        {
            // �û���������
            cout << "The current number of users has exceeded the maximum!" << endl;
        }
        else
        {
            int flag2 = 1;
            string name; // �û���
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
            // ��ע����û����Լ����뱣�浽�ļ���
            user[count].name = name;
            user[count].password = pwd;
            user[count].no = count;
            currentname = user[count];
            // ���������û�׷�ӵ��ļ���β�������������disk.txt�з�����ؿռ�
            // ��usr_pwd�ļ���׷������
            ofstream file;
            file.open("usr_pwd.txt", ios::app);
            file << user[count].name << " " << user[count].password << " " << user[count].no << endl;
            file.close();
            count++; // �����û�����
        }
    }
    return currentname; // ���ص�ǰ�û�
}

int Space_count(string str)
{
    // �����鿴�����ָ���еĿո�����
    int space_count = 0;
    // �����ַ���
    for (char c : str)
    {
        // ����ַ��Ƿ��ǿո�
        if (c == ' ')
        {
            space_count++; // ����ǿո����Ӽ�����
        }
    }
    return space_count;
}

void help()
{
    // �ļ�ϵͳ�İ����ĵ�
    cout << "               �ļ�ϵͳ�����ĵ�               " << endl;
    cout << "�ļ�����" << endl;
    cout << "create   -[filename]                 �����ļ�" << endl;
    cout << "delete   -[filename]                 ɾ���ļ�" << endl;
    cout << "close    -[filename]                 �ر��ļ�" << endl;
    cout << "open     -[filename]                 ���ļ�" << endl;
    cout << "read                                   ���ļ�" << endl;
    cout << "write                                  д�ļ�" << endl;
    cout << "move     -[filename] -[path]         �ƶ��ļ�" << endl;
    cout << "copy     -[filename] -[filename]     �����ļ�" << endl;
    cout << "flock    -[filename]                 �ļ�����" << endl;
    cout << "head  -[num] -[filename]       ��ʾ�ļ�ǰnum��" << endl;
    cout << "tail  -[num] -[filename]       ��ʾ�ļ���num��" << endl;
    cout << "lseek    -[filename]          �ļ���дָ���ƶ�" << endl;
    cout << "Ŀ¼����" << endl;
    cout << "cd       -[path]                  �ı乤��Ŀ¼" << endl;
    cout << "dir                             �г�Ŀ¼���ļ�" << endl;
    cout << "mkdir    -[direntname]                ����Ŀ¼" << endl;
    cout << "rmdir    -[direntname]                ɾ��Ŀ¼" << endl;
    cout << "import   -[path]             �����ļ�����ǰĿ¼" << endl;
    cout << "export -[filename] -[path]   �����ļ���ָ��Ŀ¼" << endl;
}

// �������������
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
        system("cls"); // ���֮ǰ�ĵ�¼��Ϣ
    }
    system("cls");
    current_dirent = temp.name;                          // ���õ�ǰĿ¼Ϊ�û���¼����ָ���Ŀ¼
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������뻺�����еĻ��з�����ֹ������� FileSystem@job:~$
    while (1)
    {
        // current_dirent="root";
        if (current_dirent == temp.name)
            cout << temp.name << "@FileOS" << ":~$ ";
        else
            cout << temp.name << "@FileOS:~/" << current_dirent << "$ ";
        string code;
        getline(cin, code);
        // ���������Ϣ
        //  int space=Space_count(code);
        //  cout<<space<<endl;
        //  cout<<endl;
        //  cout<<code.find("delete")<<endl;

        if (code == "clear")
        {
            system("cls"); // ���֮ǰ�������Ϣ
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
            create(filename, current_dirent, temp.no); // ����create���������ļ�
        }
        else if (code.substr(0, 7) == "delete " && (code.find("delete") != string::npos) && Space_count(code) == 1)
        {
            string key, filename;
            // cout << filename << endl;
            istringstream iss(code);
            iss >> key >> filename;
            Delete(current_dirent, filename, temp.no); // ����delete������ɾ���ļ�
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
                file_pos = open(current_dirent, filename, temp.no); // ����open���������ļ�
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
            seek = 0; // �����ļ�ָ��ƫ����
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
                // ��û�ҵ�ָ��·��ʱ
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