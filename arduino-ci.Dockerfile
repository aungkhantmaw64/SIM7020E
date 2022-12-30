FROM ubuntu:22.10

WORKDIR /home

COPY ./scripts/installEssentials.sh /home/

COPY ./scripts/configLocales.sh /home/

RUN bash ./installEssentials.sh

RUN rbenv install 3.1.2 && rbenv global 3.1.2

RUN bash ./configLocales.sh

ENV LANG en_US.utf8

RUN apt-get update -y && apt-get install -y \
    python3 python3-pip

RUN python3 -m pip install pyserial

RUN gem install bundler

RUN gem install arduino_ci && \
    gem update arduino_ci

WORKDIR /home/work