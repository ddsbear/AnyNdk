## 【NDK系列7】c/c++文件与流操作

### C 语言的文件读写操作

头文件 : stdio.h

函数原型：`FILE * fopen(const char * path, const char * mode); `

path:  操作的文件路径

mode:模式

| 模式 | 描述                                                         |
| ---- | ------------------------------------------------------------ |
| r    | 打开一个已有的文本文件，允许读取文件。                       |
| w    | 打开一个文本文件，允许写入文件。如果文件不存在，则会创建一个新文件。在这里，您的程序会从文件的开头写入内容。如果文件存在，则该会被截断为零长度，重新写入。 |
| a    | 打开一个文本文件，以追加模式写入文件。如果文件不存在，则会创建一个新文件。在这里，您的程序会在已有的文件内容中追加内容。 |
| r+   | 打开一个文本文件，允许读写文件。                             |
| w+   | 打开一个文本文件，允许读写文件。如果文件已存在，则文件会被截断为零长度，如果文件不存在，则会创建一个新文件。 |
| a+   | 打开一个文本文件，允许读写文件。如果文件不存在，则会创建一个新文件。读取会从文件的开头开始，写入则只能是追加模式。 |

**写入**

```c++
FILE *f = fopen("xxxx\\t.txt","w");
//写入单个字符
fputc('a', f);
fclose(f);


FILE *f = fopen("xxxx\\t.txt","w");
char *txt = "123456";
//写入以 null 结尾的字符数组
fputs(txt, f);
//格式化并输出
fprintf(f,"%s",txt);
fclose(f);
```

**读取**

```c
char buff[255];
FILE *f = fopen("xxxx\\t.txt", "r");
//读取 遇到第一个空格字符停止
fscanf(f, "%s", buff);
printf("1: %s\n", buff);

//最大读取 255-1 个字符
fgets(buff, 255, f);
printf("2: %s\n", buff);
fclose(f);
```

**二进制读写**

```c
//二进制 I/O 函数
size_t fread(void *ptr, size_t size_of_elements, 
             size_t number_of_elements, FILE *a_file);       
size_t fwrite(const void *ptr, size_t size_of_elements, 
             size_t number_of_elements, FILE *a_file);
//1、写入/读取数据缓存区
//2、每个数据项的大小
//3、多少个数据项
//4、流
//如：图片、视频等以二进制操作:
//写入buffer 有 1024个字节
fwrite(buffer,1024,1,f);
```

### C++ 文件读写操作

`<iostream\>` 和 `\<fstream\>`

| 数据类型 | 描述                                               |
| -------- | -------------------------------------------------- |
| ofstream | 输出文件流，创建文件并向文件写入信息。             |
| ifstream | 输入文件流，从文件读取信息。                       |
| fstream  | 文件流，且同时具有 ofstream 和 ifstream 两种功能。 |

```c++
char data[100];
// 以写模式打开文件
ofstream outfile;
outfile.open("XXX\\f.txt");
cout << "输入你的名字: ";
//cin 接收终端的输入
cin >> data;
// 向文件写入用户输入的数据
outfile << data << endl;
// 关闭打开的文件
outfile.close();

// 以读模式打开文件
ifstream infile;
infile.open("XXX\\f.txt");

cout << "读取文件" << endl;
infile >> data;
cout << data << endl;

// 关闭
infile.close();
```

































