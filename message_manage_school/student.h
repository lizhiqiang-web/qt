#ifndef STUDENT_H
#define STUDENT_H

#pragma once
#include "people.h"
#include <QString>
class Student : public People
{
public:
    Student(
    QString dorm = NULL,
    QString stu_id = NULL,
    QString depart = NULL,
    QString class_num = NULL):People(),
        _stu_id(stu_id),
        _class_num(class_num),
        _dorm(dorm),
        _depart(depart){}

    QString dorm(void);
    QString stu_id(void);
    QString depart(void);
    QString class_num(void);
private:
    QString _dorm;
    QString _stu_id;
    QString _depart;
    QString _class_num;
};

#endif // STUDENT_H
