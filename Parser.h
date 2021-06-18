
#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Poliz.h"

using namespace std;




template <class T, int max_size> 
class Stack
{
    T s[max_size];
    int top;
    int counter;
public:
    Stack() { top = 0; counter = 0; }
    void reset() { top = 0; counter = 0; }
    bool is_full() { return top==max_size; }
    int check_top() { return top; }
    int check_counter() {return counter;}
    bool is_empty() { return top == 0; }
    void push(T i)
    {
        if (!is_full())
        {
            s[top] = i;
            ++top;
            ++counter;
        }
        else throw Exception( "Стек полон!");
        
    }
    
    T pop()
    {
        if (!is_empty())
        {
            --top;
            ++counter;
            return s[top];
            
        }
        else throw Exception("Стек пуст!");
    }
};





class Parser
{
    Lex curr_lex;
    type_of_lex c_type;
    int c_val;
    int c_stringnum;
    int c_positionnum;
    Scanner scan; 
    Stack<type_of_lex, 100> st_lex;
    Stack<int, 100> st_bracers;
    Stack<int, 100> st_parens;
    void P();
    void D1();
    void D();
    void B();
    void S();
    void E();
    void E1();
    void E2();
    void E3();
    void T();
    void F();
    void dec(type_of_lex type, int name, int val);
    void dec(type_of_lex type, int name);
    void check_id(int i);
    void check_op();
    void check_not();
    void eq_type();
    void eq_bool();
    void check_id_in_in();
    void get_comm();
    void gl()
    {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
        c_stringnum = curr_lex.get_stringnum();
        c_positionnum = curr_lex.get_positionnum();
    }
public:
    Poliz prog;
    void PR() {prog.print(); }

    Parser (const char *program) : scan(program), prog(1000)  {}
    void analyze();
};

void Parser::analyze() {
    gl();
    st_parens.reset();
    P();
    if (!st_bracers.is_empty()) throw Exception("Ожидалась скобка!", c_stringnum, c_positionnum);
}


void Parser::P() {
    
    if (c_type == LEX_LBRACE) {
        st_bracers.push(1);
        gl();
    }
    else throw Exception("Ожидалась скобка!", c_stringnum, c_positionnum);
    D1();
    B();
    if (c_type == LEX_RBRACE) {
        if (!st_bracers.is_empty()) st_bracers.pop();
        else throw Exception("Ожидалась скобка!", c_stringnum, c_positionnum);
    }
    else throw Exception("Ожидалась скобка!", c_stringnum, c_positionnum);
}



void Parser::D1() {
    while (c_type == LEX_INT || c_type == LEX_BOOL)
    {
        D();
        if (c_type != LEX_SEMICOLON) throw Exception("Ожидалась ;!", c_stringnum, c_positionnum);
        gl();

    }
}

void Parser::D()
{
    int i, ind = 1;
    if (c_type == LEX_INT)
    {
        gl();

        if (c_type != LEX_ID) {throw Exception ("Ошибка индентификатора!", c_stringnum, c_positionnum); }
        else
        {
            i = c_val;
            gl();
    
            if (c_type == LEX_ASSIGN)
            {
                gl();
        
                
                if (c_type == LEX_MINUS) { gl(); ind = -1; }
                else ind = 1;
                
                if (c_type == LEX_NUM) {dec(LEX_INT, i, c_val * ind);
                    check_id(i);
                    prog.put_lex(Lex(POLIZ_ADDRESS, i,c_stringnum, c_positionnum));
                    prog.put_lex(Lex(LEX_NUM, c_val, c_stringnum, c_positionnum));
                    prog.put_lex(Lex(LEX_ASSIGN, 0, c_stringnum, c_positionnum));
                }
                else
                    if (c_type == LEX_ID && TID[c_val].get_type() == LEX_INT){
                    dec(LEX_INT, i, TID[c_val].get_value() * ind);
                
                
                    check_id(i);
                    prog.put_lex(Lex(POLIZ_ADDRESS, i, c_stringnum, c_positionnum));
                    prog.put_lex(Lex(LEX_NUM, c_val, c_stringnum, c_positionnum));
                    prog.put_lex(Lex(LEX_ASSIGN, 0, c_stringnum, c_positionnum));
                    }
                
                else {throw Exception("Ошибка типа!", c_stringnum, c_positionnum); }
                gl();
        
            }
            else dec(LEX_INT, i);
        }
        while (c_type == LEX_COMMA) {
            gl();
    
            if (c_type != LEX_ID) {throw Exception("Ошибка индентификатора!", c_stringnum, c_positionnum);}
            else
            {
                i = c_val;
                gl();
        
                if (c_type == LEX_ASSIGN)
                {
                    gl();
            
                    if (c_type == LEX_MINUS) { gl(); ind = -1; }
                    else ind = 1;
                    
                    if (c_type == LEX_NUM) {dec(LEX_INT, i, c_val * ind);
                        check_id(i);
                        prog.put_lex(Lex(POLIZ_ADDRESS, i, c_stringnum, c_positionnum));
                        prog.put_lex(Lex(LEX_NUM, c_val, c_stringnum, c_positionnum));
                        prog.put_lex(Lex(LEX_ASSIGN,0, c_stringnum, c_positionnum));
                    }
                    else
                        if (c_type == LEX_ID && TID[c_val].get_type() == LEX_INT){
                            dec(LEX_INT, i, TID[c_val].get_value() * ind);
                            
                            
                            check_id(i);
                            prog.put_lex(Lex(POLIZ_ADDRESS, i, c_stringnum, c_positionnum));
                            prog.put_lex(Lex(LEX_NUM, c_val, c_stringnum, c_positionnum));
                            prog.put_lex(Lex(LEX_ASSIGN,0, c_stringnum, c_positionnum));
                        }
                    
                    
                    else { throw Exception ("Ошибка типа!", c_stringnum, c_positionnum); }
                    gl();
            
                }
                else dec(LEX_INT, i);
            }
        }
        if (c_type != LEX_SEMICOLON) throw Exception ("Ожидалась ;", c_stringnum, c_positionnum);
    } 
    
    
    if (c_type == LEX_BOOL)
    {
        gl();

        if (c_type != LEX_ID) {throw Exception("Ошибка индентификатора!", c_stringnum, c_positionnum);}
        else
        {
            i = c_val;
            gl();
    
            if (c_type == LEX_ASSIGN)
            {
                gl();
        
                if (c_type == LEX_TRUE)
                {
                    dec(LEX_BOOL, i, 1);
                    prog.put_lex(Lex(POLIZ_ADDRESS, i, c_stringnum, c_positionnum));
                    prog.put_lex(Lex(LEX_TRUE, c_val, c_stringnum, c_positionnum));
                    prog.put_lex(Lex(LEX_ASSIGN, 0, c_stringnum, c_positionnum));
                }
                else if (c_type == LEX_FALSE)
                {   dec(LEX_BOOL, i, 0);
                    prog.put_lex(Lex(POLIZ_ADDRESS, i, c_stringnum, c_positionnum));
                    prog.put_lex(Lex(LEX_FALSE, c_val, c_stringnum, c_positionnum));
                    prog.put_lex(Lex(LEX_ASSIGN, 0, c_stringnum, c_positionnum));
                }
                else if (c_type == LEX_ID && TID[c_val].get_type() == LEX_BOOL)
                {
                    dec(LEX_BOOL, i, TID[c_val].get_value());
                    prog.put_lex(Lex(POLIZ_ADDRESS, i, c_stringnum, c_positionnum));
                    prog.put_lex(Lex(LEX_BOOL, c_val, c_stringnum, c_positionnum));
                    prog.put_lex(Lex(LEX_ASSIGN, 0, c_stringnum, c_positionnum));
                }
                else {throw Exception("Ошибка типа!", c_stringnum, c_positionnum); }
                gl();
        
            }
            else dec(LEX_BOOL, i);
        }
        
        while (c_type == LEX_COMMA) {
            gl();
    
            if (c_type != LEX_ID) {throw Exception("Ошибка индентификатора!", c_stringnum, c_positionnum);}
            else
            {
                i = c_val;
                gl();
        
                if (c_type == LEX_ASSIGN)
                {
                    gl();
            
                    if (c_type == LEX_TRUE)
                    {
                        dec(LEX_BOOL, i, 1);
                        prog.put_lex(Lex(POLIZ_ADDRESS, i, c_stringnum, c_positionnum));
                        prog.put_lex(Lex(LEX_TRUE, c_val, c_stringnum, c_positionnum));
                        prog.put_lex(Lex(LEX_ASSIGN, 0, c_stringnum, c_positionnum));
                    }
                    else if (c_type == LEX_FALSE)
                    {   dec(LEX_BOOL, i, 0);
                        prog.put_lex(Lex(POLIZ_ADDRESS, i, c_stringnum, c_positionnum));
                        prog.put_lex(Lex(LEX_FALSE, c_val, c_stringnum, c_positionnum));
                        prog.put_lex(Lex(LEX_ASSIGN,0, c_stringnum, c_positionnum));
                    }
                    else if (c_type == LEX_ID && TID[c_val].get_type() == LEX_BOOL)
                    {
                        dec(LEX_BOOL, i, TID[c_val].get_value());
                        prog.put_lex(Lex(POLIZ_ADDRESS, i, c_stringnum, c_positionnum));
                        prog.put_lex(Lex(LEX_BOOL, c_val, c_stringnum, c_positionnum));
                        prog.put_lex(Lex(LEX_ASSIGN, 0, c_stringnum, c_positionnum));
                    }
                    else {throw Exception("Ошибка типа!", c_stringnum, c_positionnum); }
                    gl();
            
                }
                else dec(LEX_BOOL, i);
            }
        }
        if (c_type != LEX_SEMICOLON) throw Exception("Ожидалась ;", c_stringnum, c_positionnum);
    } 
}


void Parser::B()
{
    while (c_type != LEX_RBRACE) {
        if (st_bracers.is_empty()) throw Exception("Ожидалась скобка!", c_stringnum, c_positionnum);
        S();
    }
}


void Parser::S()
{
    int pl0, pl1, pl2, pl3, pl4, pl5, k = 0;

    if (c_type == LEX_IF)
    {
        st_parens.reset();
        gl();

        if (c_type == LEX_LPAREN) 
        {
            gl();
            st_parens.push(1);
    
            
            
            E();
            eq_bool();
            if (!((st_parens.check_top() == 1) and ((st_parens.check_counter() > 7) or (st_parens.check_counter() < 3)))){
                    throw Exception("Ошибка в конструкции, неправильно составлено условие или расставлены скобки!", c_stringnum, c_positionnum);;
                }
                    
            if (c_type == LEX_RPAREN)
            {
                gl();
                st_parens.pop();
        
                pl2 = prog.get_free();
                prog.blank();
                prog.put_lex(Lex(POLIZ_FGO));

                if (c_type == LEX_LBRACE)
                {
                    gl();
            
                    st_bracers.push(1);
                    

                    while (c_type != LEX_RBRACE) {
                        S();
                    }
                    if (!st_bracers.is_empty()) st_bracers.pop();
                    else throw Exception("Ожидалась скобка!", c_stringnum, c_positionnum);
                    gl();
            

                }
                else
                S();
                if (c_type == LEX_ELSE)
                {
                    pl3 = prog.get_free();
                    prog.blank();
                    prog.put_lex(Lex(POLIZ_GO));
                    prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
                    gl();
            
                    if (c_type == LEX_LBRACE)
                    {
                        st_bracers.push(1);
                        gl();
                
                        while (c_type != LEX_RBRACE) S();
                        if (!st_bracers.is_empty()) st_bracers.pop();
                        else throw Exception("Ожидалась скобка!", c_stringnum, c_positionnum);
                        gl();
                
                    }
                    else S();
                    prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
                }  
                
                else prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);

            } 
            else throw Exception("Ошибка в конструкции if!", c_stringnum, c_positionnum);
        } 
        else throw Exception("Ошибка в конструкции if, отсутствует условие или скобка!", c_stringnum, c_positionnum);
    } 
    
    else if (c_type == LEX_DO)
    {


        pl0 = prog.get_free();

        gl();

        if (c_type == LEX_LBRACE)
        {

            st_bracers.push(1);
            gl();
    

            while (c_type != LEX_RBRACE) S();

            if (!st_bracers.is_empty()) st_bracers.pop();
            else throw Exception("Ожидалась скобка!", c_stringnum, c_positionnum);
            gl();
    
        }
        else S();

        if (c_type == LEX_WHILE)
        {

            gl();
    
            if (c_type == LEX_LPAREN) {
                
                
                gl();
                st_parens.push(1);
                E();
                eq_bool();
                if (c_type == LEX_RPAREN)
                {
                    st_parens.pop();
                    pl1 = prog.get_free();
                    prog.blank();
                    prog.put_lex ( Lex ( POLIZ_FGO, 0, c_stringnum, c_positionnum ) );
                    prog.put_lex ( Lex ( POLIZ_LABEL , pl0, c_stringnum, c_positionnum ) );
                    prog.put_lex ( Lex ( POLIZ_GO, 0, c_stringnum, c_positionnum ) );
                    prog.put_lex ( Lex ( POLIZ_LABEL , prog.get_free(), c_stringnum, c_positionnum ) , pl1 );
                    
                    gl();
            
                    if (c_type != LEX_SEMICOLON) throw Exception("Ожидалась ;", c_stringnum, c_positionnum);
                    gl();
            
                } 
                else throw Exception("Ошибка в конструкции do-while!", c_stringnum, c_positionnum);
                
            } 
            else throw Exception("Ошибка в конструкции do-while!", c_stringnum, c_positionnum);
            
        } 
        else throw Exception("Ошибка в конструкции do-while!", c_stringnum, c_positionnum);
        
    } 
    
    
    
    
    
    else if (c_type == LEX_WHILE)
    {   
        st_parens.reset();
        gl();
        if (c_type == LEX_LPAREN)
        {
            gl();
            st_parens.push(1);
             // if (c_type == LEX_LPAREN){
             //     gl();
             //     if (c_type == LEX_LPAREN){
             //        gl();
             //        if (c_type != LEX_LPAREN)
             //         throw Exception("Ошибка в конструкции, неправильно составлено условие или раставлены скобки", c_stringnum, c_positionnum);
             //     }
             // }
            pl0 = prog.get_free();
            E();
            eq_bool();
            pl1=prog.get_free();
            prog.blank();
            prog.put_lex(Lex(POLIZ_FGO, 0, c_stringnum, c_positionnum));
            if (!((st_parens.check_top() == 1) and (st_parens.check_counter() > 7))){
                    throw Exception("Ошибка в конструкции, неправильно составлено условие или расставлены скобки!", c_stringnum, c_positionnum);;
                }
                    
            if (c_type == LEX_RPAREN)
            {
                gl();
                st_parens.pop();
                if (!st_parens.is_empty())
                    throw Exception("Ошибка в конструкции, неправильно составлено условие или расставлены скобки!", c_stringnum, c_positionnum);

                if (c_type == LEX_LBRACE)
                {
                    gl();
            
                    st_bracers.push(1);
                    while (c_type != LEX_RBRACE) {
                        S();
                    }
                    if (!st_bracers.is_empty()) st_bracers.pop();
                    else throw Exception("Ожидалась скобка!", c_stringnum, c_positionnum);
                    gl();
            
                }
                else S();
                
                prog.put_lex(Lex(POLIZ_LABEL, pl0, c_stringnum, c_positionnum));
                prog.put_lex(Lex(POLIZ_GO, 0, c_stringnum, c_positionnum));
                
               prog.put_lex(Lex(POLIZ_LABEL, prog.get_free(), c_stringnum, c_positionnum), pl1);
            } 
            
            else throw Exception("Ошибка в конструкции, неправильно составлено условие или расставлены скобки", c_stringnum, c_positionnum);
        } 
        
        else throw Exception("Ошибка в конструкции while!", c_stringnum, c_positionnum);
    }
    
    else if (c_type == LEX_IN)
    {
        gl();

        if (c_type == LEX_LPAREN)
        {
            gl();
            st_parens.push(1);
    
            if (c_type == LEX_ID)
            {
                check_id_in_in();
                prog.put_lex(Lex(POLIZ_ADDRESS, c_val, c_stringnum, c_positionnum));
                gl();
        
            }
            else throw Exception("Некорректная лексема!", c_stringnum, c_positionnum);
            
            if (c_type == LEX_RPAREN)
            {
                gl();
                st_parens.pop();
        
                prog.put_lex(Lex(LEX_IN, 0, c_stringnum, c_positionnum));
                if (c_type != LEX_SEMICOLON) throw Exception("Ожидалась ;", c_stringnum, c_positionnum);
                else gl();
        
            }
        }
        
            else throw Exception("Некорректная лексема!", c_stringnum, c_positionnum);
    } 
    
    else if (c_type == LEX_OUT)
    {
        gl();

        if (c_type == LEX_LPAREN) {
            gl();
            st_parens.push(1);
            E();
            while (c_type == LEX_COMMA)
            {
                prog.put_lex(Lex(LEX_OUT, 0, c_stringnum, c_positionnum));
                gl();
        
                E();
            }
            
            if (c_type == LEX_RPAREN) {
                gl();
                st_parens.pop();
        
                prog.put_lex(Lex(LEX_OUT, 0, c_stringnum, c_positionnum));
                if (c_type != LEX_SEMICOLON) throw Exception("Ожидалась ;", c_stringnum, c_positionnum);
                else gl();
        
            }
            
        }         else throw Exception("Некорректная лексема!", c_stringnum, c_positionnum);

    } 
    
    else if (c_type == LEX_INT || c_type == LEX_BOOL)
    {
        D();
        gl();

    }
    
    else if (c_type == LEX_ID)
    {
        int j = c_val;
        gl();

        check_id(j);
        prog.put_lex(Lex(POLIZ_ADDRESS, j, c_stringnum, c_positionnum));
        
        if (c_type == LEX_ASSIGN) {
            gl();
    
            E();
            while (c_type == LEX_ASSIGN)
            {
                int k  = prog.p[prog.free - 1].get_value(), k2 = prog.p[prog.free - 2].get_value();
                type_of_lex  t = st_lex.pop();
                st_lex.push(t);
                prog.free--;
                prog.p[prog.free - 1] = Lex(POLIZ_ADDRESS, k);
                gl();
        
                E();
                eq_type();
                st_lex.push(t);
                prog.put_lex(Lex(LEX_ASSIGN, 0, c_stringnum, c_positionnum));
                prog.put_lex(Lex(POLIZ_ADDRESS, k2, c_stringnum, c_positionnum));
                prog.put_lex(Lex(LEX_ID, k, c_stringnum, c_positionnum));
            }
            eq_type();
            prog.put_lex(Lex(LEX_ASSIGN, 0, c_stringnum, c_positionnum));
            if (c_type != LEX_SEMICOLON) throw Exception("Ожидалась ;", c_stringnum, c_positionnum);
            gl();
    
        }
       else throw Exception("Некорректная лексема!", c_stringnum, c_positionnum);
    }
        else B();
}


void Parser::E()
{
    E1();
    if (c_type == LEX_OR)
    {
        type_of_lex t1 = c_type;
        st_lex.push(c_type);
        gl();

        E1();
        check_op();
        prog.put_lex(Lex(t1, 0, c_stringnum, c_positionnum));
    }
}

void Parser::E1()
{
    E2();
    if (c_type == LEX_AND)
    {
        type_of_lex t1 = c_type;
        st_lex.push(c_type);
        gl();

        E2();
        check_op();
        prog.put_lex(Lex(t1, 0, c_stringnum, c_positionnum));
    }
}

void Parser::E2()
{
    E3();
    if ( c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR || c_type == LEX_LEQ
        || c_type == LEX_GEQ || c_type == LEX_NEQ )
    {
        type_of_lex t1 = c_type;
        st_lex.push( c_type );
        gl();

        E3();
        check_op();
        prog.put_lex(Lex(t1, 0, c_stringnum, c_positionnum));
    }
}

void Parser::E3()
{
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS)
    {
        type_of_lex t1 = c_type;
        st_lex.push(c_type);
        gl();

        T();
        check_op();
        prog.put_lex(Lex(t1, 0, c_stringnum, c_positionnum));
    }
    
}

void Parser::T()
{
    F();
    while ( c_type == LEX_STAR || c_type == LEX_SLASH)
    {
        type_of_lex t1 = c_type;
        gl();

        if (c_type == LEX_STAR)
        {
            get_comm();
        }
        else {
            st_lex.push(t1);
            F();
            check_op();
            prog.put_lex(Lex(t1, 0, c_stringnum, c_positionnum));
        }
    }
}

void Parser::F()
{
    if (c_type == LEX_ID)
    {
        check_id(c_val);
        prog.put_lex(Lex(LEX_ID, c_val, c_stringnum, c_positionnum));
        
        gl();

    }
    else if (c_type == LEX_NUM)
    {
        st_lex.push(LEX_INT);
        prog.put_lex(curr_lex);
        gl();

    }
    else if (c_type == LEX_LPAREN)
    {
        gl();
        st_parens.push(2);
        E();
        if (c_type == LEX_RPAREN){
            st_parens.pop();

            gl();
    
        }
        else throw ("Некорректная лексема!", c_stringnum, c_positionnum);
        
    }
    else if (c_type == LEX_TRUE)
    {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_BOOL, 1, c_stringnum, c_positionnum));
        gl();

    }
    else if (c_type == LEX_FALSE)
    {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_BOOL, 0,  c_stringnum, c_positionnum));
        gl();

    }
    else if (c_type == LEX_NOT)
    {
        gl();

        F();
        check_not();
    }
    else if (c_type == LEX_MINUS)
    {
        gl();

        F();
        prog.put_lex(Lex(LEX_NUM, -1, c_stringnum, c_positionnum));
        prog.put_lex(Lex(LEX_STAR, 0, c_stringnum, c_positionnum));
    }
   else throw Exception("Некорректная лексема!", c_stringnum, c_positionnum);
    
}

void Parser::dec ( type_of_lex type, int name, int val ){
    if (TID[name].get_declare()) throw Exception("Переменная уже объявлена!", c_stringnum, c_positionnum );
    TID[name].put_declare();
    TID[name].put_assign();
    TID[name].put_type(type);
    TID[name].put_value(val);
    
}

void Parser::check_id(int i = -1) {
    if (i == -1) i = c_val;
    if (TID[i].get_declare()) st_lex.push(TID[i].get_type());
    else throw Exception("Переменная не объявлена!", c_stringnum, c_positionnum);
}

void Parser::eq_type() {
    if ( st_lex.pop() != st_lex.pop() ) throw Exception("Ошибка сопоставления типов!", c_stringnum, c_positionnum);
    
}

void Parser::check_op() {
    type_of_lex t1, t2, op, t = LEX_INT, b = LEX_BOOL;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();
    
    if ((t1 == LEX_BOOL) && (t2 == LEX_BOOL) && ((op == LEX_EQ) || (op == LEX_NEQ))) throw Exception("Неверные типы в операции!", c_stringnum, c_positionnum);
    if (t1 == LEX_NUM) t1 = LEX_INT;
    if (t2 == LEX_NUM) t2 = LEX_INT;
    
    if (op == LEX_MINUS || op == LEX_PLUS || op == LEX_STAR || op == LEX_SLASH)
        b = LEX_INT;
    if (op == LEX_OR || op == LEX_AND )
        t = LEX_BOOL;
    if (t1 == t2 && t1 == t)
        st_lex.push(b);
    else throw Exception("Неверные типы в операции!", c_stringnum, c_positionnum);
    
}

void Parser::eq_bool() {
    if (st_lex.pop() != LEX_BOOL ) throw Exception("Ошибка в конструкции, неправильно составлено условие или раставлены скобки", c_stringnum, c_positionnum);
}

void Parser::check_id_in_in() {
    if ( !TID [c_val].get_declare() ) throw Exception("Ошибка ввода, переменная не объявлена! \n", c_stringnum, c_positionnum);
}

void Parser::check_not() {
    if (st_lex.pop() != LEX_BOOL) throw Exception("Выражение составлено неверно!", c_stringnum, c_positionnum);
    else
    {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_NOT, 0, c_stringnum, c_positionnum));
    }
}

void Parser::get_comm() {
    gl();
    while (c_type != LEX_SLASH)
    {
        while (c_type != LEX_STAR){
            gl();
    
        }
        gl();

    }
    gl();
}

void Parser::dec(type_of_lex type, int name){
    if (TID[name].get_declare()) throw Exception("Переменная уже объявлена!", c_stringnum, c_positionnum);
    TID[name].put_declare();
    TID[name].put_type(type);
}


