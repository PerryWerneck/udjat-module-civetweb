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
  * @brief Implement CivetWeb request.
  */

 #include <config.h>
 #include <udjat/defs.h>
 #include <private/request.h>
 #include <udjat/tools/http/request.h>
 #include <udjat/tools/request.h>
 #include <udjat/tools/intl.h>
 #include <udjat/tools/logger.h>
 #include <udjat/tools/string.h>
 #include <udjat/tools/configuration.h>
 #include <ctype.h>

 #include <civetweb.h>

 using namespace std;

 namespace Udjat {

	namespace CivetWeb {

		Request::Request(struct mg_connection *c)
			: HTTP::Request{mg_get_request_info(c)->local_uri,mg_get_request_info(c)->request_method}, conn{c}, info{mg_get_request_info(c)} {

			debug("Request path set to '",path(),"', type set to ",to_string(((HTTP::Method) *this)));
			debug("Content-type: ",getProperty("Content-Type"));
			debug("Accept: ",getProperty("Accept"));
			debug("Authorization:",getProperty("Authorizatio"));

			// https://github.com/civetweb/civetweb/blob/master/examples/embedded_c/embedded_c.c
			if(!strcasecmp(getProperty("Content-Type").c_str(),"application/x-www-form-urlencoded")) {

				// https://github.com/civetweb/civetweb/blob/master/examples/embedded_c/embedded_c.c#L466
				struct InputParser {

					string name;
					std::map<std::string,std::string> &values;

					static int field_found(const char *key,const char *,char *,size_t ,void *user_data) {
						debug("Field name : '", key , "'");
						((InputParser *) user_data)->name = key;
						return MG_FORM_FIELD_STORAGE_GET;
					}

					static int field_get(const char *, const char *value, size_t valuelen, void *user_data) {
						if(valuelen) {
							debug("   [",string{value,valuelen},"]");
							((InputParser *) user_data)->values[((InputParser *) user_data)->name] += string{value,valuelen};
						}
						return MG_FORM_FIELD_HANDLE_GET;
					}

					static int field_stored(const char *, long long, void *) {
						return 0;
					}

					struct mg_form_data_handler fdh;

					InputParser(std::map<std::string,std::string> &v) : values{v}, fdh{field_found, field_get, field_stored, this} {
					}

				};

				InputParser input{values};

				mg_handle_form_request(c, &input.fdh);

			}

		}

  		const char * Request::c_str() const noexcept {
			return info->local_uri;
  		}

		const char * Request::query(const char *) const {
			return info->query_string;
		}

		String Request::address() const {
			return info->remote_addr;
		}

		MimeType Request::mimetype() const noexcept {

			for(String &value : getProperty("accept").split(",")) {
				auto mime = MimeTypeFactory(value.c_str(),MimeType::custom);
				if(mime != MimeType::custom) {
					return mime;
				}
			}

			return MimeType::custom;
		}

 		String Request::getProperty(const char *name, const char *def) const {

			for(int header = 0; header < info->num_headers; header++) {
				if(!strcasecmp(info->http_headers[header].name,name)) {
					return info->http_headers[header].value;
				}
			}

			return Udjat::Request::getProperty(name,def);
 		}

		String Request::getArgument(const char *name, const char *def) const {

			if(!values.empty()) {
				auto it = values.find(name);
				if(it != values.end()) {
					return it->second;
				}
			}
			return HTTP::Request::getArgument(name,def);

		}

	}

 }
