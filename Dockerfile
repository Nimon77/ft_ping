FROM debian:latest

RUN apt update && apt install curl build-essential -y

RUN curl -sL https://ftp.gnu.org/gnu/inetutils/inetutils-2.0.tar.gz | tar xz

WORKDIR /inetutils-2.0

RUN ./configure && make && make install
