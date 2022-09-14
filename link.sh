#!/bin/bash
make Debug
if [ "$?" != "0" ]; then
	exit -1
fi

sudo ln -sf $(readlink -f .bin/Debug/lib*.so.*) /usr/lib64
if [ "$?" != "0" ]; then
	exit -1
fi

sudo mkdir -p /usr/lib64/udjat-modules/1.0
sudo ln -sf $(readlink -f .bin/Debug/udjat-module-civetweb.so) /usr/lib64/udjat-modules/1.0
if [ "$?" != "0" ]; then
	exit -1
fi

cat > .bin/libudjat.pc << EOF
prefix=/usr
exec_prefix=/usr
libdir=$(readlink -f .bin/Debug)
includedir=$(readlink -f ./src/include)

Name: udjat-module-civetweb
Description: UDJAT HTTPD library
Version: 1.0
Libs: -L$(readlink -f .bin/Debug) -ludjathttpd
Libs.private: -ludjat
Cflags: -I$(readlink -f ./src/include)
EOF

sudo ln -sf $(readlink -f .bin/libudjat.pc) /usr/lib64/pkgconfig/udjat-httpd.pc
