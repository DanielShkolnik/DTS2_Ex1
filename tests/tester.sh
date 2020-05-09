#!/bin/bash
exec 2> /dev/null
testdir="$(pwd)/tests"
indir="${testdir}/in"
outdir="${testdir}/out"
errdir="${testdir}/err"
tempdir="${testdir}/temp"
all_pass="true"
no_memory_leaks="true"
msg1="!!!!ALL Logic Tests Passed!!!!"
msg2="!!!!ALL Memory Tests Passed!!!!"

if [ "$#" -ne 1 ]
then
   echo "usage: \"tester <path to executable>\""
   exit
fi
exe=$1

mkdir -p "${tempdir}"
rm -rf "${tempdir}/*"
rm -rf "${errdir}/*.txt"
temp_infiles="$(find "${indir}" -type f -printf '%p;')"
oldIFS=$IFS
IFS=';'
infiles=($temp_infiles)
IFS=$oldIFS

echo "Running logic and memory tests"
for filename in "${infiles[@]}";do
    file=${filename##*/}
    in_prefix=${file:0:2}
    if [[ $in_prefix -eq "in" ]]
    then
        suffix=${file:2}
        outfile=${outdir}/out${suffix}
        errfile=${errdir}/out${suffix}
        if test -f "$outfile"; then
            ./${exe} < "${indir}/${file}">"${tempdir}/${suffix}" || segfault="true"

            if [[ "${segfault}" == "true" ]]; then
                echo "${file} runtime error or segfault"
                segfault="false"
                all_pass="false"
                
                continue 
            fi
            diff_result=$(diff "${outdir}/out${suffix}" "${tempdir}/${suffix}") 
            if [[ "$diff_result" != "" ]]
            then
                echo "${file} failed"
                all_pass="false"
            else
                echo "${file} succeeded"
                mkdir -p "${errdir}"
                echo "${diff_result}" > "${errdir}/${suffix}"
            fi
            $(valgrind --error-exitcode=1 --leak-check=full "./${exe}" < "${indir}/${file}" >/dev/null )
            if  [ $? -eq 0 ]; then
                echo "${file} no memory leaks"
            else
                echo "${file} memory leak exists"
                no_memory_leaks="false"
            fi
        else
            echo "${file} out file  missing"
        fi
    fi
done


if [[ "$all_pass" == "true" ]];then
    echo $msg1
fi
if [[ "$no_memory_leaks" == "true" ]];then
    echo $msg2
fi

exec 2> /dev/tty
exit
