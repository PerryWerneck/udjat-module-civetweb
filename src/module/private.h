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

 #pragma once

 #include <config.h>
 #include <udjat/defs.h>
 #include <cstring>
 #include <string>
 #include <stdexcept>
 #include <civetweb.h>
 #include <iostream>

 using namespace Udjat;
 using namespace std;

 /// @brief Handler for API requests.
 int apiWebHandler(struct mg_connection *conn, void *cbdata);

 /// @brief Handler for application info.
 int infoWebHandler(struct mg_connection *conn, void *cbdata);

