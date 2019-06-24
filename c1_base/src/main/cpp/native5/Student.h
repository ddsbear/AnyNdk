//
// Created by dds on 2019/6/22.
//

#ifndef ANYNDK_STUDENT_H
#define ANYNDK_STUDENT_H


class Student {
    int i;    //默认 private
public:
    Student(int i, int j, int k) : i(i), j(j), k(k) {};    //构造方法
    ~Student() {};    //析构方法

    // 常量函数
    void  setName(char* _name) const  {
        //错误 不能修改name 去掉const之后可以
       // name = _name;
    }
private:
    int j;
protected:
    int k;
};


#endif //ANYNDK_STUDENT_H
