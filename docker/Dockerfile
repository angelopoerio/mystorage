FROM debian
MAINTAINER Angelo Poerio <angelo.poerio@gmail.com>
RUN apt-get update
RUN apt-get -y install git make gcc g++ supervisor
RUN adduser mystorage 
RUN cd /home/mystorage;git clone https://github.com/angelopoerio/mystorage.git
COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf
RUN cd /home/mystorage/mystorage/src; make mystorage
RUN cp /home/mystorage/mystorage/src/mystorage /usr/bin/mystorage

EXPOSE 7777
CMD ["supervisord"]
