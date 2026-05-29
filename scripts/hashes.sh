#!/bin/bash

INPUT_FILE="$1"
HASH_FILE="${INPUT_FILE}.hashes"

if [[ -z "$INPUT_FILE" || ! -f "$INPUT_FILE" ]]; then
  echo "Usage: $0 <input_file>"
  echo "Error: Input file not specified or does not exist."
  exit 1
fi

if [[ "$(uname -s)" == "Darwin" ]]; then
  SHA256=$(shasum -a 256 "$INPUT_FILE" | awk '{print $1}')
  SHA1=$(shasum -a 1 "$INPUT_FILE" | awk '{print $1}')
  MD5=$(md5 -q "$INPUT_FILE")
else
  SHA256=$(sha256sum "$INPUT_FILE" | awk '{print $1}')
  SHA1=$(sha1sum "$INPUT_FILE" | awk '{print $1}')
  MD5=$(md5sum "$INPUT_FILE" | awk '{print $1}')
fi

{
  echo "File: $INPUT_FILE"
  echo "SHA256: $SHA256"
  echo "SHA1:   $SHA1"
  echo "MD5:    $MD5"
} > "$HASH_FILE"

echo "Hashes written to $HASH_FILE"
