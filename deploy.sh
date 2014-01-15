#!/bin/bash

# root permissions needed to run this script
# build the application using the generated makefiles - this should create the HttpServer executable

echo "building httpserver...."
cd Build
make
cd ..

# copying the application to /usr/bin 
echo "copying HttpServer to /usr/bin"
cp Build/HttpServer /usr/bin

# copy the config to /etc

echo "copying Http.conf to etc directory..."
cp Http.conf /etc/HttpTwo/


# create the htdocs directory if not already there

if [ -d "/var/www/htdocs" ];
then
  echo "var/www/htdocs exists so not going to create it"
else
  echo "creating directory /var/www/htdocs as document root"
  mkdir /var/www/htdocs
fi

# create the errors directory if not already there

if [ -d "/var/www/htdocs" ];
then
  echo "var/www/errors exists so not going to create it"
else
  echo "creating directory /var/www/errors for error pages"
  mkdir /var/www/htdocs
fi

