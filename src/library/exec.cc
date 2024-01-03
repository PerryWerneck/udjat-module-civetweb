/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * Copyright (C) 2023 Perry Werneck <perry.werneck@gmail.com>
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

 /**
  * @brief Implements Request::exec().
  */

 #include <config.h>
 #include <udjat/defs.h>
 #include <udjat/tools/request.h>
 #include <udjat/tools/http/request.h>
 #include <udjat/tools/http/response.h>
 #include <udjat/tools/http/report.h>
 #include <udjat/tools/http/mimetype.h>
 #include <udjat/tools/http/exception.h>
 #include <udjat/tools/worker.h>
 #include <udjat/tools/logger.h>
 #include <string>

 using namespace std;

 namespace Udjat {

	/*
	std::string HTTP::Request::exec(const MimeType mimetype) {

		// https://softwareengineering.stackexchange.com/questions/431218/rest-api-endpoint-returning-detailed-or-summary-data
		size_t output_format = getArgument("output-format","detailed").select("detailed","list","combined",nullptr);

		if(output_format == 1 || mimetype == MimeType::csv) {

			// List
			HTTP::Report response{mimetype};
			debug("Sending list response");
			Udjat::exec(*this,response);
			return response.to_string();

		} else {

			// Detailed
			HTTP::Response response{mimetype};
			debug("Sending detailed response");
			Udjat::exec(*this,response);
			return response.to_string();

		}

		throw system_error(ENOENT,system_category(),"Unknown path");

	}
	*/

	String HTTP::Request::cookie(const char *) const {
		return "";
	}

	String HTTP::Request::address() const {

		String proxy{getProperty("X-Forwarded-For")};
		if(!proxy.empty()) {
			// Get proxy
			auto separator = proxy.find(',');
			if(separator != string::npos) {
				proxy.resize(separator);
			}
			return proxy;
		}

		return "";
	}

 }
