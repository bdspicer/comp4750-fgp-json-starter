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
	Null null;
	using Boolean = bool;
	using Number = double;
	using String = std::string;
	using Array = std::vector<json::Data>;
	using Object = std::map<std::string, json::Data>;
	using DataVariant = std::variant<Null, Boolean, Number, String, Array, Object>;

	class Data {
	public:
		Data();
		Data(Null n);
		template<std::same_as<bool> B>
		Data(B value);
		template<std::same_as<bool> B>
		void operator=(const B& value);
		Data(Number numValue);
		void operator=(const Number& numValue);
		Data(String strValue);
		void operator=(const String& strValue);
		Data(const char* characters);
		Data(Array arValue);
		void operator=(const Array& arrValue);
		Data(Object objValue);
		void operator=(const Object& objValue);
		bool operator==(Data const&) const;
		std::partial_ordering operator <=>(Data const&) const;
		DataVariant& variant()&;
		DataVariant const& variant() const&;
		DataVariant&& variant()&&;

	private:
		DataVariant data;
	};

	std::ostream& operator<<(std::ostream& out, const Data& data);
	template<typename T> 
	T& as(Data& data) {
		return std::get<T>(data.variant());
	}
	template<typename T>
	const T& as(Data const& data) {
		return std::get<T>(data.variant());
	}
	/*template<typename T>
	bool is(Data const& data) {
		return std::holds_alternative<T>(data.variant());
	}*/
	template<typename T>
	bool is(Data& data) {
		return std::holds_alternative<T>(data.variant());
	}
	template<typename T>
	bool is(Data&& data) {
		return std::holds_alternative<T>(std::move(data.variant()));
	}
}