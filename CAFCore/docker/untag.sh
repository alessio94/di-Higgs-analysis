#/bin/bash

untagImage(){

    if [ "$#" -ne 4 ]; then
        echo "4 parameters needed. Use with arguments GITLAB_API_BASE IMAGE TAG GITLABTOKEN"
        return 1
    fi

    local GITLAB_API_BASE=$1
    local IMAGE=$2
    local TAG=$3
    local GITLABTOKEN=$4
    local AUTH="PRIVATE-TOKEN: $GITLABTOKEN"

    IMAGE_ID=$(curl --header "$AUTH" "$GITLAB_API_BASE" | tr "},{" "},\n{"  | grep "location\":\"$IMAGE\"" | sed 's|"id":\([0-9]*\),.*|\1|')
    if [ "$IMAGE_ID" == "" ] ; then
        echo "Couldn't find image ID for $IMAGE"
        return 1
    else
        local CMD="curl --header \"$AUTH\" --request DELETE $GITLAB_API_BASE/$IMAGE_ID/tags/$TAG"
        echo "$CMD"
        eval $CMD
        return $?
    fi
}
