#!/bin/bash
#de modificat pe git

malloc=$(objdump -d $1 | egrep "call.*malloc" |tr -d " "|cut -f1 -d":")
free=$(objdump -d $1 | egrep "call.*free" |tr -d " "|cut -f1 -d":")
lines=($(objdump -WL $1 | tail -n +8 |tr -s " " | cut -f2 -d" "))
adress=$(objdump -WL $1 | tail -n +8 |tr -s " " | cut -f3 -d" ")

declare -i index
declare -i index_malloc=0
declare -i index_free=0

for i in $adress
do
    declare -i l1=$(printf "%d" $i)
    
    for j in $malloc 
    do
       
        hexa=$(echo "0x$j")
        # echo $hexa

        declare -i l2=$(printf "%d" $hexa)

        declare -i diff=l2-l1


        if [[ diff -eq 10 ]]
        then
            echo "$i ${lines[$index]} ---> malloc " >> mylines
            index_malloc+=1;
            break
        fi
        
    done
    for k in $free
    do
        hexa=$(echo "0x$k")
        #echo $hexa

        declare -i l2=$(printf "%d" $hexa)

        declare -i diff=l2-l1


        if [[ diff -eq 7 ]]
        then
            echo "$i ${lines[$index]} ---> free " >> mylines
            index_free+=1;
            break
        fi
    done
    ((index++))
done

printf "ERROR SUMMARY :\nmalloc/free:$index_malloc allocs, $index_free frees\n"