import InfNumber;

import <iostream>;
import <limits>;

int main()
{
	InfInt n(-240);
	auto m = n.Abs();
	std::cout << std::format("{}, {}, {}", n, *m, InfInt(0)) << std::endl;
	InfInt bin("10001011011001", 2, 1);
	auto hex = bin.baseConvert(16);
	std::cout << std::format("0b10001011011001 = {}", *hex) << std::endl;
	std::cout << std::format("-240 = {} = {}", n, *(n.baseConvert(16))) << std::endl;
	InfInt var("28515155", 10, -1);
	std::cout << std::format("{}, {}, {}", var, *var.Abs(), var.ToInt()) << std::endl;
	InfInt var_24(-451645142, 24);
	std::cout << var_24 << std::endl;
}