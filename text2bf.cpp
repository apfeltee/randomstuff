
#include <mars/IO.hpp>
#include <mars/File.hpp>
#include <mars/Strings.hpp>
#include <mars/Array.hpp>


class factorPair {
    int f1;
    int f2;
    int rest;

    public:
        factorPair()
        {
        }

        factorPair(int f1, int f2, int rest)
        {
            this->f1 = f1;
            this->f2 = f2;
            this->rest = rest;
        }

        int getLength()
        {
            return 7+f1+f2+abs(rest);
        }

        void output(bool bigger)
        {
            int i;
            char r;
            printf(">");
            for (i = 0; i < f1;i++)
                printf("+");
            printf("[<");
            r = bigger ? '+' : '-';
            for(i = 0; i < f2;i++)
                printf("%c", r);
            printf(">-]<");
            r = ((rest < 0 && bigger) || (rest > 0 && (!bigger))) ? '-' : '+';
            rest = abs(rest);
            for(i=0; i<rest; i++)
            {
                printf("%c", r);
            }
        }
};

void outputNoLoop(int difference, bool bigger)
{
    int i;
    for(i=0; i<difference; i++)
    {
        printf("%c", bigger ? '+' : '-');
    }
}


void generate(int prev, int c)
{
    bool bigger;
    int i;
    int j;
    int k;
    int l;
    int rest;
    int shortest;
    int difference;
    int absDifference;
    factorPair fShortest;
    mars::Array<factorPair> factorPairs;
    difference = c-prev;
    if(difference == 0)
    {
        return;
    }
    bigger = c > prev;
    absDifference = abs(difference);
    for (i=2;i < (absDifference * 2);i++)
    {
        rest = absDifference - i;
        for(j=2; j<=(i/2); j++)
        {
            for (k=j; k<=(i/2); k++)
            {
                if((j*k) == i)
                {
                    factorPairs.append(factorPair(j, k, rest));
                }
            }
        }
    }

    shortest=absDifference;
    for (i=0; i<(int)(factorPairs.size()); i++)
    {
        factorPair f = (factorPair)factorPairs.at(i);
        l = f.getLength();
        if(l < shortest)
        {
            shortest = l;
            fShortest = f;
        }
    }

    if(shortest == absDifference)
    {
        outputNoLoop(absDifference,bigger);
    }
    else
    {
        fShortest.output(bigger);
    }
}




void convert(const mars::mstring& text)
{
    char c;
    int i;
    int prev = 0;
    for (i=0; i<(int)(text.length()); i++)
    {
        c = text.at(i);
        generate(prev,c);
        prev = c;
        printf(".");
    }
}

int main(int argc, char* argv[])
{
    mars::File handle;
    mars::mstring filename;
    if(argc > 1)
    {
        filename = argv[1];
        try
        {
            handle.open(filename, "r");
            convert(handle.readUntil(-1));
            handle.close();
        }
        catch(mars::IOError& err)
        {
            mars::Stderr << "Cannot open '" << filename << "': " << err.what() << "\n";
            return 1;
        }
    }
    else
    {
        mars::Stdout << "Usage: " << argv[0] << " <filename>\n";
        return 1;

    }

}
