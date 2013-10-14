#!/bin/sh

cd docutils && \

if [ $1 = "install" ]; then
	echo "Installing DocUtils, SuperUser access is needed..." && \
	echo "Calling sudo..."
	sudo python2.5 setup.py install
else
	echo "Building DocUtils..." && \
	python2.5 setup.py build && \
	mv build ../docutils_build
fi

cd ..

