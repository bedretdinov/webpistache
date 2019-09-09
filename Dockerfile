FROM python:3.6-alpine

MAINTAINER webpistache

WORKDIR /root/build
RUN echo "@community http://dl-cdn.alpinelinux.org/alpine/edge/community" >> /etc/apk/repositories
RUN apk update
RUN apk update
RUN apk add git
RUN apk add g++
RUN apk add make
RUN apk add cmake
RUN cat /usr/include/stdint.h
RUN cat /usr/include/bits/alltypes.h
RUN cat /usr/include/bits/stdint.h
RUN cat /usr/include/sys/types.h
RUN cat /usr/include/sys/sysmacros.h
RUN git clone https://github.com/rob-h-w/pistache.git
WORKDIR pistache
RUN git checkout Avoid-major-minor-macros
RUN git submodule update --init

WORKDIR build
RUN cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
RUN make

RUN make install

WORKDIR /home/pistache

# Clean up the build files.
RUN rm -rf /root/build





RUN  git clone https://github.com/mongodb/mongo-cxx-driver.git \
    --branch releases/stable --depth 1 \
    cd mongo-cxx-driver/build



COPY  . /home/pistache