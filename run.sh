#!/bin/bash

# !!! This file should always be located in under root directory !!!

# Configuring files and directories used by the app
CURRENT_FILE=$(readlink -f "$0")
echo "Configuring environment with: $CURRENT_FILE"

export ROOT_DIR=$(dirname  $CURRENT_FILE)
echo "Project's root directory: $ROOT_DIR"

export DATA_DIR="$ROOT_DIR/data"
echo "Project's data, resources and graphics stored in: $DATA_DIR"

export BUILD_DIR="$ROOT_DIR/build"
echo "Project is build in: $BUILD_DIR"

export EXEC_PATH="$BUILD_DIR/server"
echo "Application exec file is: $EXEC_PATH"

export USER_DATA_FILE="$DATA_DIR/users.txt"
echo "Users auth data stored in: $USER_DATA_FILE"

$BUILD_DIR/server