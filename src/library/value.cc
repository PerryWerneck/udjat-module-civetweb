/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * Copyright (C) 2021 Perry Werneck <perry.werneck@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

 #include <config.h>
 #include <udjat/civetweb.h>
 #include <udjat/tools/value.h>
 #include <udjat/tools/http/value.h>
 #include <udjat/tools/logger.h>
 #include <iostream>
 #include <iomanip>

 using namespace std;

 namespace Udjat {

	HTTP::Value::Value(Udjat::Value::Type t) : type{t} {
	}

	HTTP::Value::Value(const char *v, Udjat::Value::Type t) : type{t}, value{v} {
	}

	HTTP::Value::~Value() {
		reset(Udjat::Value::Undefined);
	}

	bool HTTP::Value::empty() const noexcept {
		return children.empty();
	}

	HTTP::Value::operator Value::Type() const noexcept {
		return this->type;
	}

	bool HTTP::Value::for_each(const std::function<bool(const char *name, const Udjat::Value &value)> &call) const {

		for(const auto & [key, value] : children)	{
			if(call(key.c_str(),*value)) {
				return true;
			}
		}

		return false;
	}

	Value & HTTP::Value::reset(const Udjat::Value::Type type) {

		if(type != this->type) {

			// Reset value.
			this->type = type;
			this->value.clear();

			// cleanup children.
			for(auto child : children) {
				delete child.second;
			}

			children.clear();

		}

		return *this;

	}

	bool HTTP::Value::isNull() const {
		return this->type == Udjat::Value::Undefined;
	}

	Value & HTTP::Value::operator[](const char *name) {

		reset(Udjat::Value::Object);

		auto search = children.find(name);
		if(search != children.end()) {
			return *search->second;
		}

		Value * rc = new Value();

		children[name] = rc;

		return *rc;
	}

	Value & HTTP::Value::append(const Type type) {
		reset(Udjat::Value::Array);

		Value * rc = new Value(type);
		children[std::to_string((int) children.size()).c_str()] = rc;

		return *rc;
	}

	Value & HTTP::Value::set(const char *value, const Type type) {
		reset(type);
		this->value = value;
		return *this;
	}

	Value & HTTP::Value::set(const Udjat::Value UDJAT_UNUSED(&value)) {
		throw runtime_error("Not implemented");
		return *this;
	}

	Value & HTTP::Value::set(const Udjat::TimeStamp value) {
		if(value)
			return this->set(value.to_string(TIMESTAMP_FORMAT_JSON).c_str(),Udjat::Value::String);
		return this->set("false",Value::Type::Boolean);
	}

	Udjat::Value & HTTP::Value::setFraction(const float fraction) {
		std::stringstream out;
		out.imbue(std::locale("C"));
		out << std::fixed << std::setprecision(2) << (fraction *100);
		return Udjat::Value::set(out.str(),Value::Fraction);
	}

	Value & HTTP::Value::set(const float value) {
		std::stringstream out;
		out.imbue(std::locale("C"));
		out << value;
		return Udjat::Value::set(out.str(),Value::Real);
	}

	Value & HTTP::Value::set(const double value) {
		std::stringstream out;
		out.imbue(std::locale("C"));
		out << value;
		return Udjat::Value::set(out.str(),Value::Real);
	}

	const Udjat::Value & HTTP::Value::get(std::string &value) const {

		if(!children.empty()) {

			// Has children, check standard names.
			static const char *names[] = {
				"summary",
				"value",
				"name"
			};

			for(size_t ix = 0; ix < (sizeof(names)/sizeof(names[0]));ix++) {

				auto child = children.find(names[ix]);
				if(child != children.end()) {
					child->second->get(value);
					if(!value.empty()) {
						return *this;
					}
				}

			}

		}

		value = this->value;
		return *this;
	}


 }
