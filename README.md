# 指令使用说明
~~~
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
    cout << "inport   -[path]             导入文件到当前目录" << endl;
    cout << "export -[filename] -[path]   导出文件到指定目录" << endl;
~~~

1. create指令：其后跟文件名称，会在当前目录下创建一个指定文件名称的文件，若文件名已经存在则会输出错误信息
2. delete指令：其后跟文件名称，会删除当前目录下的指定文件，若文件不存在则会输出错误信息
3. open指令，其后跟文件名称，会打开当前目录的指定文件，并且返回文件所在的文件控制块编号
4. read指令，没有参数，在使用open函数之后使用，会将文件中的内容打印输出
5. write函数，没有参数，在使用open函数之后使用，其会让你选取操作方法，-a为在文件之后追加，-m为覆盖当前文件内容
6. close指令，没有参数，在使用open函数之后用于关闭文件打的操作，方便下次再打开文件，每次打开文件之后记得关闭文件
7. copy指令，两个参数，都是文件名称，用于将前一文件的内容拷贝到后一文件中
8. move指令，两个参数，前一参数可以是文件也可以是目录，用于前一文件移动到后一目录中
9. head指令，两个参数，前一参数是要显示的行数，后一参数为需要处理的文件，显示文件的前num行内容
10. tail指令，两个参数，前一参数是要显示的行数，后一参数为需要处理的文件，显示文件的后num行内容
11. cd指令，一个参数，其后跟目录，用于切换当前工作目录
12. dir指令，没有参数，用于显示当前目录下的所有文件
13. rmdir指令，一个参数，其后跟目录名称，用于删除目录以及目录下的所有文件
14. mkdir指令，一个参数，用于在当前目录下创建指定目录
15. inport指令，一个参数，其后跟文件路径，用于将外部指定路径下的文件导入
16. export指令，两个参数，其后跟文件名，路径，用于将当前目录下的指定文件导出到指定目录之下
17. lseek指令，用于移动文件操作指针当参数为正数时向右移动文件指针，当参数为负数时向左移动指针，需要在使用open函数之后然后才可以进行文件指针操作，注意在操作完成之后如果不在需要使用文件指针记得关闭文件





