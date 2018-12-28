#!/bin/bash
set -e
set -u

testBranch=$1;

gitrepo="$(git rev-parse --show-toplevel)"

masterdir=/tmp/mace-master
newdir="/tmp/mace-$testBranch"

rm -rf /tmp/mace-master;
rm -rf /tmp/mace-$testBranch;

git clone "$gitrepo" "$masterdir"
git clone "$gitrepo" "$newdir";

pushd "$masterdir"
git checkout master
popd

pushd "$newdir"
git checkout "$testBranch"
popd

for dir in "$masterdir" "$newdir"; do
    pushd "$dir";
    autoreconf -i;
    ./configure
    make
done

cutechess-cli \
    -engine name=mace-new proto=uci  cmd="$newdir"/src/mace \
    -engine name=mace-master proto=uci cmd="$masterdir"/src/mace \
    -openings file="$masterdir"/etc/silversuite.pgn \
    -concurrency 3 -ratinginterval 10 -games 500 -pgnout games.pgn \
    -each  tc=100+0.01
