#!/bin/bash

if which clang-format >/dev/null; then

foldersToExclude=(Vendor vendor submodules build cmake-build-debug cmake-build-release xcode visualstudio)
extensionsToProcess=(cpp hpp h)

command="find . \( "
for i in ${!extensionsToProcess[@]}; do
    extension=${extensionsToProcess[$i]}
    command+="-name '*.$extension' "
    if [ "$((${#extensionsToProcess[@]}-1))" -ne "$i" ]; then
        command+=" -or "
    fi
done
command+=" \) -and \( "
for i in ${!foldersToExclude[@]}; do
    folder=${foldersToExclude[$i]}
    command+="-not -path '**/$folder/*' "
    if [ "$((${#foldersToExclude[@]}-1))" -ne "$i" ]; then
        command+=" -and "
    fi
done
command+="\)"

files=$(eval ${command})

count=0
echo "Formatting files:"
for file in $files; do
    # echo $file
    count=$(( count + 1 ))
    clang-format -i "$file"
done
echo "${count} files formatted. Thanks to clang-format ❤️"

else
echo "warning: ClangFormat not installed"
fi
