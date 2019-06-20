## 【NDK系列3】结构体和共用体

感觉c这块的内容真的无比枯燥，但在整理的过程中确实又学会了很多



## 1、结构体

### 声明和使用

结构体是C编程中一种用户自定义的数据类型，类似于Java的JavaBean

```c
// 一般定义变量 student a 
struct Student
{
	char name[50];
	int age;
} student,a;


//使用typedef定义
typedef struct{
    char name[50];
    int age;
} Student;
```

当结构体需要内存过大，使用动态内存申请。结构体占用字节数和结构体内字段有关，指针占用内存就是4/8字节，因此传指针比传值效率更高。

```c
Student *s = (Student*)malloc(sizeof(Student));
memset(s,0,sizeof(Student));

printf("%d\n", s->age);
```

### 字节对齐

内存空间按照字节划分，理论上可以从任何起始地址访问任意类型的变量。但实际中在访问特定类型变量时经常在特定的内存地址开始访问，这就需要各种类型数据按照一定的规则在空间上排列，而不是顺序一个接一个地存放，这就是对齐。 

字节对齐的问题主要就是针对结构体。 

```c
struct Struct1
{
	short a;  
	int b;	
	short c; 
};
struct Struct2
{
    short a;
	short c; 
	int b;
};


// short = 2  补 2
// int = 4
// short = 2  补 2
sizeof(Struct1) = 12
    
// 2个short在一起组成一个 4 
// int = 4
sizeof(Struct2) = 8
```

**指定对齐位数**

`#pragma pack(n)`的意思是告诉编译器字节对齐方式为n字节对齐，n字节对齐就是说变量存放的起始地址的偏移量有两种情况：第一、如果n大于等于该变量所占用的字节数，那么偏移量必须满足默认的对齐方式，第二、如果n小于该变量的类型所占用的字节数，那么偏移量为n的倍数，不用满足默认的对齐方式。结构的总大小也有个约束条件，分下面两种情况：如果n大于所有成员变量类型所占用的字节数，那么结构的总大小必须为占用空间最大的变量占用的空间数的倍数；否则必须为n的倍数。

```c
#pragma pack(2) //指定以2字节对齐
struct MyStruct1
{
	short a;  
	int b;	
	short c; 
};
#pragma pack()	//取消对齐
//short = 2
//int = 4      用2的倍数进行对齐
//short = 2
```



## 2、共用体

在相同的内存位置存储不同的数据类型

共用体占用的内存应足够存储共用体中最大的成员

```c
//占用4字节
union Data
{
	int i;
	short j;
}
union Data data;
data.i = 1;        1
//i的数据损坏
data.j = 1.1f;     1 
```





























