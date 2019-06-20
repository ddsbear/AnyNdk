作为一个Android开发者，怎能不会NDK开发，下面我们开始我们的NDK之旅

怎么搭建环境我们暂且不管，随心而为

## 基本数据类型

1.**signed**---有符号，可修饰 char、int。Int是默认有符号的。
2.**unsigned**---无符号，可修饰 int 、char

| 整型           | 字节 | 取值范围                        | 占位 |
| :------------- | ---- | ------------------------------- | ---- |
| int            | 4    | -2,147,483,648 到 2,147,483,647 | %d   |
| unsigned int   | 4    | 0 到 4,294,967,295              | %u   |
| short          | 2    | -32,768 到 32,767               | %hd  |
| unsigned short | 2    | 0 到 65,535                     | %hu  |
| long           | 4    | -2,147,483,648 到 2,147,483,647 | %ld  |
| unsigned long  | 4    | 0 到 4,294,967,295              | %lu  |
| char           | 1    | -128 到 127                     | %c   |
| unsigned char  | 1    | 0 到 255                        | %c   |

为了得到某个类型或某个变量在特定平台上的准确大小，使用 sizeof 运算符。

表达式 sizeof(type) 得到对象或类型的存储字节大小。

long int 其实就是长整型 = long 可以省去int

在标准中,规定 int至少和short一样长，long至少和int一样长。

**问题探究：**

**为什么会存在long?**

long和int在早期16位电脑时候 int 2字节，long 4字节，而计算机发展到现在，一般32、64下，long和int一样。和java类比的话，java的long就是 long long 8字节



**格式化显示**

| 浮点型      | 字节 | 精度     | 占位 |
| ----------- | ---- | -------- | ---- |
| float       | 4    | 6位小数  | %f   |
| double      | 8    | 15位小数 | %lf  |
| long double | 8    | 19位小数 | %Lf  |

8进制 			%o
16进制			小写： %x    大写：%X
(0x)+16进制前面 	%#x 

**其他一些东西**

C99标准以前，C语言里面是没有bool，C++里面才有，
C99标准里面定义了bool类型，需要引入头文件stdbool.h
bool类型有只有两个值：true =1 、false=0。
因此实际上bool就是一个int
所以在c/c++中 if 遵循一个规则， 非0为true，非空为true；
NULL 其实也就是被define为了 0 



## 格式化

printf、sprintf等

将格式化的数据写入第一个参数

```c
char str[100];
sprintf(str, "img/png_%d.png", 1);
printf("%s", str);

//使用 0 补到3个字符
sprintf(str, "img/png_%03d.png", 1);
printf("%s", str);
```



## 数组与内存布局

数组  ： 连续的内存

```c
//java
int[] a

//c
//必须声明时候确定大小
int a[10]  
//或者 直接初始化 
int a[] = {1,2,3}

//大小
printf("%d",sizeof(a)/sizeof(int));
```

栈内存限制  linux：ulimit -a 查看
但是直接分配这么大不行，因为堆栈可能保存参数，返回地址等等信息

**动态内存申请**

**malloc** 
	没有初始化内存的内容,一般调用函数memset来初始化这部分的内存空间.

**calloc**
	申请内存并将初始化内存数据为NULL.

```c
int pn = (int)calloc(10, sizeof(int));
```

**realloc**

​	 对malloc申请的内存进行大小的调整.

```c
char *a = (char*)malloc(10);
realloc(a,20);
```

特别的：
**alloca**
	在栈申请内存,因此无需释放.

```c
int *p = (int *)alloca(sizeof(int) * 10);
```

**物理内存**
	物理内存指通过物理内存条而获得的内存空间

**虚拟内存**
	一种内存管理技术
	电脑中所运行的程序均需经由内存执行，若执行的程序占用内存很大，则会导致内存消耗殆尽。
	虚拟内存技术还会匀出一部分硬盘空间来充当内存使用。

![内存布局](art/内存布局.png)



进程分配内存主要由两个系统调用完成：**brk和mmap** 。

1. brk是将_edata(指带堆位置的指针)往高地址推；
2. mmap 找一块空闲的虚拟内存。



通过glibc (C标准库)中提供的malloc函数完成内存申请

malloc小于128k的内存，使用brk分配内存，将_edata往高地址推,大于128k则使用mmap

存放程序执行代码（cpu要执行的指令）

> 栈是向低地址扩展数据结构
> 堆是向高地址扩展数据结构



## 代码
https://github.com/ddssingsong/AnyNdk

















