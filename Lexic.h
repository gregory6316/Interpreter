
#pragma once


#include <iostream>
#include <fstream>
#include <string.h>


using namespace std;
ifstream fin;

class Exception
{
private:
    char * s;
    int strnum, posnum;
public:
    Exception (char * str,  int st = 0, int pos = 0)
    {
        s = str;
        strnum = st;
        posnum = pos;
    }
    void error(){
        printf ("%s\n", s);
        printf ("%d %d\n", strnum, posnum);
    }
    
};


enum type_of_lex
{
    LEX_NULL, 
    LEX_AND, 
    LEX_BOOL, 
    LEX_DO, 
    LEX_ELSE, 
    LEX_IF, 
    LEX_FALSE, 
    LEX_INT, 
    LEX_NOT,  
    LEX_OR,  
    LEX_PROGRAM,  
    LEX_IN, 
    LEX_TRUE,  
    LEX_WHILE,  
    LEX_OUT,  
    LEX_SEMICOLON,  
    LEX_COMMA,  
    LEX_COLON,  
    LEX_ASSIGN,  
    LEX_LPAREN, 
    LEX_RPAREN,  
    LEX_LBRACE,  
    LEX_RBRACE, 
    LEX_EQ,  
    LEX_LSS,  
    LEX_GTR,  
    LEX_PLUS,  
    LEX_MINUS, 
    LEX_STAR, 
    LEX_SLASH, 
    LEX_LEQ,  
    LEX_NEQ,  
    LEX_GEQ,  
    LEX_NUM,  
    LEX_ID,  
    POLIZ_LABEL,  
    POLIZ_ADDRESS,  
    POLIZ_GO,  
    POLIZ_FGO, 
};

class Lex
{
    type_of_lex t_lex;
    int v_lex;
    int stringnum, positionnum;
public:
    Lex(type_of_lex t = LEX_NULL, int v = 0, int s = 0, int p = 0)
    {
        t_lex = t; v_lex = v;
        stringnum = s; positionnum = p;
    }
    type_of_lex get_type() { return t_lex; }
    int get_value() { return v_lex; }
    int get_stringnum() {return stringnum; }
    int get_positionnum() {return positionnum; }
    friend ostream& operator << (ostream &s, Lex l)
    {
        s << '(' << l.t_lex << ',' << l.v_lex << ',' << l.stringnum << ',' << l.positionnum << ");";
        return s;
    }
};

class Ident
{
    char * name;
    type_of_lex type;
    bool declare;
    bool assign;
    int value;
    int stringnum, positionnum;

public:
    Ident()
    {
        declare = false;
        assign = false;
    }

    char* get_name()
    {
        return name;
    }

    void put_name(const char* s)
    {
        name = new char[strlen(s) + 1];
        strcpy(name, s);
    }

    bool get_declare()
    {
        return declare;
    }

    void put_declare()
    {
        declare = true;
    }

    type_of_lex get_type()
    {
        return type;
    }

    void put_type(type_of_lex t)
    {
        type = t;
       
        
    }

    bool get_assign()
    {
        return assign;
    }

    void put_assign()
    {
        assign = true;
    }

    int get_value()
    {
        return value;
    }

    void put_value(int v)
    {
        value = v;
    }
};

class table_ident
{
public:
    Ident * p;
    int size;
    int top;

public:
    table_ident(int max_size)
    {
        p = new Ident[max_size];
        top = 1;
    }

    ~table_ident()
    {
        delete[]p;
    }

    Ident& operator[] (int k)
    {
        return p[k];
    }

    int put(const char* buf)
    {
        for (int j = 1; j < top; ++j)
            if (!strcmp(buf, p[j].get_name()))
                return j;

        p[top].put_name(buf);
        ++top;
        return top - 1;
    }
    
    
};




class Scanner
{
    enum state { H, IDENT, NUMB, COM, ALE, DELIM, EQ, NEQ };
    static char * TW[];
    static char * TD[];
    static type_of_lex words[];
    static type_of_lex dlms[];
    state CS;
    char c;
    char buf[80];
    int buf_top;
    int stringnum =  1, positionnum = 0;
    void clear()
    {
        buf_top = 0;
        for (int j = 0; j < 80; ++j)
            buf[j] = '\0';
    }

    void add()
    {
        buf[buf_top++] = c;
    }

    int look(const char* buf, char** list)
    {
        int i = 0;
        while (list[i])
        {
            if (!strcmp(buf, list[i])) return i;
            ++i;
        }

        return 0;
    }

    void gc()
    {
        if (fin.good() )  fin.get(c); if ((c=='\n') || (c == '\t')) {stringnum++; positionnum = 0; } else positionnum++;  
    }

public:
    Lex get_lex();
    Scanner(const char * program)
    {
        if (!fin.is_open()) throw Exception("Файл не найден!");
        CS = H;
        clear();
        gc();
    }

};

char * Scanner::TW[] =
        {
                "",  
                "and",  
                "boolean",  
                "do",  
                "else",  
                "if",  
                "false",  
                "int", 
                "not", 
                "or",  
                "program", 
                "in",  
                "true", 
                "while",  
                "out", 
                NULL,  
        };

char * Scanner::TD[] =
        {
                "",  
                ";",  
                ",",  
                ":",  
                "=",  
                "(",  
                ")", 
                "{", 
                "}",  
                "==",  
                "<",  
                ">",  
                "+",  
                "-",  
                "*",  
                "/",  
                "<=",  
                "!=", 
                ">=",  
                NULL
        };

table_ident TID(100);

type_of_lex Scanner::words[] =
        {
                LEX_NULL,  
                LEX_AND,  
                LEX_BOOL,  
                LEX_DO,  
                LEX_ELSE, 
                LEX_IF,  
                LEX_FALSE,  
                LEX_INT, 
                LEX_NOT, 
                LEX_OR,  
                LEX_PROGRAM,  
                LEX_IN,  
                LEX_TRUE,  
                LEX_WHILE,  
                LEX_OUT,  
                LEX_NULL, 
        };

type_of_lex Scanner::dlms[] =
        {
                LEX_NULL,  
                LEX_SEMICOLON,  
                LEX_COMMA,  
                LEX_COLON,  
                LEX_ASSIGN,  
                LEX_LPAREN, 
                LEX_RPAREN,  
                LEX_LBRACE,  
                LEX_RBRACE,  
                LEX_EQ,  
                LEX_LSS,  
                LEX_GTR,  
                LEX_PLUS,  
                LEX_MINUS,  
                LEX_STAR,  
                LEX_SLASH,  
                LEX_LEQ, 
                LEX_NEQ,  
                LEX_GEQ,  
                LEX_NULL
        };

Lex Scanner::get_lex()
{
    int d, j, p, s;
    CS = H;
    do
    {
        switch (CS)
        {
            case H:
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
                    gc();
                else if (isalpha(c))
                {
                    s = stringnum;
                    p = positionnum;
                    clear();
                    add();
                    gc();
                    CS = IDENT;
                }
                else if (isdigit(c))
                {
                    s = stringnum;
                    p = positionnum;
                    d = c - '0';
                    gc();
                    CS = NUMB;
                }
                else if (c == '{') {
                    s = stringnum;
                    p = positionnum;
                    gc();
                    return Lex(LEX_LBRACE, 0, s, p);
                }
                else if (c == '}') {
                    s = stringnum;
                    p = positionnum;
                    gc();
                    return Lex(LEX_RBRACE, 0, s, p);
                }
                else if (c == '=' || c == '<' || c == '>')
                {
                    s = stringnum;
                    p = positionnum;
                    clear();
                    add();
                    gc();
                    
                    CS = ALE;
                }
                else if (c == '!')
                {
                    s = stringnum;
                    p = positionnum;
                    clear();
                    add();
                    gc();
                    CS = NEQ;
                }
                else
                {
                    CS = DELIM;
                    s = stringnum;
                    p = positionnum;
                }
                break;
            case IDENT:
                if (isalpha(c) || isdigit(c))
                {

                    add();
                    gc();
                }
                else
                

                if (j = look(buf, TW))
                    return Lex(words[j], j, s, p);
                else
                {
                    
                    j = TID.put(buf);
                    clear();
                    return Lex(LEX_ID, j, s, p);
                }
                    break;
            case NUMB:
                if (isdigit(c))
                {
                    d = d * 10 + (c - '0');
                    gc();
                }
                else
                    return Lex(LEX_NUM, d, s, p);
                break;
            case COM:
                if (c == '}')
                {
                    gc();
                    CS = H;
                }
                else if ( c == '{')
                    throw c;
                else
                    gc();
                break;
            case ALE:
                if (c == '=')
                {
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(dlms[j], j, s, p);
                }
                else
                {
                    j = look(buf, TD);
                    return Lex(dlms[j], j, s, p);
                }
                break;
            case NEQ:
                if (c == '=')
                {
                    add();
                    gc();
                    j = look(buf, TD);
                    return Lex(LEX_NEQ, j, s, p);
                }
                else
                    throw '!';
                break;
            case DELIM:
                clear();
                add();

                if (j = look(buf, TD))
                {
                    gc();
                    if (j == 15 && c == '*')
                    {
                        gc();
                        while (c != '/')
                        {
                            while (c != '*')
                                gc();
                            gc();
                        }
                        gc();
                        CS = H;
                        break;
                    }
                    
                    else return Lex(dlms[j], j, s, p);
                }
                else
                
                    throw c;
                
                break;
        }
        } while (1);
    

    
}





