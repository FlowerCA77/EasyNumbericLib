# EasyNumbericLib

[![License](https://img.shields.io/badge/English-orange.svg)](./README.md)
[![License](https://img.shields.io/badge/ÖÐÎÄ-green.svg)](./README_CN.md)

About
A simple numerical calculation library (not yet completed)

## TOC

- [Install](#Install)
- [Usage](#Usage)
- [Contribute](#Contribute)
- [License](#License)
- [Contact](#Contact)

## Install

To install EasyNumericLib, you should put all `ifc` files (which will be released in the future) into your project directory or known folder, and set project properties in `MSVC`:
```
Configuration Properties -> C/C++ -> General -> Additional Include Directories
```
`MSVC` will automatically compile and find the `ifc` file, but you can also manually compile it
```sh
cl /std:c++20 yourcode.cpp /module:import binaryfiles.ifc
```

### Prerequisite

This project is written using modules and incorporates constraints, `std::format`, and other content, so you need to ensure that your environment is at least compliant with the `C++20` standard. It is recommended to use `C++20` in `MSVC` and enable module compilation.

## Usage

File `Main.cpp` in project src repo is an example:
```c++
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
```

## Contribute

## License

![License](https://img.shields.io/badge/License-GPL%203.0-blue.svg)

This project is licensed under the GPL-3.0 License - see the LICENSE(./LICENSE.txt) file for details.

## Contact

Welcome!
- Github : [FlowerCA77](https://github.com/FlowerCA77/)
- eMail : `ca538468142@outlook.com`
