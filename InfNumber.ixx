export module InfNumber;

import <vector>;
import <string>;

import <iostream>;
import <sstream>;
import <format>;

import <stdexcept>;
import <limits>;

/*
 * std::string DigitToChar(int)
 *    Convert the positive int n to a std::string sn, where when 0 <= n <= 9, sn is a single
 *    character string composed of the characters corresponding to n. When 10 <= n <= 35, sn
 *    is a single character string mapped from the uppercase alphabet in the order of 10->A,
 *    11->B,..., 35->Z. When n>36, output the decimal representation string of n. If n is
 *    negative, the output result is equivalent to the absolute value of n.
 * 
 * template <typename StreamType>
 * void outputStream(StreamType&, const InfInt&)
 *    Output InfInt integer to StreamType& stream, where StreamType& is used to identify the
 *    type of output stream, such as std::ostream, std::stringstream, etc. The output method
 *    is S(INTSTR)_(PSTR), where S is represented by "+", "-", or "" for positive, negative,
 *    and zero values, respectively. INTSTR is a PSTR(=_base) base string generated according
 *    to the mapping rules of std::string DigitToChar (int), and PSTR is _base's base 10 string.
 *    There are specializations for the four cases of binary, octal, decimal, and hexadecimal,
 *    and the output mode is changed to S#PREVIX#INTSTR, where the prefix PREVIX is "0b", "0", 
 *    "", and "0x" respectively.
 * 
 * Class InfInt
 *	  // TODO 没写完， 注意写完后改成doxygen文档注释
 */

export std::string DigitToChar(int n)
{
	std::string CharStr;
	if (n < 0) CharStr = DigitToChar(-n);
	else if (n < 10) CharStr.push_back((char)(n + 0x30));
	else if (n < 36) CharStr.push_back((char)(n - 10 + 0x41));
	else {
		CharStr = std::to_string(n);
		CharStr.push_back((char)0x7D);
		std::reverse(CharStr.begin(), CharStr.end());
		CharStr.push_back((char)0x7B);
		std::reverse(CharStr.begin(), CharStr.end());
	}
	return CharStr;
}

template<typename Ty>
int Sign(Ty value) {
	if constexpr (std::is_integral_v<Ty>) {
		if (value > 0) {
			return 1;
		}
		else if (value < 0) {
			return -1;
		}
		else {
			return 0;
		}
	}
	else if constexpr (std::is_floating_point_v<Ty>) {
		if (std::signbit(value)) {
			return -1;
		}
		else if (value == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		static_assert(std::is_integral_v<Ty> || std::is_floating_point_v<Ty>,
			"\033[37,41m[Debug Fatal Error]\033[0m not std::is_integral_v or std::is_floating_point_v");
	}
}

export class InfInt
{
public:

	// constructor and destructor

	template<typename tyInt = int>
	InfInt(tyInt _copyInt, int base = 10) requires std::integral<tyInt> : _base(base), _sign(
		Sign(_copyInt)
	)
	{
		_copyInt = _sign == -1 ? -_copyInt : _copyInt;
		_digits = new std::vector<int>{};
		if (_copyInt == 0) { _digits->push_back(0); return; }
		while (_copyInt)
		{
			_digits->push_back(_copyInt % base);
			_copyInt /= base;
		}
	}

	InfInt() : InfInt(10, 0) {};
	InfInt(std::string literal, int base = 10, int sign = 1) : _digits(new std::vector<int>{}), _base(base), _sign(sign)
	{
		if (literal == "0") { _sign = 0; }
		for (auto It = literal.rbegin(); It != literal.rend(); ++It)
		{
			auto ch = *It;
			if ('0' <= ch && ch <= '9') _digits->push_back((int)(ch - '0'));
			else if ('A' <= ch && ch <= 'Z') _digits->push_back((int)(ch - 'A' + 10));
			else throw std::invalid_argument("\033[31m[Error]\033[0m literal char should in union range [0,9] and [A,Z]");
		}
	}

	InfInt(const InfInt& other) // copy_constructor
		: _digits(new std::vector<int>(*other._digits)), _base(other._base), _sign(other._sign) {}

	InfInt(InfInt&& other) noexcept // move_constructor
		: _digits(other._digits), _base(other._base), _sign(other._sign)
	{
		other._digits = nullptr;
	}

	int GetBase() const { return _base; }
	int GetSign() const { return _sign; }
	auto CopyDigitsToVector() const { return _digits; }

	~InfInt()
	{
		delete _digits;
		_digits = nullptr;
	}

	// basic convertors

	template<typename tyInt = int>
		requires std::is_integral_v<tyInt>
	tyInt ToInt()
	{
		tyInt val = 0, pow = 1;
		for (auto bit : *(this->_digits))
		{
			val += pow * bit;
			pow *= this->_base;
		}
		// TODO 这里打算直接用返回值生成 一个新的大数来与原大数比较来判断是否溢出
		// 但在此之前需要写完三路比较运算符opeator<=>的代码
		//if (InfInt(val, this->_base) != *this)
		//	throw std::overflow_error("\033[31m[Error]\033[0m This int type will be overflowed!");
		return val;
	}

	void BaseConvert(int newBase) //TODO：有问题，无法工作，要重写
	{
		if (newBase == this->_sign) return;

		auto* result = new std::vector<int>{};

		int carry = 0;
		for (int digit : *(this->_digits))
		{
			int current = digit + (carry * this->_base);
			result->push_back(current % newBase);
			carry = current / newBase;
		}
		while (carry > 0) {
			result->push_back(carry % newBase);
			carry /= newBase;
		}
		std::reverse(result->begin(), result->end());

		delete this->_digits;
		this->_digits = result;
		result = nullptr;

		this->_base = newBase;
	}

	// transformation operator

	std::shared_ptr<InfInt> Abs() const
	{
		std::shared_ptr<InfInt> absValue = std::make_shared<InfInt>(*this);
		absValue->_sign = 1;
		return absValue;
	}
	std::shared_ptr<InfInt> PosAdd(const InfInt& other) const
	{
		std::shared_ptr<InfInt> sum = std::make_shared<InfInt>(*this);
		if (this->_sign <= 0) return sum;
	}

	// operator
	auto operator<=>(const InfInt& other)
	{
		if ((this->_base == other._base) && (this->_sign == other._sign) && (this->_digits == other._digits)) return true;
		// TODO 没写完
	}

	friend std::ostream& operator<<(std::ostream&, const InfInt&);

private:
	std::vector<int>* _digits;
	int _base;
	int _sign;
};

// output 

export template <typename StreamType>
void outputStream(StreamType& stream, const InfInt& n)
{
	switch (n.GetSign())
	{
	case 0:
		stream << "0";
		return;
	case 1:
		stream << "+";
		break;
	case -1:
		stream << "-";
		break;
	}

	switch (n.GetBase())
	{
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

	if (n.GetBase() != 2 && n.GetBase() != 8 && n.GetBase() != 10 && n.GetBase() != 16)
		stream << (char)(0x28);
	auto d_List = n.CopyDigitsToVector();
	for (auto It = d_List->rbegin(); It != d_List->rend(); ++It)
		stream << DigitToChar(*It);
	if (n.GetBase() != 2 && n.GetBase() != 8 && n.GetBase() != 10 && n.GetBase() != 16)
		stream << (char)(0x29) << "_" << std::format("({})", n.GetBase());
}

export std::ostream& operator<<(std::ostream& os, const InfInt& n)
{
	outputStream(os, n);
	return os;
}

template <>
class std::formatter<InfInt>
{
public:
	constexpr auto parse(auto& ctx)
	{
		return ctx.begin();
	}

	auto format(const InfInt& n, auto& ctx) const
	{
		std::stringstream ss;
		outputStream(ss, n);
		return std::format_to(ctx.out(), "{}", ss.str());
	}
};