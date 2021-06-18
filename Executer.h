
#include "Parser.h"

using namespace std;

class Executer
{
    Lex pc_el;
public:
    void execute(Poliz &prog);
};

void Executer::execute(Poliz &prog)
{
    Stack <int, 100> args;
    Stack <type_of_lex ,100> arg_types;
    int i, j, index = 0, size = prog.get_free();
    type_of_lex t;
    
    while (index < size)
    {
        pc_el = prog[index];
        
        switch (pc_el.get_type())
        {
            case LEX_TRUE:
            case LEX_FALSE:
            case LEX_BOOL:
            case POLIZ_ADDRESS:
            case POLIZ_LABEL:
                args.push(pc_el.get_value());
                arg_types.push(pc_el.get_type());
                break;
            case LEX_NUM:
                args.push(pc_el.get_value());
                arg_types.push(LEX_INT);
                break;
            case LEX_ID:
                i = pc_el.get_value();
                if (TID[i].get_assign())
                {
                    args.push(TID[i].get_value());
                    arg_types.push(TID[i].get_type());
                    break;
                }
                else throw Exception("ПОЛИЗ: неизвестный индентификатор!", pc_el.get_stringnum(), pc_el.get_positionnum());
            case LEX_NOT:
                args.push(!args.pop());
                break;
            case LEX_OR:
                i = args.pop();
                args.push(args.pop() || i);
                arg_types.pop();
                break;
            case LEX_AND:
                i = args.pop();
                arg_types.pop();
                args.push(args.pop() && i);
                break;
            case POLIZ_GO:
                index = args.pop() - 1;
                arg_types.pop();
                break;
            case POLIZ_FGO:
                i = args.pop();
                arg_types.pop();
                if (!args.pop())
                    index = i - 1;
                arg_types.pop();
                break;
            case LEX_OUT:
            {
                i = args.pop();
                t = arg_types.pop();
                switch (t)
                {
                    case LEX_INT:
                        cout << i << endl;
                        break;
                    case LEX_BOOL:
                        if (i == 0) cout << "false" << endl;
                        else cout << "true" << endl;
                        break;
                    default: "Ошибка вывода!";
                }
                break;
            }
            case LEX_IN: 
            {
                int k;
                i = args.pop();
                arg_types.pop();
                if (TID[i].get_type() == LEX_INT) {
                    cin >> k;
                    TID[i].put_value(k);
                    TID[i].put_assign();
                }
                if (TID[i].get_type() == LEX_BOOL) {
                    char j[20];
                rep:
                    cin >> j;
                    if (!strcmp(j, "true"))
                        k = 1;
                    else if (!strcmp(j, "false"))
                        k = 0;
                    else {
                        cout << "Ошибка ввода!" << endl;
                        goto rep;
                    }
                    TID[i].put_value(k);
                    TID[i].put_assign();
                }
                
                break;
            }
            case LEX_PLUS:
            {
                t = arg_types.pop();
                switch (t)
                {
                    case LEX_INT:
                        args.push(args.pop() + args.pop());
                        break;
                    case LEX_BOOL:
                        args.push(args.pop() + args.pop());
                        break;
                }
                
                break;
            }
            case LEX_STAR:
                args.push(args.pop() * args.pop());
                arg_types.pop();
                break;
            case LEX_MINUS:
                i = args.pop();
                arg_types.pop();
                args.push(args.pop() - i);
                break;
            case LEX_SLASH:
                i = args.pop();
                arg_types.pop();
                if (i)
                {
                    args.push(args.pop() / i);
                    break;
                }
                else throw Exception("ПОЛИЗ: деление на ноль!",pc_el.get_stringnum(), pc_el.get_positionnum());
            case LEX_EQ:
            {
                t = arg_types.pop();
                switch (t)
                {
                    case LEX_INT:
                    case LEX_NUM:
                        args.push(args.pop() == args.pop());
                        break;
                }
                arg_types.pop();
                arg_types.push(LEX_BOOL);
                break;
            }
            case LEX_LSS:
            {
                t = arg_types.pop();
                switch (t)
                {
                    case LEX_INT:
                    case LEX_NUM:
                        args.push(args.pop() > args.pop());
                        break;
                }
                arg_types.pop();
                arg_types.push(LEX_BOOL);
                break;
            }
            case LEX_GTR:
            {
                t = arg_types.pop();
                switch (t)
                {
                    case LEX_INT:
                    case LEX_NUM:
                        args.push(args.pop() < args.pop());
                        break;
                }
                arg_types.pop();
                arg_types.push(LEX_BOOL);
                break;
            }
            case LEX_LEQ:
                i = args.pop();
                arg_types.pop();
                args.push(args.pop() <= i);
                break;
            case LEX_GEQ:
                i = args.pop();
                arg_types.pop();
                args.push(args.pop() >= i);
                break;
            case LEX_NEQ:
                i = args.pop();
                arg_types.pop();
                args.push(args.pop() != i);
                break;
            case LEX_ASSIGN:
                i = args.pop();
                j = args.pop();
                arg_types.pop();
                arg_types.pop();
                TID[j].put_value(i);
                TID[j].put_assign();
                break;
            default:
                throw Exception("ПОЛИЗ: неизвестный элемент!", pc_el.get_stringnum(), pc_el.get_positionnum());
        } 
        ++index;
    }
    cout << "Программа завершена!" << endl;
}





class Interpretator
{
    Parser pars;
    Executer E;
public:
    Interpretator(char *program):pars(program) {};
    void interpretation();
};




void Interpretator::interpretation()
{
    pars.analyze();
    E.execute(pars.prog);
}



