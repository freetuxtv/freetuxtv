#!/bin/bash

for fich in channels/*
do
	identify -format "%wx%h" $fich
done
