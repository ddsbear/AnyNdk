## 【NDK系列】c++ 智能指针 



自C++11起，C++标准库提供了两大类型的智能指针

## 1. shared_ptr

操作引用计数实现共享式拥有的概念。多个智能指针可以指向相同的对象，这个对象和其相关资源会在最后一个被销毁时释放。

```c
class A {
public:
	~A() {
		cout << "释放A" << endl;
	}
};

void test() {
	//自动释放 引用计数为1
	shared_ptr<A> a(new A());
    //退出方法 shared_ptr a本身释放，对内部的 A 对象引用计数减1 则为0 释放new 出来的A 对象 
}
```

虽然使用shared_ptr能够非常方便的为我们自动释放对象，但是还是会出现一些问题。最典型的就是循环引用问题。

```c
class B;
class A {
public:
	~A() {
		cout << "释放A" << endl;
	}
	shared_ptr<B> b;
};

class B {
public:
	~B() {
		cout << "释放B" << endl;
	}
	shared_ptr<A> a;
};
void test() {
	//自动释放
	shared_ptr<A> a(new A()); //A引用计数为1
	shared_ptr<B> b(new B()); //B引用计数为1
    cout << a.use_count() << endl; //查看内部对象引用计数
	a->b = b;			//A 引用计数为2
	b->a = a;			//B 引用计数为2
	//退出方法，a释放，A引用计数-1结果为1 不会释放 B也一样
}
```

## 2. weak_ptr

weak_ptr是为配合shared_ptr而引入的一种智能指针。主要用于观测资源的引用情况。

它的构造和析构不会引起引用记数的增加或减少。没有重载*和->但可以使用lock获得一个可用的shared_ptr对象。

weak_ptr 提供expired 方法等价于 use_count == 0，当expired为true时，lock返回一个存储空指针的shared_ptr 

配合shared_ptr解决循环引用问题



```c
class B;
class A {
public:
	~A() {
		cout << "释放A" << endl;
	}
	weak_ptr<B> b;
};
class B {
public:
	~B() {
		cout << "释放B" << endl;
	}
	weak_ptr<A> a;
};

void test() {
	//自动释放
	shared_ptr<A> a(new A()); //A引用计数为1
	shared_ptr<B> b(new B()); //B引用计数为1

	a->b = b;			//weak_ptr 引用计数不增加
	b->a = a;			//weak_ptr 引用计数不增加
	//退出方法，A B释放
}
```

## 3. unique_ptr

实现独占式引用，保证同一时间只有一个智能指针指向内部对象。

```c
unique_ptr<A> a(new A());
```

## 4. 自定义智能指针

```c
template <typename T>
class Ptr {
public:
	Ptr() {
		count = new int(1);
		t = 0;
	}
	Ptr(T *t):t(t) {
		//引用计数为1
		count = new int(1);
	}
	~Ptr() {
		//引用计数-1 为0表示可以释放T了
		if (--(*count) == 0)
		{
			if (t) {
				delete t;
			}
			delete count;
			t = 0;
			count = 0;
		}
	}
	//拷贝构造函数
	Ptr(const Ptr<T> &p) {
		//引用计数+1
		++(*p.count);
		t = p.t;
		count = p.count;
	}

	Ptr<T>& operator=(const Ptr<T>& p) {
		++(*p.count);
		//检查老的数据是否需要删除
		if (--(*count) == 0) {
			if (t) {
				delete t;
			}
			delete count;
		}
		t = p.t;
		count = p.count;
		return *this;
	}
	//重载-> 操作T 类
	T* operator->() { return t; }

private:
	T *t;
	int *count;
};
```























