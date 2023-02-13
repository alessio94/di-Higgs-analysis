#!/bin/bash


_printHelp(){
    echo "First provide optional arguments:"
    echo "-h, --help : Print this message and exit."
    echo "--no-setup : Do not set up CAF environment automatically."
    echo ""
    echo "Then, specify the command to execute. If none is specified,"
    echo "an interactive shell will be started (/bin/bash)."
}

doSetup=1
nArgumentsKnown=0
for arg in "$@"; do
    # only consider arguments until the first unknown argument occurs
    if [ "$arg" == '--help' ] || [ "$arg" == '-h' ] || [ "$arg" = '--no-setup' ]; then
        if [ "$arg" == '--help' ] || [ "$arg" == '-h' ]; then
            nArgumentsKnown=$((nArgumentsKnown + 1))
            _printHelp
            exit 1
        fi
        if [ "$arg" == '--no-setup' ]; then
            nArgumentsKnown=$((nArgumentsKnown + 1))
            doSetup=0
        fi
    else
        break
    fi
done

firstArgument=$((nArgumentsKnown +1))
nArgumentsToPass=$(($# - $nArgumentsKnown))

if [[ $doSetup -eq 1 ]]; then
    if [ -f /release_setup.sh ]; then
        # Only source this file in AnalysisBase container
        source /release_setup.sh
    fi
    source /cafsetup.sh
fi

if [[ $nArgumentsToPass -eq 0 ]]; then
    # If the user does not specify any arguments, assume they want a shell
    exec /bin/bash
elif [[ $nArgumentsToPass -gt 0 ]]; then
    # Execute whatever the user specified after the script-specific options
    exec ${@:$firstArgument}
fi
