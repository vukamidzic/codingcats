#!/bin/bash

DESMOS=https://desmos.com/calculator

function catsrun() {
    CMD="$@"
    $CMD 
    echo $? > $CODINGCATS/info.config
}

function catsrun_desmos() {
    firefox $DESMOS 2>/dev/null &
}

