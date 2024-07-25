export module InfNumber;

import Base;

/* Declaration of class InfInt */
export class InfInt
{
	/* constructors and destructors */
public:
	InfInt(std::vector<int> &in_digits, std::size_t in_base = 10, int in_sign = 1)
		: m_digits(new std::vector<int>(in_digits)), m_base(in_base), m_sign(in_sign)
	{}
	// I will destroy m_digits in destructor, so m_digits is a copy of digits, not reference!

	InfInt(std::string in_literal, std::size_t in_base = 10, int in_sign = 1);

	template <typename Ty_INT>
	InfInt(Ty_INT in_value, std::size_t in_base = 10);

	InfInt(const InfInt &in_other)  // copy_constructor
		: m_digits(in_other.m_digits), m_base(in_other.m_base), m_sign(in_other.m_sign)
	{}
	InfInt(InfInt &&in_other) noexcept  // move_constructor
		: m_digits(in_other.m_digits), m_base(in_other.m_base), m_sign(in_other.m_sign)
	{
		in_other.m_digits = nullptr;
	}

	~InfInt() {}

	/* APIs */
public:
	std::vector<int> *new_CopyDigits() const
	{
		std::vector<int> *copy = nullptr;
		if (m_digits) copy = new std::vector<int>(*m_digits);
		return copy;
	}
	// I will not offer get_digits because it is on heap! If I release m_digits you will deref a nullptr. so I
	// use word "new" in the function name, you should delete when you don't need it.

	std::size_t get_base() const { return m_base; }
	int get_sign() const { return m_sign; }

	template <typename Ty_INT = int>
		requires std::is_integral_v<Ty_INT>
	Ty_INT api_ToInteger();

	InfInt *new_CreateAbsValue() const
	{
		auto *abs = new InfInt(*this);
		abs->m_sign = 1;
		return abs;
	}
	void api_SetSelfToAbs() { this->m_sign = 1; }

	InfInt *new_ConvertBase(int in_base) const;

	friend std::ostream &operator<<(std::ostream &, const InfInt &);
	auto operator<=>(const InfInt &other);
	InfInt &operator=(const InfInt &in_other);
	InfInt &operator=(InfInt &&in_other) noexcept;

	/* private resource members */
private:
	std::vector<int> *m_digits;
	// if possible it can be substituted by a linkedlist

	std::size_t m_base;
	int m_sign;  // 1 for positive, -1 for negative, 0 for zero
};

/* do not export these helper functions. */
template <typename StreamType>
void outputStream(StreamType &stream, const InfInt &in)
{
	switch (in.get_sign()) {
		case 0:
			stream << "0";
			return;
		case +1:
			stream << "+";
			break;
		case -1:
			stream << "-";
			break;
	}

	switch (in.get_base()) {
		case 2:
			stream << "0b";
			break;
		case 8:
			stream << "0";
			break;
		case 16:
			stream << "0x";
			break;
	}

	if (in.get_base() != 2 && in.get_base() != 8 && in.get_base() != 10 && in.get_base() != 16) {
		stream << (char)(0x28);
	}
	std::vector<int> *d_List = in.new_CopyDigits();
	if (d_List) {
		for (auto It = d_List->rbegin(); It != d_List->rend(); ++It) {
			stream << DigitToChar(*It);
		}
	}
	if (in.get_base() != 2 && in.get_base() != 8 && in.get_base() != 10 && in.get_base() != 16) {
		stream << (char)(0x29) << "_" << in.get_base();
	}
}

template <>  // support for C++20 <format>
class std::formatter<InfInt>
{
public:
	constexpr auto parse(auto &ctx) { return ctx.begin(); }

	auto format(const InfInt &n, auto &ctx) const
	{
		std::stringstream ss;
		outputStream(ss, n);
		return std::format_to(ctx.out(), "{}", ss.str());
	}
};

/* class and export functions */
template <typename Ty_INT>
InfInt::InfInt(Ty_INT in_value, std::size_t in_base)
	: m_digits(new std::vector<int>()), m_base(in_base), m_sign(Sign(in_value))
{
	in_value = (m_sign == -1 ? -in_value : in_value);
	if (in_value == 0) {
		m_digits->push_back(0);
		return;
	}
	while (in_value) {
		m_digits->push_back(in_value % m_base);
		in_value /= m_base;
	}
}

InfInt::InfInt(std::string in_literal, std::size_t in_base, int in_sign)
	: m_digits(new std::vector<int>()), m_base(in_base), m_sign(in_sign)
{
	if (in_literal == "0") {
		m_sign = 0;
	}
	for (auto It = in_literal.rbegin(); It != in_literal.rend(); ++It) {
		auto ch = *It;
		if ('0' <= ch && ch <= '9') {
			m_digits->push_back((int)(ch - '0'));
		}
		else if ('A' <= ch && ch <= 'Z') {
			m_digits->push_back((int)(ch - 'A' + 10));
		}
		else {
			throw std::invalid_argument("literal char should in union range [0,9] and [A,Z]");
		}
	}
}

InfInt &InfInt::operator=(const InfInt &in_other)
{  // copy operator=
	if (this == &in_other) {
		return *this;
	}

	this->m_base = in_other.m_base;
	this->m_sign = in_other.m_sign;

	if (!(this->m_digits)) {
		delete this->m_digits;
	}

	this->m_digits = new std::vector<int>(*(in_other.m_digits));

	return *this;
}

InfInt &InfInt::operator=(InfInt &&in_other) noexcept
{  // move operator=
	if (this == &in_other) {
		return *this;
	}

	this->m_base = in_other.m_base;
	this->m_sign = in_other.m_sign;

	if (!(this->m_digits)) {
		delete this->m_digits;
	}

	this->m_digits = in_other.m_digits;
	in_other.m_digits = nullptr;

	return *this;
}

export std::ostream &operator<<(std::ostream &os, const InfInt &n)
{
	outputStream(os, n);
	return os;
}

template <typename Ty_INT>
	requires std::is_integral_v<Ty_INT>
Ty_INT InfInt::api_ToInteger()
{
	bool overflow_flags = false;

	Ty_INT val = 0, pow = 1;
	auto criticality = std::numeric_limits<Ty_INT>::max();
	for (auto bit : *(this->m_digits)) {
		overflow_flags = willValAddMulOverflow<Ty_INT>(val, pow, bit);
		val += pow * bit;
		pow *= this->m_base;
	}

	if (overflow_flags) {
		throw std::overflow_error("overflowed!");
	}

	return val;
}

/* Declaration of class InfFloat */
export class InfFloat
{
public:
private:
};