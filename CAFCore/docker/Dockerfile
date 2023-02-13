# This dockerfile builds CAFCore on top of the AnalysisBase image.
# It is the equivalent of Dockerfile.CI, but it builds the code
# itself instead of copying it from artifacts. It should work out-
# of-the box with
# docker build -f docker/Dockerfile -t cafcore:customtag .

ARG ANALYSISBASE_IMAGE=atlas/analysisbase:21.2.102
FROM ${ANALYSISBASE_IMAGE}

ARG CODE_DIR=/CAFCore
ARG CMAKE_CXX_FLAGS=""

# Copy the project's source code into the image
COPY --chown=atlas . ${CODE_DIR}/source

# Build the project inside /CAFCore/build
RUN sudo chown -R atlas ${CODE_DIR} && \
    mkdir ${CODE_DIR}/build && \
    cd ${CODE_DIR}/build && \
    source /release_setup.sh && \
    [ $CMAKE_CXX_FLAGS != "" ] && cmake -D CMAKE_CXX_FLAGS="$CMAKE_CXX_FLAGS" ../source/ || cmake ../source/ && \
    make -j4 && \
    sudo ln -s ${CODE_DIR}/build/cafsetup.sh /cafsetup.sh && \
    sudo ln -s ${CODE_DIR}/source/docker/docker-entrypoint.sh /docker-entrypoint.sh && \
    cd ${CODE_DIR}/source && \
    git remote set-url origin ssh://git@gitlab.cern.ch:7999/atlas-caf/CAFCore.git

ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["/bin/bash"]
