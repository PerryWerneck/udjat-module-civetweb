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

 #include <udjat/module.h>
 #include <udjat/tools/logger.h>
 #include <udjat/worker.h>
 #include <udjat/tools/url.h>
 #include <udjat/factory.h>
 #include <pugixml.hpp>
 #include <unistd.h>
 #include <civetweb.h>
 #include <udjat/civetweb.h>
 #include <udjat/tools/threadpool.h>
 #include <udjat/tools/mainloop.h>

 using namespace std;
 using namespace Udjat;

 #pragma GCC diagnostic ignored "-Wunused-parameter"
 #pragma GCC diagnostic ignored "-Wunused-function"

//---[ Implement ]------------------------------------------------------------------------------------------

static void test_httpd() {

	class Factory : public Udjat::Factory {
	public:
		Factory() : Udjat::Factory("random") {
			cout << "random agent factory was created" << endl;
			srand(time(NULL));
		}

		bool parse(Abstract::Agent &parent, const pugi::xml_node &node) const override {

			class RandomAgent : public Agent<unsigned int> {
			private:
				unsigned int limit = 5;

			public:
				RandomAgent(const pugi::xml_node &node) : Agent<unsigned int>() {
					cout << "Creating random Agent" << endl;
					load(node);
				}

				bool refresh() override {
					set( ((unsigned int) rand()) % limit);
					return true;
				}

			};

			parent.insert(make_shared<RandomAgent>(node));

			return true;

		}

	};

	static Factory factory;

	Udjat::load("./test.xml");
	auto agent = Abstract::Agent::root();

	if(Module::find("information")) {
		cout << "http://localhost:8989/api/1.0/info/modules.xml" << endl;
		cout << "http://localhost:8989/api/1.0/info/workers.xml" << endl;
		cout << "http://localhost:8989/api/1.0/info/factories.xml" << endl;
	}

	for(auto agent : *agent) {
		cout << "http://localhost:8989/api/1.0/agent/" << agent->name() << ".xml" << endl;
	}

	Udjat::MainLoop::getInstance().run();

	agent->deinit();

}

void test_http_get() {



	try {

		Udjat::URL url("http://localhost");
		cout << "Response for " << url << ": " << endl << url.get() << endl;

	} catch(const std::exception &e) {

		cerr << "Exception: " << e.what() << endl;

	}


}

static void test_report() {

	HTTP::Report report;

	report.start("sample","v1","v2","v3",nullptr);

	report 	<< 1
			<< 2
			<< 3
			<< "4"
			<< "5"
			<< "6";

	cout << report.to_string() << endl;
}

int main(int argc, char **argv) {

	Logger::redirect();

	setlocale( LC_ALL, "" );

	Module * module = udjat_module_init();

	/*
	if(URL("http://127.0.0.1/~perry/test.xml").get("/tmp/localhost.html")) {
		cout << endl << endl << "File was updated!" << endl << endl;
	}
	*/

	test_httpd();
	// test_http_get();
	// test_report();

	delete module;

	Module::unload();

	return 0;
}
