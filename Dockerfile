FROM ubuntu:20.04

USER root
RUN apt-get -y update &&  apt-get -y  upgrade && cd home && mkdir builder
WORKDIR /home/builder
RUN DEBIAN_FRONTEND="noninteractive" apt-get -y install binutils wget g++ libgmp-dev libmpfr-dev libmpc-dev flex libbison-dev make xz-utils && \
    wget http://mirror.linux-ia64.org/gnu/gcc/snapshots/LATEST-14/gcc-14-20240211.tar.xz &&  \
    tar -xf gcc-14-20240211.tar.xz && \
    cd gcc-14-20240211 && \
    mkdir objdir &&  \
    cd objdir &&  \
    ../configure --disable-multilib && \
    make -j24 && make install && cd /home/builder/ && rm -rf gcc-14-20240211 gcc-14-20240211.tar.xz && \
    apt-get -y purge --autoremove libgmp-dev libmpfr-dev libmpc-dev flex libbison-dev g++


RUN useradd builder && chown builder .

RUN wget https://github.com/Kitware/CMake/releases/download/v3.28.3/cmake-3.28.3-linux-x86_64.tar.gz  \
    && su builder -c "tar -xf cmake-3.28.3-linux-x86_64.tar.gz && rm -r cmake-3.28.3-linux-x86_64.tar.gz && mv cmake-3.28.3-linux-x86_64 cmake"
#VOLUME /home/builder/vcpkg/
RUN apt-get -y install git zip curl  && git config --global --add safe.directory "*" && \
    su builder -c "git clone https://github.com/microsoft/vcpkg" &&  \
    su builder -c "./vcpkg/bootstrap-vcpkg.sh -disableMetrics"
RUN wget https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-linux.zip && unzip ninja-linux.zip && rm ninja-linux.zip
RUN DEBIAN_FRONTEND="noninteractive" apt-get -y install pkg-config libmpc3 libc6-dev python3 bison python3-distutils libx11-dev libxft-dev libxext-dev \
 libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev autoconf libtool
#gdb
#
ENV PATH=$PATH:/home/builder/cmake/bin:/home/builder
ENV VCPKG_ROOT=/home/builder/vcpkg
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib64
#USER builder
#RUN apt-get -y clean