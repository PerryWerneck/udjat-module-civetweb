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

 /**
  * @brief Implements the default index page.
  *
  */

 #include <config.h>
 #include <udjat/defs.h>
 #include <private/module.h>
 #include <udjat/tools/intl.h>
 #include <udjat/tools/logger.h>
 #include <udjat/tools/http/exception.h>
 #include <udjat/tools/http/response.h>
 #include <udjat/tools/configuration.h>
 #include <private/request.h>

 using namespace std;
 using namespace Udjat;

 int rootWebHandler(struct mg_connection *conn, void UDJAT_UNUSED(*cbdata)) {

	CivetWeb::Connection connection{conn};

	debug("Request path is '",mg_get_request_info(conn)->local_uri,"'");

 	try {

		//
		// Execute API call
		//
		CivetWeb::Request request{mg_get_request_info(conn)};
		HTTP::Response response{(MimeType) request};
		request.exec(response);
		string rsp{response.to_string()};
		return connection.success(to_string((MimeType) request),rsp.c_str(),rsp.size());

	} catch(const HTTP::Exception &error) {

		cerr << "civetweb\t" << error.what() << endl;
		return connection.response(error);

	} catch(const system_error &error) {

		cerr << "civetweb\t" << error.what() << endl;
		return connection.response(error);

	} catch(const exception &error) {

		cerr << "civetweb\t" << error.what() << endl;
		return connection.response(error);

	} catch(...) {

		cerr << "civetweb\tUnexpected error" << endl;
		connection.failed(500, "Unexpected error");
		return 500;

	}

	return 500;
 }
