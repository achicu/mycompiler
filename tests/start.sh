#!/bin/sh
mycompiler=../build/Debug/lex

for test in `ls *.txt`
{ 
    echo $test;
    # $mycompiler parser.txt > $test.expected
    $mycompiler parser.txt > output/$test.output
}

cat `ls output/*.output` > output/all.output
cat `ls *.expected` > output/all.expected
diff -s -y output/all.output output/all.expected

echo '\ntest end\n\n';