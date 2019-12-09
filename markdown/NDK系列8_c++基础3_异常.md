## 【NDK系列】c++ 异常

```c
try {
	test1();
}
catch (const char *m) {
	cout << m << endl;
}
```



```c
//自定义
class MyException : public exception
{
public:
   virtual char const* what() const
    {
        return "myexception";
    }
};
```

