#include"Data.hpp"
#include<iomanip>

namespace json {
	Data::Data() : data{} {};
	Data::Data(Null n) : data{} {};
	template<std::same_as<bool> B>
	Data::Data(B value) : data{ value } {};
	template<std::same_as<bool> B>
	void Data::operator=(const B& value) {
		data = value;
	}
	Data::Data(Number numValue) : data{ numValue } {};
	void Data::operator=(const Number& numValue) {
		data = numValue;
	}
	Data::Data(char const* characters) {
		String str = characters;
		data = str;
	}
	Data::Data(String strValue) : data{ strValue } {};
	void Data::operator=(const String& strValue) {
		data = strValue;
	}
	Data::Data(Array arValue) : data{ arValue } {};
	void Data::operator=(const Array& arValue) {
		data = arValue;
	}
	Data::Data(Object objValue) : data{ objValue } {};
	void Data::operator=(const Object& objValue) {
		data = objValue;
	}
	std::partial_ordering Data::operator <=>(Data const&) const = default;

	DataVariant& Data::variant()& {
		return data;
	}
	DataVariant const& Data::variant() const& {
		return data;
	}
	DataVariant&& Data::variant()&& {
		return std::move(data);
	}

	bool Data::operator==(Data const&) const = default;

	bool anyIndent(int indentAmount) { return indentAmount > 0; }

	std::string optionalNewLine(const int& indent) {
		std::string optionalNewLineChar = "";
		if (indent > 0) {
			optionalNewLineChar = "\n";
		}
		return optionalNewLineChar;
	}

	std::string addIndent(int indentAmount) {
		std::string indent = "";
		if (anyIndent(indentAmount)) {
			for (int i = 0; i < indentAmount; ++i) {
				char space = ' ';
				indent.push_back(space);
			}
		}
		return indent;
	}

	std::string spaceIfIndent(int indentAmount) {
		std::string indentSpacing{ "" };
		if (anyIndent(indentAmount)) {
			indentSpacing = " ";
		}
		return indentSpacing;
	}

	struct Printer {
		std::ostream& out;
		int indent = 0;
		int nestIndent = indent;
		void operator()(const Null& n) {

			out << "null";
		}
		void operator()(const Boolean& b) {
			if (b) {
				out << "true";
			}
			else {
				out << "false";
			}
		}
		void operator()(const Number& n) {
			out << n;
		}
		void operator()(const String& str) {
			out << std::quoted(str);
		}
		void operator()(const Array& arr) {
			out << '[' << spaceIfIndent(indent);
			if (arr.size() > 0) {
				nestIndent += indent;
				for (int i = 0; i < arr.size(); ++i) {
					if (i != 0) {
						out << ',' << spaceIfIndent(nestIndent);
					}

					out << optionalNewLine(indent) << addIndent(nestIndent);
					if (is<Array>(arr[i])) {
						std::visit(*this, arr[i].variant());
					}
					else {
						std::visit(*this, arr[i].variant());
					}
				}
			}
			nestIndent -= indent;
			out << optionalNewLine(nestIndent) << addIndent(nestIndent) << ']';
		}
		void operator()(const Object& obj) {
			int count = 0;
			out << '{';
			if (obj.size() > 0) {
				for (std::pair<std::string, json::Data> key : obj) {
					if (count != 0) { out << ','; }
					out << optionalNewLine(indent) << addIndent(indent) << std::quoted(key.first) << ':' << spaceIfIndent(indent);
					std::visit(*this, key.second.variant());
					count++;
				}
			}
			out << optionalNewLine(indent) << "}";
		}
	};

}

//struct Format {
//	const Data& data;
//	int indent = 0;
//};
//
//Format format(const Data& data, int indent) {
//	Format formatData{ data, indent };
//	return formatData;
//}

