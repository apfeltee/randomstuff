
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <tuple>

struct EachTuple
{
    template<typename Callback>
    class EachElement
    {
        private:
            Callback m_callback;

        public:
            EachElement(Callback fn): m_callback(fn)
            {
            }

            template<std::size_t Index=0, typename... Args>
            typename std::enable_if<(Index == (sizeof...(Args))), void>::type
            call(std::tuple<Args...>& tuple)
            {
            }

            template<std::size_t Index=0, typename... Args>
            typename std::enable_if<(Index < (sizeof...(Args))), void>::type
            call(std::tuple<Args...>& tuple)
            {
                m_callback(std::get<Index>(tuple));
                call<Index + 1, Args...>(tuple);
            }
    };

    template<typename Object, typename... Args>
    static void ForEach(std::tuple<Args...>& tuple, Object fn)
    {
        TupleHelper::EachElement<Object> each(fn);
        each.call(tuple);
    };
};

struct printer
{
    template<typename Type>
    void operator ()(const Type& val)
    {
        std::cout << "printer: val = " << val << std::endl;
    }
};


int main()
{
    auto t = std::make_tuple(42, 'H', 8.32e1, "hello", 10, 1);
    auto fn = [&](int val)
    {
        std::cout << "TupleHelper::foreach: val = " << val << std::endl;
    };
    EachTuple::ForEach(t, printer());
}
