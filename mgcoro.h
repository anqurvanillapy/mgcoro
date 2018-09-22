#ifndef _MGCORO_H
#define _MGCORO_H

namespace mgcoro {
namespace detail {

using status_t = int;

class coro_ctx;

} /* namespace detail */

class coroutine {
public:
	coroutine()
		: state_{0}
	{
		/* nop */
	}

	~coroutine() = default;

	bool
	is_complete() const
	{
		return state_ == -1;
	}

	bool
	is_child() const
	{
		return state_ < 0;
	}

	bool
	is_parent() const
	{
		return !is_child();
	}
private:
	friend class detail::coro_ctx;
	int state_;
};

namespace detail {

class coro_ctx {
public:
	/* explicit(false) */ coro_ctx(coroutine& c)
		: state_{c.state_}
		, modified_{false}
	{
		/* nop */
	}

	/* explicit(false) */ coro_ctx(coroutine* c)
		: state_{c->state_}
		, modified_{false}
	{
		/* nop */
	}

	~coro_ctx()
	{
		if (!modified_) {
			state_ = -1;
		}
	}

	coro_ctx(const coro_ctx&) = delete;
	coro_ctx& operator=(const coro_ctx&) = delete;
	coro_ctx(coro_ctx&&) = delete;
	coro_ctx& operator=(coro_ctx&&) = delete;

	constexpr explicit operator int() const
	{
		return state_;
	}

	int&
	operator=(int s)
	{
		modified_ = true;
		return state_ = s;
	}
private:
	int& state_;
	bool modified_;
};

} /* namespace detail */

#define resumable(c)                                \
	::mgcoro::detail::coro_ctx __ctx = (c);         \
	switch (static_cast<int>(__ctx))                \
		case -1:                                    \
			if (static_cast<int>(__ctx)) {          \
				goto __term_coro;                   \
			__term_coro:                            \
				__ctx = -1;                         \
				goto __bail_out_of_coro;            \
			__bail_out_of_coro:                     \
				break;                              \
			} else /* fallthrough */ case 0:

#define __yield_impl(n)                                                     \
	for (__ctx = (n); ; )                                                   \
		if (static_cast<int>(__ctx) == 0) {                                 \
			case (n): ;                                                     \
			break;                                                          \
		} else                                                              \
			switch (static_cast<int>(__ctx) ? 0 : 1)                        \
				while (true)                                                \
					/* fallthrough */ case -1: if (static_cast<int>(__ctx)) \
						goto __term_coro;                                   \
					else while (true)                                       \
					/* fallthrough */ case 1: if (static_cast<int>(__ctx))  \
						goto __bail_out_of_coro;                            \
					else                                                    \
					/* fallthrough */ case 0:

#define __fork_impl(n)                          \
	for (__ctx = -(n); ; __ctx = (n))           \
		if (static_cast<int>(__ctx) == (n)) {   \
			case -(n): ;                        \
			break;                              \
		} else

#define yield   __yield_impl(__LINE__)
#define fork    __fork_impl(__LINE__)

} /* namespace mgcoro */

#endif /* !_MGCORO_H */
