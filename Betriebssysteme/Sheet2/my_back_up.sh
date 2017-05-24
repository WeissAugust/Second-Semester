#!/bin/bash
# Backup
#

BASEDIR=$(pwd)

BACKUPDIRECTORY=$1
if (( $# == 1 ))
then
	if [ -d "$BACKUPDIRECTORY" ]
	then
		for entry in "$BASEDIR"/*
		do
			cp -R -u * "$BACKUPDIRECTORY"
		done
	else
		printf "the given argument is no directory!\n"
	fi
else
	printf "usage: ./my_back_up.sh backupdirectory\n"
fi
