#include <iostream>
#include "Calculation.hpp"
#include <io.h>
#include <fcntl.h>

int main() {

	//system("chcp 1251 ");//включение кирилиццы. способ 1
	//system("cls");//Очистить консоль
	

	// включение кирилиццы. способ 2
	if (!_setmode(_fileno(stdout), _O_U16TEXT)) {
		std::wcerr << L"error _setmode stdout" << std::endl;
		return 0;
	}

	if (!_setmode(_fileno(stdin), _O_U16TEXT)) {
		std::wcerr << L"error _setmode stdin" << std::endl;
		return 0;
	}



	std::wstring vvod;

	while (true)
	{
		wprintf(L"\tВведите выражение: ");

		std::getline(std::wcin, vvod);

		Calculation::removeStrItem(vvod, { L' ' });

		if (vvod == L"close" || vvod == L"закрыть")
			return 0;

		if (vvod == L"clear" || vvod == L"очистить")
		{
			system("cls");
			vvod.clear();
			continue;
		}

		if (vvod == L"саид")
		{
			std::wcout << L"\tсам как леее ?\n";
			vvod.clear();
			continue;
		}

		if (vvod.empty())
		{
			wprintf(L"\tА где ?\n");
			continue;
		}


		if (!Calculation::checkExprErorr(vvod))
		{
			wprintf(L"\tНеправильно введено выражение\n");
			vvod.clear();
			continue;
		}
		wprintf(L"\tОтвет: %lf\n", Calculation::matimaticExpression(vvod));

	}

	return 0;
}

