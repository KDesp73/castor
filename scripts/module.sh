#!/usr/bin/env bash

NAME=$1
if [ -z "$NAME" ]; then
  echo "Usage: $0 <module_name>"
  exit 1
fi

mkdir -p engine/src/"$NAME"

touch engine/src/"$NAME"/"$NAME".c
touch engine/include/"$NAME".h

echo "#ifndef CASTOR_${NAME^^}_H" >> engine/include/"$NAME".h
echo "#define CASTOR_${NAME^^}_H" >> engine/include/"$NAME".h
echo "" >> engine/include/"$NAME".h
echo "// $NAME module" >> engine/include/"$NAME".h
echo "" >> engine/include/"$NAME".h
echo "#endif" >> engine/include/"$NAME".h

echo "mkdir -p \$(BUILD_DIR)/$NAME" >> engine/Makefile

echo "[Castor] 🧱 Module '$NAME' created"

