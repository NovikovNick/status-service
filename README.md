## Status service

Status service is a sample of c++ microservice. 

```bash
#build docker
docker build -t metalheart/status-service ./docker

#run docker-compose
docker-compose -f ./docker/docker-compose.yml up

# run status-service
docker run --network host --rm -it  metalheart/status-service:latest 

# useful command
docker stats
```