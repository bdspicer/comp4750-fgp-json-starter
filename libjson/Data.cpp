#pragma once
#include"Data.hpp"
#include<iomanip>

namespace json {

	// Default Constructor implementation
	Data::Data() : data{} {};

	// Null constructor implementation
	Data::Data(Null n) : data{} {};

	// Global Null variable
	Null null;

	// Null assignment operator
	Data& Data::operator=(const Null& null) {
		data = null;
		return *this;
	}

	// Number constructor implementation
	Data::Data(Number numValue) : data{ numValue } {};

	// Number assignment operator implementation
	Data& Data::operator=(const Number& numValue) {
		data = numValue;
		return *this;
	}

	// String and char* constructor implementations
	Data::Data(String strValue) : data{ strValue } {};
	Data::Data(char const* characters) {
		String str = characters;
		data = str;
	}
	// String assignment operator implementation
	Data& Data::operator=(const String& strValue) {
		data = strValue;
		return *this;
	}

	Data& Data::operator=(const char* charValue) {
		data = charValue;
		return *this;
	}

	// Array constructor implementation
	Data::Data(Array arValue) : data{ arValue } {};

	// Array assignment operator implementation
	Data& Data::operator=(const Array& arValue) {
		data = arValue;
		return *this;
	}

	// Object Constructor implementation
	Data::Data(Object objValue) : data{ objValue } {};
	// Object assignment operator implementation
	Data& Data::operator=(const Object& objValue) {
		data = objValue;
		return *this;
	}

	std::partial_ordering Data::operator <=>(Data const&) const = default;

	DataVariant& Data::variant()& {
		return data;
	}

	// Variant getter implemenations
	DataVariant const& Data::variant() const& {
		return data;
	}
	DataVariant&& Data::variant()&& {
		return std::move(data);
	}

	// Equality operator overload
	bool Data::operator==(Data const&) const = default;


	// Indentation helper functions
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

// Printer manipulator struct
struct Printer {
	std::ostream& out;
	int indent = 0;
	int nestIndent = 0;

	// Null print behavior
	void operator()(const Null& n) {
		out << "null";
	}

	// Boolean print behavior
	void operator()(const Boolean& b) {
		if (b) {
			out << "true";
		}
		else {
			out << "false";
		}
	}

	// Number print behavior
	void operator()(const Number& num) {
		out << num;
	}

	// String print behavior
	void operator()(const String& str) {
		out << std::quoted(str);
	}

	// Array print behavior
	void operator()(const Array& arr) {
		out << '[' << spaceIfIndent(indent);
		if (arr.size() > 0) {
			nestIndent += indent;
			for (int i = 0; i < arr.size(); ++i) {
				if (i != 0) {
					out << ',' << spaceIfIndent(indent);
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
		out << optionalNewLine(indent) << addIndent(nestIndent) << ']';
	}

	// Object print behavior
	void operator()(const Object& obj) {
		int count = 0;
		out << '{';
		if (obj.size() > 0) {
			nestIndent += indent;
			for (std::pair<std::string, json::Data> key : obj) {
				if (count != 0) { out << ','; }
				out << optionalNewLine(indent) << addIndent(nestIndent) << std::quoted(key.first) << ':' << spaceIfIndent(indent);
				std::visit(*this, key.second.variant());
				count++;
			}
		}
		nestIndent -= indent;
		out << optionalNewLine(indent) << addIndent(nestIndent) << "}";
	}
};
	// Ostream operator overloads for Data and Format types
	std::ostream& operator<<(std::ostream& out, const Data& data) {
		Printer printer{ out, 0 };
		std::visit(printer, data.variant());
		return out;
	}
	std::ostream& operator<<(std::ostream& out, const Format& format) {
		Printer printer{ out, format.indent };
		std::visit(printer, format.data.variant());
		return out;
	}
}

