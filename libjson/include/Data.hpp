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
		Data(Null n);
		template<std::same_as<bool> B>
		Data(B value) : data{ value } {};
		template<std::same_as<bool> B>
		Data& operator=(const B& value);
		Data(Number numValue);
		Data& operator=(const Number& numValue);
		Data(String strValue);
		Data& operator=(const String& strValue);
		Data(const char* characters);
		Data(Array arValue);
		Data& operator=(const Array& arrValue);
		Data(Object objValue);
		Data& operator=(const Object& objValue);
		bool operator==(Data const&) const;
		std::partial_ordering operator <=>(Data const&) const;
		DataVariant& variant()&;
		DataVariant const& variant() const&;
		DataVariant&& variant()&&;

	private:
		DataVariant data;
	};

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
	
	template<std::same_as<bool> B>
	Data& Data::operator=(const B& value) {
		data = value;
		return *this;
	}
	template<typename T> 
	T& as(Data& data) {
		return std::get<T>(data.variant());
	}
	template<typename T>
	const T& as(Data const& data) {
		return std::get<T>(data.variant());
	}
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