## 【NDK系列4】c++基础

终于开始c++基础的整理了

@[TOC]

## 1. 输出

```c
// C使用printf向终端输出信息
// C++提供了 标准输出流 
#include <iostream>
using namespace std;

   // 1. c++输出
    int time = 8;
    cout << "dds:" << time << "点," << "hello world" << endl;

```

## 2. 函数符号兼容

C的大部分代码可以在C++中直接使用，但是仍然有需要注意的地方。

```c
//如果需要在C++中调用C实现的库中的方法
extern "C" //指示编译器这部分代码使用C的方式进行编译而不是C++
    
void func(int x, int y);
```

对于 `func` 函数 被C的编译器编译后在函数库中的名字可能为`func `(无参数符号),而C++编译器则会产生类似`funcii`之类的名字。

因此，对于C库可以

```
#ifdef __cplusplus
extern "C"{
#endif
void func(int x,int y);
#ifdef __cplusplus    
}
#endif

//__cplusplus 是由c++编译器定义的宏，用于表示当前处于c++环境
```

## 3. 引用

引用是C++定义的一种新类型，引用和指针是两个东西，这里需要分清楚

```c
//声明形参为引用
void change(int& i) {
	i = 10;
}
int i = 1;

change(i);

printf("%d\n",i); //i == 10
```

## 4. 字符串

字符串实际上是使用 NULL字符 `'\0' `终止的一维字符数组。

```c
//字符数组 = 字符串
char str1[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
//自动加入\0
char str2[] = "Hello";
```

### 字符串操作

| 函数            | 描述                                                         |
| --------------- | ------------------------------------------------------------ |
| strcpy(s1, s2); | 复制字符串 s2 到字符串 s1。                                  |
| strcat(s1, s2); | 连接字符串 s2 到字符串 s1 的末尾。                           |
| strlen(s1);     | 返回字符串 s1 的长度。                                       |
| strcmp(s1, s2); | 如果 s1 和 s2 相同，则返回 0；如果 s1 &lt; s2 则返回小于0；如果 s1&gt;s2 则返回大于0 |
| strchr(s1, ch); | 返回指向字符串 s1 中字符 ch 的第一次出现的位置的指针。       |
| strstr(s1, s2); | 返回指向字符串 s1 中字符串 s2 的第一次出现的位置的指针。     |

### C++ string类

C++ 标准库提供了 **string** 类类型，支持上述所有的操作，另外还增加了其他更多的功能。 

```c
#include <string>
//string 定义在 std命令空间中
usning namespace std;
string str1 = "Hello";
string str2 = "World";

str1.append(str2);
//获得c 风格字符串
const char *s1 = str1.c_str();
//字符串长度
str1.size();
//长度是否为0
str1.empty();

......
```



## 5. 命名空间

namespace 命名空间 相当于package

```c
namespace A{
    void a(){}
}

错误 : a();
// :: 域操作符
正确： A::a();


//当然也能够嵌套
namespace A {
	namespace B{
		void a() {};
	}
}
A::B::a();

//还能够使用using 关键字
using namespace A;
using namespace A::B;
```



## 代码

https://github.com/ddssingsong/AnyNdk





