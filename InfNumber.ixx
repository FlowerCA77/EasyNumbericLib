export module InfNumber;

import <vector>;
import <string>;
import <unordered_map>;

import <iostream>;
import <sstream>;
import <format>;
import <algorithm>;

import <stdexcept>;
import <limits>;
import <memory>;


export std::string DigitToChar(int n) {
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
		if (value > 0) return 1;
		else if (value < 0) return -1;
		else return 0;
	}
	else if constexpr (std::is_floating_point_v<Ty>) {
		if (std::signbit(value)) return -1;
		else if (value == 0) return 0;
		else return 1;
	}
	else {
		static_assert(std::is_integral_v<Ty> || std::is_floating_point_v<Ty>,
			"\033[37,41m[Debug Fatal Error]\033[0m not std::is_integral_v or std::is_floating_point_v");
	}
}

export class InfInt {
public:

	// constructor and destructor

	InfInt(std::vector<int> digits, int base, int sign)
		: _digits(std::make_shared<std::vector<int>>(digits)), _base(base), _sign(sign) {}
	InfInt(std::vector<int>* digits, int base, int sign)
		: _digits(std::shared_ptr<std::vector<int>>(digits)), _base(base), _sign(sign) {
		digits = nullptr;
	}
	InfInt(std::shared_ptr<std::vector<int>> digits, int base, int sign)
		: _digits(digits), _base(base), _sign(sign) {}

	template<typename tyInt = int>
	InfInt(tyInt _copyInt, int base = 10) requires std::integral<tyInt>
		: _digits(std::make_shared<std::vector<int>>()), _base(base), _sign(Sign(_copyInt)) {
		_copyInt = _sign == -1 ? -_copyInt : _copyInt;
		if (_copyInt == 0) { _digits->push_back(0); return; }
		while (_copyInt)
		{
			_digits->push_back(_copyInt % base);
			_copyInt /= base;
		}
	}

	InfInt() : InfInt(0) {};
	InfInt(std::string literal, int base = 10, int sign = 1)
		: _digits(std::make_shared<std::vector<int>>()), _base(base), _sign(sign) {
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
		: _digits(std::make_shared<std::vector<int>>(*other._digits)), _base(other._base), _sign(other._sign) {}

	InfInt(InfInt&& other) noexcept // move_constructor
		: _digits(other._digits), _base(other._base), _sign(other._sign) {
		other._digits = nullptr;
	}

	int GetBase() const { return _base; }
	int GetSign() const { return _sign; }
	auto GetDigits() const {
		if (_digits)
			return std::make_shared<std::vector<int>>(*_digits);
		else
			return std::shared_ptr<std::vector<int>>(nullptr);
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

	/*  base convertor */
	std::shared_ptr<InfInt> baseConvert(int newBase) const;

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
		// TODO 必须要写进制转换，写完才能进一步完善无限大整数的内容
	}

	// operator
	auto operator<=>(const InfInt& other)
	{
		if ((this->_base == other._base) && (this->_sign == other._sign) && (this->_digits == other._digits)) return true;
		// TODO 没写完
	}

	friend std::ostream& operator<<(std::ostream&, const InfInt&);

private:
	std::shared_ptr<std::vector<int>> _digits;
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
	auto d_List = n.GetDigits();
	if (d_List)
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

/*
 * author: ChatGPT 4o/4o-mini
 */
std::unordered_map<int, std::unordered_map<int, std::vector<int>>> bin_hex_linksTable = {
	{2,	 {  {0,		{0, 0, 0, 0}},	{1,		{0, 0, 0, 1}},	{2,		{0, 0, 1, 0}},	{3,		{0, 0, 1, 1}},
			{4, 	{0, 1, 0, 0}},	{5,		{0, 1, 0, 1}},	{6,		{0, 1, 1, 0}},	{7,		{0, 1, 1, 1}},
			{8, 	{1, 0, 0, 0}},	{9,		{1, 0, 0, 1}},	{10,	{1, 0, 1, 0}},	{11,	{1, 0, 1, 1}},
			{12,	{1, 1, 0, 0}},	{13,	{1, 1, 0, 1}},	{14,	{1, 1, 1, 0}},	{15,	{1, 1, 1, 1}}		}},
	{16, {  {0,		{0}},			{1,		{1}},			{2,		{2}},			{3,		{3}},
			{4,		{4}},			{5,		{5}},			{6,		{6}},			{7,		{7}},
			{8,		{8}},			{9,		{9}},			{10,	{10}},			{11,	{11}},
			{12,	{12}},			{13,	{13}},			{14,	{14}},			{15,	{15}}				}}
};

static void baseConvert_common(std::vector<int>& old_digits, std::vector<int>& new_digits, int old_base, int new_base) {
	if (old_digits.empty() || old_base < 2 || new_base < 2) {
		return;
	}

	// Clear new_digits vector
	new_digits.clear();

	// Initialize variables
	int _carry = 0;
	std::vector<int> _temp_digits;
	_temp_digits.reserve(old_digits.size());

	// Convert each digit of old_digits to the new base
	for (int _i = old_digits.size() - 1; _i >= 0; --_i) {
		_carry = old_digits[_i];
		for (size_t _j = 0; _j < _temp_digits.size(); ++_j) {
			int _value = _temp_digits[_j] * old_base + _carry;
			_temp_digits[_j] = _value % new_base;
			_carry = _value / new_base;
		}
		while (_carry > 0) {
			_temp_digits.push_back(_carry % new_base);
			_carry /= new_base;
		}
	}

	// Remove leading zeros
	size_t _first_non_zero = 0;
	while (_first_non_zero < _temp_digits.size() && _temp_digits[_first_non_zero] == 0) {
		++_first_non_zero;
	}

	// Copy the relevant digits to new_digits
	new_digits.assign(_temp_digits.begin() + _first_non_zero, _temp_digits.end());

	// Ensure new_digits is not empty (i.e., for the case of zero)
	if (new_digits.empty()) {
		new_digits.push_back(0);
	}
}

static void baseConvert_downGrade(std::vector<int>& old_digits, std::vector<int>& new_digits, int old_base, int new_base) {
}

static void baseConvert_upGrade(std::vector<int>& old_digits, std::vector<int>& new_digits, int old_base, int new_base) {
}

std::shared_ptr<InfInt> InfInt::baseConvert(int newBase) const {
	auto newInfInt = std::make_shared<InfInt>();
	baseConvert_common(*(this->_digits), *(newInfInt->_digits), this->_base, newBase);
	newInfInt->_sign = this->_sign;
	newInfInt->_base = newBase;
	return newInfInt;
}