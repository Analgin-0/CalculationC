#include "Calculation.hpp"



/////////////////////////////PUBLIC///////////////////////////
/////////////////////Функции для выражения 
double Calculation::matimaticExpression(std::wstring exsp) {

	std::vector<int> index_exsp_skobka;
	std::vector<std::vector<std::wstring>> arr_skobka;
	std::vector<std::wstring> vec_res;

	int count_ = 0;

	if (exsp.find(L'(') != -1) {

		addMultiplicationSkobka(exsp, exsp);

		sortingArrSkobka(arr_skobka, exsp);

		// вычесление выражений из скобок
		for (int i = 0; i < arr_skobka.size(); ++i) {
			for (int f = 0; f < arr_skobka[i].size(); ++f) {
				std::wstring res;
				matSkobkaExsp(arr_skobka[i][f], res);
				arr_skobka[i][f] = res;
			}
		}

		//
		for (int i = 0; i < arr_skobka.size(); ++i) {
			std::wstring res_add;
			for (int f = 0; f < arr_skobka[i].size(); ++f)
				res_add += arr_skobka[i][f];
			vec_res.push_back(res_add);
			res_add.clear();
		}

		//
		for (int i = 0; i < arr_skobka.size(); ++i)
			arr_skobka[i].resize(1);

		//
		for (int i = 0; i < arr_skobka.size(); ++i)
			arr_skobka[i][0] = vec_res[i];

		//
		for (int i = 0; i < arr_skobka.size(); ++i)
			matExsp(arr_skobka[i][0], arr_skobka[i][0]);

		//удаление 
		skobkaClearExpr(exsp, index_exsp_skobka);

		//
		for (int i = 0; i < index_exsp_skobka.size(); i++) {
			std::wstring add = arr_skobka[i][0];

			exsp.insert((i != 0 ? index_exsp_skobka[i] + count_ : index_exsp_skobka[i] + 1), add);
			count_ += static_cast<int>(add.size());
		}

		//удаление скобок из выражения
		removeStrItem(exsp, { L'(', L')' });

		//
		matExsp(exsp, exsp);
	}
	else
		matExsp(exsp, exsp);
	return std::stod(exsp);
}

double Calculation::matimaticExpression(const wchar_t* exsp) {
	return matimaticExpression(std::wstring(exsp));
}



////////////////////////////////PRIVATE

/// <summary>
/// 
/// </summary>
/// <param name="exsp">Выражение</param>
/// <param name="result">Ссылка на строку для записи результата</param>
void Calculation::matExsp(const std::wstring exsp, std::wstring& result) {

	std::wstring temporarily;
	std::wstring strTypeOp;

	std::vector<double> numbers;
	std::vector<wchar_t> operations;

	bool isPrioritet = false;

	auto addNumber = [&temporarily, &numbers]() {
		if (temporarily.size() != 0) {
			numbers.push_back(stod(temporarily));
			temporarily.clear();
		}

	};

	auto whetherToMultiply = [&isPrioritet, &numbers, &strTypeOp]() {
		if (isPrioritet) {
			double value_1 = numbers[numbers.size() - 2]; // берём пред последний элемент массива
			double value_2 = numbers[numbers.size() - 1]; // берём последний элемент массива

			numbers.pop_back();
			numbers.pop_back(); // удаляем два последних элемента
			//вычесление и добавление в массив
			switch (strTypeOp[0]) {
			case L'*':
				numbers.push_back(value_1 * value_2);
				break;
			case L'/':
				numbers.push_back(value_1 / value_2);
				break;
			default:
				break;
			}
			strTypeOp.clear();
			isPrioritet = false;
		}
	};


	for (int i = 0; i < exsp.size(); i++) {
		char item = exsp[i];

		if (isdigit(item)) {
			temporarily.push_back(item);
			continue;
		}

		switch (item) {
		case L'+':
			addNumber();//
			whetherToMultiply();//
			operations.push_back(item);//
			continue;
		case L'-':
			if (i == 0) {
				temporarily.insert(temporarily.begin(), '-');
				continue;
			}
			addNumber();
			whetherToMultiply();
			operations.push_back(item);
			continue;
		case L'*':
			addNumber();
			whetherToMultiply();
			strTypeOp = L'*';
			isPrioritet = true;
			continue;

		case L'/':
			addNumber();
			whetherToMultiply();
			strTypeOp = L'/';
			isPrioritet = true;
			continue;
		case L'.':
		case L',':
			temporarily.push_back(L'.');
			continue;
		case L' ':
			continue;
		default:
			break;
		}

	}

	addNumber();
	whetherToMultiply();
	matimaticRecurse(numbers, operations);

	result = convertDoubleToString(numbers.front());
}

/// <summary>
/// 
/// </summary>
/// <param name="numbers">:Массив с числами</param>
/// <param name="operations">:Массив с операциями</param>
void Calculation::matimaticRecurse(std::vector<double>& numbers, std::vector<wchar_t>& operations) {

	if (numbers.size() == 1 || operations.empty()) return;

	double value_1 = numbers[0];
	numbers.erase(numbers.begin());

	double value_2 = numbers[0];
	numbers.erase(numbers.begin());

	char operation = operations[0];
	operations.erase(operations.begin());

	switch (operation) {
	case L'+':
		numbers.insert(numbers.begin(), value_1 + value_2);
		break;
	case L'-':
		numbers.insert(numbers.begin(), value_1 - value_2);
		break;
	default:
		break;
	}

	if (numbers.size() > 1) matimaticRecurse(numbers, operations);
}


/////////////////////////////////////////Функции для скобок

void Calculation::matSkobkaExsp(const std::wstring expression, std::wstring& result_) {

	std::wstring str_op_skobka;
	std::wstring str_type_op;
	std::wstring temporarily_;

	std::vector<double> numbers_;
	std::vector<wchar_t> operations_;
	OpLocation location = OpLocation::none;
	bool prioritet = false;

	auto add_number = [&temporarily_, &numbers_]() {
		if (!temporarily_.empty()) {
			numbers_.push_back(stod(temporarily_));
			temporarily_.clear();
		}
	};

	auto whether_to_multiply = [&prioritet, &numbers_, &str_type_op]() {
		if (prioritet) {
			double value_1 = numbers_[numbers_.size() - 2]; // берём пред последний элемент массива
			double value_2 = numbers_[numbers_.size() - 1]; // берём последний элемент массива

			numbers_.pop_back();
			numbers_.pop_back(); // удаляем два последних элемента
			//вычесление и добавление в массив
			switch (str_type_op[0]) {
			case L'*':
				numbers_.push_back(value_1 * value_2);
				break;
			case L'/':
				numbers_.push_back(value_1 / value_2);
				break;
			default:
				break;
			}
			str_type_op.clear();
			prioritet = false;
		}
	};

	auto add_location_op = [&expression, &location, &str_op_skobka, &temporarily_](int i, char typeOp) {
		if (i == 0) {
			location = OpLocation::begin;
			str_op_skobka.insert(str_op_skobka.begin(), typeOp);
			//test
			return true;
		}
		else if (i == last_index(expression)) {
			location = OpLocation::end;
			str_op_skobka.push_back(typeOp);
			return true;
		}
		return false;
	};

	for (int i = 0; i < expression.size(); ++i) {
		const char item = expression[i];

		if (isdigit(item)) {
			temporarily_.push_back(item);
			continue;
		}

		switch (item) {
		case L'+': {
			if (add_location_op(i, L'+'))
				continue;
			add_number();
			whether_to_multiply();
			operations_.push_back(item);
			continue;
		}

		case L'-': {
			if (add_location_op(i, L'-'))
				continue;
			add_number();
			whether_to_multiply();
			operations_.push_back(item);
			continue;
		}

		case L'*': {
			if (add_location_op(i, L'*'))
				continue;
			add_number();
			whether_to_multiply();
			str_type_op = L'*';
			prioritet = true;
			continue;
		}

		case L'/': {
			if (add_location_op(i, L'/'))
				continue;
			add_number();
			whether_to_multiply();
			str_type_op = L'/';
			prioritet = true;
		}
				 continue;
		case L'.':
		case L',':
			temporarily_.push_back(L',');
			continue;
		default:
			break;

		}

	}

	add_number();
	whether_to_multiply();

	if (str_op_skobka.size() == 2)
		location = OpLocation::on_both_sides;

	matimaticRecurse(numbers_, operations_);


	switch (location) {
	case OpLocation::none:
		result_ = Calculation::convertDoubleToString(numbers_.front());
		return;
	case OpLocation::begin:
		result_ = str_op_skobka + Calculation::convertDoubleToString(numbers_.front());
		return;
	case OpLocation::end:
		result_ = Calculation::convertDoubleToString(numbers_.front()) + str_op_skobka;
		return;
	case OpLocation::on_both_sides:
		result_ = str_op_skobka[0] + Calculation::convertDoubleToString(numbers_.front()) + str_op_skobka[1];
		return;
	}


}

void Calculation::skobkaClearExpr(std::wstring& str, std::vector<int>& vecIndex) {

	int count = 0;
	std::wstring result;

	for (auto& element : str) {
		if (element == L')')
			count--;

		if (count == 0)
			result += element;

		if (element == L'(')
			count++;
	}

	for (int i = 0; i < result.size(); i++)
		if (result[i] == L'(')
			vecIndex.push_back(i);

	str = result;
}

void Calculation::sortingArrSkobka(std::vector<std::vector<std::wstring>>& result_, std::wstring exsp) {

	std::wstring time;
	std::vector<std::wstring> add;
	int count = 0;

	for (auto& element : exsp) {
		if (count != 0 && element != L'(' && element != L')')
			time.push_back(element);

		if (element == L'(') {
			if (!time.empty()) {
				add.push_back(time);
				time.clear();
			}
			count++;
			continue;
		}

		if (element == L')') {
			if (!time.empty()) {
				add.push_back(time);
				time.clear();
			}
			if (count - 1 == 0) {
				result_.push_back(add);
				add.clear();
			}
			count--;
			continue;
		}
	}

}

void Calculation::addMultiplicationSkobka(const std::wstring exprSkobka, std::wstring& result) {

	int number_skobka = 0;
	int add = 0;

	result = exprSkobka;

	for (int i = 0; i < exprSkobka.size(); i++) {
		bool condition = exprSkobka[i] == L'(' && number_skobka <= 1 && i != 0;

		if (condition && isdigit(exprSkobka[i - 1]) || condition && exprSkobka[i - 1] == L')') {
			result.insert(result.begin() + i + add, L'*');
			add++;
			number_skobka++;
			continue;
		}

		if (exprSkobka[i] == L'(') {
			number_skobka++;
			continue;
		}

		if (exprSkobka[i] == L')')
			number_skobka--;
	}

}


//////////////////////////////////////Вспомогательные функции////////

std::wstring Calculation::convertDoubleToString(const double val) {
	return std::to_wstring(val);
}

template<typename T>
int Calculation::last_index(const std::vector<T>& arr) {
	return arr.size() - 1;
}

int Calculation::last_index(const std::wstring& str) {
	return int(str.size()) - 1;
}


void Calculation::removeStrItem(std::wstring& str, const std::initializer_list<wchar_t> remove_items) {
	for (auto& element : remove_items)
		while (true) {
			size_t i = str.find(element);
			if (i == -1)
				break;
			str.erase(str.begin() + i);
		}

}


bool Calculation::checkExprErorr(const std::wstring expr) {

	int countNumber = 0;
	int countOperations = 0;

	int beginSkobka = 0;
	int endSkobka = 0;
	bool skobka;

	wchar_t operations_[] = {
			L'+', L'-', L'*', L'/'
	};
	constexpr size_t operationsSize = sizeof(operations_) / sizeof(char);


	for (int i = 0; i < expr.size(); ++i) {
		if (expr[i] > (L'0' - 1) && expr[i] < (L'9' + 1))
			countNumber++;

		for (int j = 0; j < operationsSize; ++j) {

			if (expr[i] == L'-' && i == 0)
				break;
			if (operations_[j] == expr[i]) {
				countOperations++;
				break;
			}
		}

	}

	// проверяет правильно ли расставлены скобки
	for (wchar_t ch_ : expr) {
		if (ch_ == L'(') {
			beginSkobka++;
			continue;
		}
		if (ch_ == L')') {
			endSkobka++;
			continue;
		}
	}
	skobka = (beginSkobka + endSkobka) % 2 == 0 || beginSkobka == 0 && endSkobka == 0;


	return ((countNumber - 1) == countOperations) && skobka;
}