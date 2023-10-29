FROM ubuntu:latest

ADD ./status-service /srv/app/status-service
ADD ./boot.sh /srv/app/

ENTRYPOINT ["/srv/app/boot.sh"]