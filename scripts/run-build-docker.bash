#!/bin/bash

dir_script="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
dir_docker="$dir_script"/../docker

cd "$dir_docker" || exit
docker-compose run --rm pico-gcc-automated
