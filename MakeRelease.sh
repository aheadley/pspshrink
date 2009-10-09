#!/bin/bash

project="pspmpd"
password=$1

if [ "$password" == "" ]
then
	echo "No password for Google Code supplied"
	exit 1
fi

status=`svn status -u | wc -l`
if [ $status -ne 1 ]
then
	echo "svn status is not clean, commit changes first"
	exit 1
fi

#get version number
version=""
version=`cat configure.ac | grep AC_INIT | cut -d[ -f3 | cut -d] -f1`

#build the code
builddir="out-$version"
rm -rf $builddir
mkdir -p $builddir

make -j3 distcheck
if [ $? != 0 ]
then
	echo "Make failed"
	exit 1
fi

cd ..

#upload archive to google
python googlecode_upload.py -s "Release $version" -p $project -u dirk.vdb -w $password -l "Featured,Type-Source,OpSys-Linux" $builddir/$project-$version.tar.gz

#create a tag
svn copy https://$project.googlecode.com/svn/trunk https://$project.googlecode.com/svn/tags/$project-$version -m "Tag of release $version"
