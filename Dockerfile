FROM ubuntu:16.04
RUN apt update -y && \
    apt install build-essential libncurses5-dev libncursesw5-dev wget libcairo2-dev libgraphicsmagick1-dev \
    libpng-dev software-properties-common -y && \
    apt-add-repository ppa:ubuntu-toolchain-r/test -y  && \
    apt update -y && \
    apt install g++-7 -y && \
    rm /usr/bin/g++ && \
    ln -s /usr/bin/g++-7 /usr/bin/g++ &&\
    wget https://github.com/Kitware/CMake/releases/download/v3.14.3/cmake-3.14.3.tar.gz && \
    tar xzvf cmake-3.14.3.tar.gz && chmod +x /cmake-3.14.3/bootstrap
WORKDIR /cmake-3.14.3
RUN ./bootstrap && make && make install
WORKDIR /
COPY . /PROJ_HOME
WORKDIR /PROJ_HOME/Debug
RUN cmake .. && make
CMD ["./monitor"]
