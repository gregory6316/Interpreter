
#pragma once
#include "Lexic.h"

using namespace std;

class Poliz
{
public:
    Lex *p;
    int size;
    int free;
public:
    Poliz(int max_size)
    {
        p = new Lex[size = max_size];
        free = 0;
    }
    
    ~Poliz() {
        delete[]p;
    }
    
    void put_lex(Lex l)
    {
        p[free] = l;
        ++free;
    }
    
    void put_lex(Lex l, int place)
    {
        p[place] = l;
    }
    
    void blank()
    {
        ++free;
    }
    
    int get_free()
    {
        return free;
    }
    
    Lex& operator[] (int index)
    {
        if (index > size) throw Exception("ПОЛИЗ: выход за пределы!");
        else
            if (index > free) throw Exception("ПОЛИЗ: неизвестный элемент!");
            else return p[index];
    }
    
    void print()
    {
        for (int i = 0; i < free; ++i)
            cout << p[i];
    }
};
