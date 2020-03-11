#ifndef MYFILE_H
#define MYFILE_H
#include <string>
#include <QString>
//#include <QString>
#include "myBlock.h"
#include "linkedList.h"

class myFile
{
    QString name;
    int size;
    linkedList <myBlock> blocks_used;
    //char* fdata;
    friend class FileSystem;
    friend class systemWindow;

public:
    myFile()
    {
        size = 0;
        //fdata = nullptr;
    }

    myFile(QString n, int s)
        :name(n), size(s)
    {
        //fdata = nullptr;
    }

    /*file(string n, int s, char* data)
        :file(n, s)
    {
        fdata = new char[strlen(data) + 1];
        strcpy(fdata, data);
    }*/

    /*void insertData(char* data)
    {
        if (fdata != nullptr)
        {
            delete[]fdata;
        }

        fdata = new char[strlen(data) + 1];
        strcpy(fdata, data);
    }*/

    QString getName()const
    {
        return name;
    }

    bool operator==(const myFile& obj)
    {
        if (name == obj.name && size == obj.size)
        {
            /*for (int i = 0; i < size; i++)
            {
                if (fdata[i] != obj.fdata[i])
                    return false;
            }*/

            return true;
        }

        return false;
    }

    ~myFile()
    {
        //delete[]fdata;
    }
};


#endif // MYFILE_H
