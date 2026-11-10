#pragma once

#include<iostream>
#include<string>
#include<concepts>
#include<variant>
#include<vector>
#include<map>

namespace json {
	class Data;

	using Null = std::monostate;
	extern Null null;
	using Boolean = bool;
	using Number = double;
	using String = std::string;
	using Array = std::vector<json::Data>;
	using Object = std::map<std::string, json::Data>;
	using DataVariant = std::variant<Null, Boolean, Number, String, Array, Object>;

	class Data {
	public:
		Data();

		// Null Constructor
		Data(Null n);

		// Null assignment operator
		Data& operator=(const Null& null);

		// Boolean Constructor
		template<std::same_as<bool> B>
		Data(B value) : data{ value } {};

		// Boolean assignment operator
		template<std::same_as<bool> B>
		Data& operator=(const B& value);
		
		// Number constructor
		Data(Number numValue);
		Data& operator=(const Number& numValue);

		// String and char* constructors
		Data(String strValue);
		Data(const char* characters);

		// String assignment operator
		Data& operator=(const String& strValue);

		// Array constructor
		Data(Array arValue);
		Data& operator=(const Array& arrValue);

		// Object Constructor
		Data(Object objValue);

		// Object assignment operator
		Data& operator=(const Object& objValue);

		// Variant getters
		DataVariant& variant()&;
		DataVariant const& variant() const&;
		DataVariant&& variant()&&;

		// Equality Operator
		bool operator==(Data const&) const;

		// Spaceship operator
		std::partial_ordering operator <=>(Data const&) const;

	private:
		DataVariant data;
	};

	// Format struct and function
	struct Format {
		const Data& data;
		int indent;
	};

	inline json::Format format(const Data& data, int indent) {
		Format formatData{ data, indent };
		return formatData;
	}

	std::ostream& operator<<(std::ostream& out, Format const& format);
	std::ostream& operator<<(std::ostream& out, const Data& data);
	
	// Bool assignment operator implementation 
	template<std::same_as<bool> B>
	Data& Data::operator=(const B& value) {
		data = value;
		return *this;
	}
	
	// "As" function implementations
	template<typename T> 
	T& as(Data& data) {
		return std::get<T>(data.variant());
	}
	template<typename T>
	const T& as(Data const& data) {
		return std::get<T>(data.variant());
	}
	template<typename T>
	T&& as(Data&& data) {
		return std::get<T>(std::move(data.variant()));
	}

	// "Is" function implementations
	template<typename T>
	bool is(Data const& data) {
		return std::holds_alternative<T>(data.variant());
	}
	template<typename T>
	bool is(Data& data) {
		return std::holds_alternative<T>(data.variant());
	}
	template<typename T>
	bool is(Data&& data) {
		return std::holds_alternative<T>(std::move(data.variant()));
	}
}