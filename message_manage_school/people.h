#ifndef PEOPLE_H
#define PEOPLE_H
#include <QString>

class People{

private:

    QString _name ;
    int _age ;
    QString _sex ;
    QString _id ;
    QString _homeaddress ;
public:
    People(
    QString name = NULL,
    int age = 0,
    QString sex = "男",
    QString id = NULL,
    QString homeaddress = NULL
    ):_name(name), _age(age), _sex(sex), _id(id), _homeaddress(homeaddress){}
    QString name(void)
    {
        return _name;
    }
    int age(void)
    {
        return _age;
    }
    QString sex(void)
    {
        return _sex;
    }
    QString id(void)
    {
        return _id;
    }
    QString homeDdress(void)
    {
        return _homeaddress;
    }
    void setName(QString name)
    {
        _name = name;
    }
    void setAge(int age)
    {
        if(age >= 0)
        {
           _age = age;
        }
    }
    void setSex(QString sex)
    {
         _sex = sex;
    }
    void setId(QString id)
    {
        _id = id;
    }
    void setHomeaddress(QString homeaddress)
    {
         _homeaddress = homeaddress;
    }
};



#endif // PEOPLE_H
