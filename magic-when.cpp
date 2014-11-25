
#include <iostream>
#include <cstdio>
#include <cstring>

namespace Magic
{
	struct action
	{
		template<typename... Args>
		static const bool run(Args&&...)
		{
			return false;
		}
	};

	template<typename Act>
	struct then_do
	{
		template<typename... Args>
		static const bool run(Args&&... args)
		{
			return Act::run(args...);
		}
	};

	template<typename Act>
	struct else_do
	{
		template<typename... Args>
		static const bool run(Args&&... args)
		{
			return Act::run(args...);
		}
	};
	template<typename Expr, typename ThenDo, typename ElseDo=action>
	struct when
	{
		template<typename... Args>
		static const bool run(Args&&... args)
		{
			if(Expr::run(args...))
			{
				return ThenDo::run(args...);
			}
			return ElseDo::run(args...);
		}
	};

	template<typename When>
	struct repeat
	{
		template<typename... Args>
		static const bool run(Args&&... args)
		{
			if(When::run(args...) == false)
			{
				return repeat<When>::run(args...);
			}
			return true;
		}
	};
};

/** end of impl **/

struct ReadName: Magic::action
{
	static const bool run(std::string& name)
	{
		char buf[1024+1] = {0};
		char* ret;
		fprintf(stderr, "enter your name: ");
		ret = fgets(buf, 1024, stdin);
		if((ret == NULL) or (*ret == '\n') or feof(stdin))
		{
			fprintf(stderr, "fgets failed\n");
			return false;
		}
		name.append(buf, strlen(buf)-1);
		return true;
	}
};

struct PrintName: Magic::action
{
	static const bool run(std::string& name)
	{
		fprintf(stderr, "hello, %s!\n", name.c_str());
		return true;
	}
};

int main()
{
	using namespace Magic;
	std::string name;
	repeat<
		when<ReadName,
			then_do<PrintName>
		>
	>::run(name);
}

