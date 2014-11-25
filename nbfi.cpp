
/*
* additional supported tokens:
*
*   "<data>"
*       acts as static string data chunk. REPLACES the current memory
*       with <data>.
*       example:
*
*           "hello world!"_
*
*       prints hello world using the underscore op.
*
************
*
*   _
*       prints the contents of the memory buffer all at once.
*       useful in conjunction with the string op.
*
************
*
*   @
*       purges the memory buffer.
*
************
*
*   $
*       initiates the secondary buffer, and contents of the main buffer
*       are written to the secondary buffer.
*       useful as sort-of variable.
*
************
*
*   &
*       replaces content of the main buffer with the content of
*       the secondary buffer.
*
************
*
*   ?
*       similar to the quotes op, but reads from stdin until a
*       a newline has been encountered.
*       contents of the mainbuffer are overwritten with the
*       read data.
*       WARNING: this is __not__ a replacement for the dot op!
*       useful only for linebased input.
*       example:
*
*           "Enter your name"_
*           ?$
*           "Hello"_&_
*
*
*
*/

#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <cstdio>

#define croak(...) fprintf(stderr, __VA_ARGS__);

template<typename Key, typename Value>
std::map<Key, Value> shittybimap(const std::map<Key, Value>& mp)
{
    std::map<Key, Value> newmp;
    for(auto pair: mp)
    {
        newmp[pair.second] = pair.first;
    }
    return newmp;
    // go fuck yourself, at least it works
}

template<typename Key, typename Value>
Value inverseget(std::map<Key, Value>& map, const Key& k)
{
    
}


struct Fuck
{
    using JTable = std::map<int, int>;

    JTable jumpback_table;
    std::string program;
    char d[30000];

    Fuck(const std::string& pg): program(pg)
    {
        read_program();
    }

    void read_program()
    {
        std::vector<int> jump_to;
        for(size_t idx=0; idx<program.size(); idx++)
        {
            auto ch = program[idx];
            switch(ch)
            {
                case '[':
                    jump_to.push_back(idx);
                    break;
                case ']':
                    jumpback_table[idx] = jump_to.back();
                    jump_to.pop_back();
                    break;
            }
        }
    }

    void run()
    {
        int ch;
        int ex;
        size_t dc = 0;
        size_t pc = 0;
        while(pc < program.size())
        {
            if(program[pc] == '"')
            {
                pc++;
                dc = 0;
                while(true)
                {
                    ch = program[pc];
                    if(ch == '"')
                    {
                        pc++;
                        goto handle_rest;
                    }
                    else
                    {
                        switch(ch)
                        {
                            case '\\':
                                ex = program[++pc];
                                switch(ex)
                                {
                                    case 'n':
                                        ch = '\n';
                                        break;
                                    default:
                                        croak("!bad escape code (\\%c)\n", ex);
                                        break;
                                }
                                break;
                            default:
                                break;
                        }
                        d[dc++] = ch;
                        pc++;
                    }
                }
            }
            handle_rest:
            switch(program[pc])
            {
                case '>':
                    dc += 1;
                    break;
                case '<':
                    dc -= 1;
                    break;
                case '+':
                    d[dc] += 1;
                    break;
                case '-':
                    d[dc] -= 1;
                    break;
                case '.':
                    putchar(d[dc]);
                    break;
                case ',':
                    d[dc] = getchar();
                    break;
                case '[':
                    if(d[dc] == 0)
                    {
                        //pc = shittybimap(jumpback_table)[pc];
                        pc = inverseget(jumpback_table, pc);
                    }
                    break;
                case ']':
                    if(d[dc] != 0)
                    {
                        pc = jumpback_table[pc];
                    }
                    break;
                case '_':
                    std::cout.write(d, dc);
            }
            pc += 1;

        }
    }
};

int main()
{
    #if 0
    Fuck fk(R"(
        >++++++++[<+++++++++>-]<.>>+>+>++>[-]+<[>
        [->+<<++++>]<<]>.+++++++..+++.>>+++++++.<
        <<[[-]<[-]>]<+++++++++++++++.>>.+++.-----
        -.--------.>>+.>++++.
    )");
    #else
    Fuck fk(R"(
"hello world\n"_
"some
multiline
text\n"_
    )");
    #endif
    fk.run();
}

