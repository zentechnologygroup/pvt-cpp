#!/bin/bash

l=`ls *.rb`
for f in $l; do
    cp $f $f.tmp;
    sed -f sed.sed $f.tmp> $f;
done
