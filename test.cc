#include <iostream>
#include "mgcoro.h"

struct generator : mgcoro::coroutine {
	int
	operator()()
	{
		resumable (this) while (true) {
			yield return i;
			++i;
		}
		return 0;
	}

	int i{0};
};

struct arith_seq : mgcoro::coroutine {
	int
	operator()()
	{
		resumable (this) while (true) {
			yield {
				++i;
				i *= 2;
			}
		}
		return 0;
	}

	int i{0};
};

struct pair : mgcoro::coroutine {
	int
	operator()()
	{
		resumable (this) while (true) {
			yield return a;
			++a;
			yield return b;
			b *= 2;
		}
		return 0;
	}

	int a{0};
	int b{1};
};

struct duck : mgcoro::coroutine {
	void
	operator()()
	{
		resumable (this) while (true) {
			for (int i = 0; i < n; ++i) {
				std::cout << "quack!";
			}
			++n;
			yield;
		}
	}

	int n{1};
};

struct no_resume : mgcoro::coroutine {
	int
	operator()()
	{
		resumable (this) {
			// Quit the resumable scope.
			yield break;
		}
		return 42;
	}
};

struct branching  : mgcoro::coroutine {
	std::string
	operator()()
	{
		resumable (this) while (true) {
			do {
				fork {
					if (i < 2)
						yield return "parent";
					else
						return "parent exiting...";
					++i;
				}
			} while (is_parent());

			yield return "child";
			i = 0;
		}
		return "exit";
	}

	int i{0};
};

int
main()
{
	generator gen;
	for (int i = 0; i < 3; ++i) {
		std::cout << gen() << std::endl;
	}

	arith_seq seq;
	for (int i = 0; i < 3; ++i) {
		seq();
	}
	std::cout << seq.i << std::endl;

	pair p;
	for (int i = 0; i < 10; ++i) {
		std::cout << p() << ",";
	}
	std::cout << std::endl;

	duck mcdonald;
	for (int i = 0; i < 3; ++i) {
		mcdonald();
		std::cout << std::endl;
	}

	no_resume obj;
	std::cout << obj() << std::endl;

	branching branched_obj;
	for (int i = 0; i < 5; ++i) {
		std::cout << branched_obj() << std::endl;
	}

	return 0;
}
