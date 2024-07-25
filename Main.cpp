import InfNumber;

import <iostream>;
import <limits>;

int main()
{
	InfInt n(-240);
	auto m = n.Abs();
	std::cout << std::format("{}, {}", n, *m) << std::endl;
	n.BaseConvert(16);
	std::cout << n << std::endl;
	InfInt var("28515155", 10, -1);
	std::cout << std::format("{}, {}, {}", var, *var.Abs(), var.ToInt()) << std::endl;
	InfInt var_24(-451645142, 24);
	std::cout << var_24 << std::endl;
}