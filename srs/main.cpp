#include <iostream>
#include "Calculation.hpp"
#include <io.h>
#include <fcntl.h>

int main() {

	//system("chcp 1251 ");//��������� ���������. ������ 1
	//system("cls");//�������� �������
	

	// ��������� ���������. ������ 2
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
		wprintf(L"\t������� ���������: ");

		std::getline(std::wcin, vvod);

		Calculation::removeStrItem(vvod, { L' ' });

		if (vvod == L"close" || vvod == L"�������")
			return 0;

		if (vvod == L"clear" || vvod == L"��������")
		{
			system("cls");
			vvod.clear();
			continue;
		}

		if (vvod == L"����")
		{
			std::wcout << L"\t��� ��� ���� ?\n";
			vvod.clear();
			continue;
		}

		if (vvod.empty())
		{
			wprintf(L"\t� ��� ?\n");
			continue;
		}


		if (!Calculation::checkExprErorr(vvod))
		{
			wprintf(L"\t����������� ������� ���������\n");
			vvod.clear();
			continue;
		}
		wprintf(L"\t�����: %lf\n", Calculation::matimaticExpression(vvod));

	}

	return 0;
}

