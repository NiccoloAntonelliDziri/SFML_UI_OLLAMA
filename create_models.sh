#!/bin/bash

directory_path="modelfiles"
for file in "$directory_path"/*; do
    if [ -f "$file" ]; then
        echo "Creating model from file: $file"
        # Remove the directory path from the file name
        name=$(basename "$file")
        ollama create $name -f $file
    fi
done

