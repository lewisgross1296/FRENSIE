#!/bin/bash

EXTRA_ARGS=$@

TRILINOS_INSTALL=$(pwd)/../../../install/trilinos
TRILINOS_SRC=$(pwd)/../src
MPI_DIR=$(pwd)/../../../install/mpi
LAPACK_DIR=$(pwd)/../../../install/lapack

source $HOME/.bashrc

cmake --version

cmake \
-D CMAKE_INSTALL_PREFIX:PATH=$TRILINOS_INSTALL \
-D CMAKE_BUILD_TYPE:STRING=RELEASE \
-D Trilinos_VERBOSE_CONFIGURE:BOOL=ON \
-D CMAKE_VERBOSE_MAKEFILE:BOOL=ON \
-D Trilinos_ASSERT_MISSING_PACKAGES:BOOL=OFF \
-D BUILD_SHARED_LIBS:BOOL=ON \
-D Trilinos_LINK_SEARCH_START_STATIC:BOOL=OFF \
-D TPL_FIND_SHARED_LIBS:BOOL=ON \
-D Trilinos_ENABLE_DEBUG:BOOL=OFF \
-D Trilinos_ENABLE_Teuchos:BOOL=ON \
-D Trilinos_ENABLE_TESTS:BOOL=ON \
-D Trilinos_ENABLE_EXAMPLES:BOOL=ON \
-D Trilinos_ENABLE_DEVELOPMENT_MODE:BOOL=OFF \
-D Teuchos_ENABLE_DEBUG_RCP_NODE_TRACING:BOOL=OFF \
-D Teuchos_ENABLE_ABC:BOOL=OFF \
-D TPL_ENABLE_MPI:BOOL=ON \
-D MPI_BASE_DIR:PATH=$MPI_DIR \
-D TPL_BLAS_LIBRARIES:PATH=$LAPACK_DIR/lib/libblas.so \
-D TPL_LAPACK_LIBRARIES:PATH=$LAPACK_DIR/lib/liblapack.so \
$EXTRA_ARGS \
$TRILINOS_SRC
