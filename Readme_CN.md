# EasyNumbericLib

[![License](https://img.shields.io/badge/English-orange.svg)](./README.md)
[![License](https://img.shields.io/badge/%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87-purple.svg)](./Readme_CN.md)

## ����

һ�����׵���ֵ����⣨ûд�꣡��

��`Readme`�� [shields.io](https://shields.io/) �ṩ�Ű�֧�֡�

![| Recommand: Use Visual Studio (MSVC) to edit source code](./iamworking.png)

## Ŀ¼

- [��װ](#��װ)
- [ʹ��](#ʹ��)
- [����](#����)
- [���֤](#���֤)
- [��ϵ](#��ϵ)

## ��װ

��ȫ�� `ifc` �ļ� (δ�������ߵ� release ��) ������Ĺ���Ŀ¼����֪�ļ��У��� `MSVC` ��������Ŀ���ԣ�
```
�������� -> C/C++ -> ���� -> ���Ӱ���Ŀ¼
```
`MSVC` ���Զ������ҵ� `ifc` �ļ�����Ȼ��Ҳ�����ֶ�����
```sh
cl /std:c++20 yourcode.cpp /module:import binaryfiles.ifc
```

��Ȼ��Ҳ����ֱ�� clone Դ��������, ���б��� `ixx` �ļ�

������� Linux �� POSIX ��ϵͳ������ (�� `gcc` / `clang` ��) �����԰� `ixx` Դ�����׺������ `cpp` �����룬��Ȼ�������Ҫ�����޸Ĵ����������� `MSVC` �����������ݡ�

### �Ⱦ�����

����Ŀʹ��ģ���д�������õ���Լ����`std::format` �����ݣ���������Ҫ��֤��Ļ��������� `C++20` ��׼���ϡ������� `MSVC` ��ʹ�� `C++20` ��Ȼ������ģ����б��롣

## ʹ��

��ĿԴ������ `Main.cpp` ����һ�����ӡ�
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

## ����

## ���֤

[![License](https://img.shields.io/badge/License-GPL%203.0-blue.svg)](./LICENSE.txt)

This project is licensed under the GPL-3.0 License - see the [LICENSE](./LICENSE.txt) file for details.

## ��ϵ

��ӭ��ϵ�ң�
- Github : [FlowerCA77](https://github.com/FlowerCA77/)
- eMail : `ca538468142@outlook.com`