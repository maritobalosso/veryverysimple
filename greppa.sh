#!/bin/bash
cat ../test.txt | while read LINE ; do
        # This is where LINE is the second argument:
	grep ",$LINE:" similarity.txt | sed s/",$LINE:"/:/ > tmp.txta
        # This is where LINE is the first argument:
	grep "^$LINE," similarity.txt | sed s/"^$LINE,"// >> tmp.txta
        ./due $LINE >> resi
done
