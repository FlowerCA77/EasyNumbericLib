export module Base;

export import <string>;
export import <iostream>;
export import <format>;
export import <type_traits>;
export import <algorithm>;
export import <vector>;
export import <limits>;
export import <stdexcept>;

export template <typename Ty>
int Sign(Ty value)
{
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
					  "not std::is_integral_v or std::is_floating_point_v");
	}
}

export std::string DigitToChar(int n)
{
	std::string CharStr;
	if (n < 0) {
		CharStr = DigitToChar(-n);
	}
	else if (n < 10) {
		CharStr.push_back((char)(n + 0x30));
	}
	else if (n < 36) {
		CharStr.push_back((char)(n - 10 + 0x41));
	}
	else {
		CharStr = std::to_string(n);
		CharStr.push_back((char)0x7D);
		std::reverse(CharStr.begin(), CharStr.end());
		CharStr.push_back((char)0x7B);
		std::reverse(CharStr.begin(), CharStr.end());
	}
	return CharStr;
}

// overflowed checker
template <typename T>
bool willAddOverflow(T a, T b)
{
	if (std::is_unsigned<T>::value) {
		return a > std::numeric_limits<T>::max() - b;
	}
	else {
		if (b > 0) {
			return a > std::numeric_limits<T>::max() - b;
		}
		else {
			return a < std::numeric_limits<T>::min() - b;
		}
	}
}

template <typename T>
bool willMulOverflow(T a, T b)
{
	if (a == 0 || b == 0) return false;

	if (std::is_unsigned<T>::value) {
		return a > std::numeric_limits<T>::max() / b;
	}
	else {
		if (a > 0) {
			if (b > 0) {
				return a > std::numeric_limits<T>::max() / b;
			}
			else {
				return b < std::numeric_limits<T>::min() / a;
			}
		}
		else {
			if (b > 0) {
				return a < std::numeric_limits<T>::min() / b;
			}
			else {
				return a != 0 && b < std::numeric_limits<T>::max() / a;
			}
		}
	}
}

export template <typename T>
bool willValAddMulOverflow(T val, T pow, T bit)
{
	T mulResult;
	if (willMulOverflow(pow, bit)) {
		return true;
	}
	else {
		mulResult = pow * bit;
	}

	if (willAddOverflow(val, mulResult)) {
		return true;
	}

	return false;
}