## 【NDK系列5】c++基础  面向对象编程

## 1. 类

C++ 在 C 语言的基础上增加了面向对象编程，C++ 支持面向对象程序设计。类是 C++ 的核心特性，用户定义的类型。

```c
class Student {
	int i;    //默认 private
public:
	Student(int i,int j,int k):i(i),j(j),k(k){};	//构造方法 
	~Student(){};	//析构方法 
private:
	int j;
protected:
	int k;
};
```

**调用方法**

```c
//调用构造方法 栈
//出方法释放student 调用析构方法
Student student(1,2,3); 

//动态内存(堆)
Student *student = new Student(1,2,3);
//释放
delete student;
student = 0;
```

## 2. 常量函数 

## 3. 友元

## 4. 静态成员

```c
//Instance.h
class Instance {
public:
    static Instance* getInstance();

    void printHello();
private:
    static Instance *instance;
};


//Instance.cpp
#include "Instance.h"
Instance* Instance::instance = 0;
Instance* Instance::getInstance() {
	//C++11以后，编译器保证内部静态变量的线程安全性
	if (!instance) {
		instance = new Instance;
	}
	return instance;
}


```

## 5. 重载函数





## 6. 继承



## 7. 多态

多种形态。当类之间存在层次结构，并且类之间是通过继承关联时，就会用到多态。

静态多态（静态联编）是指在编译期间就可以确定函数的调用地址，通过**函数重载**和**模版（泛型编程）**实现 

动态多态（动态联编）是指函数调用的地址不能在编译器期间确定，必须需要在运行时才确定 ,通过**继承+虚函数** 实现

### 虚函数

### 



## 8. 模板

模板是泛型编程的基础

### 函数模板

### 类模板(泛型类)

```c
template<class E,class T>
class Queue {
public:
    T add(E e,T t){
        return e+t;
    }
};

Queue<int,float> q;
q.add(1,1.1f) = 2.1f
```































## 代码

https://github.com/ddssingsong/AnyNdk





