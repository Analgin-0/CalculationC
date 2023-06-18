#pragma once
#include <vector>
#include <string>
#include <initializer_list>


class Calculation
{

	enum class OpLocation
	{
		none, // нету
		begin, // спереди
		end, // в конце
		on_both_sides // с двух строн
	};

	typedef std::wstring string_;
	

	static void matExsp(const std::wstring exsp, std::wstring& result);// вычестляет выражение и записывает его в result

	static void matimaticRecurse(std::vector<double>& numbers, std::vector<wchar_t>& operations);//берёт из массива 2 числа

	static void matSkobkaExsp(const std::wstring expression, std::wstring& result_);

	static  void skobkaClearExpr(std::wstring& str, std::vector<int>& vecIndex); // удаляет всё что есть в скобках

	static  void sortingArrSkobka(std::vector<std::vector<std::wstring>>& result_, std::wstring exsp);

	// добавляет перед скобкой знак умножения если перед ним ничего нету
	static void addMultiplicationSkobka(std::wstring exprSkobka, std::wstring& result);

	static std::wstring convertDoubleToString(const double val); // конвертирует число в строку

	template<typename T>int last_index(const std::vector<T>& arr); // Length - 1 || size() -1 

	static int last_index(const std::wstring& str); // Length - 1 || size() -1 

	

public:

	static double matimaticExpression(std::wstring exsp);
	
	static double matimaticExpression(const wchar_t* exsp);
	
	static bool checkExprErorr(const std::wstring expr);
    static void removeStrItem(std::wstring& str, std::initializer_list<wchar_t> remove_item);


};

