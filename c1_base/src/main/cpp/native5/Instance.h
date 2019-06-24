//
// Created by dds on 2019/6/22.
//

#ifndef ANYNDK_INSTANCE_H
#define ANYNDK_INSTANCE_H


class Instance {
public:
    static Instance* getInstance();

    void printHello();
private:
    static Instance *instance;
};


#endif //ANYNDK_INSTANCE_H
