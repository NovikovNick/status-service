## Status service

Status service is a sample of c++ microservice. 

```bash
#init cmake project
mkdir build
cd build
cmake ..

#build status-service docker image
cmake --build ./build --target build_docker

#run redis
docker-compose -f ./docker/docker-compose.yml up

#stop redis
docker-compose -f ./docker/docker-compose.yml stop

#run status-service
docker run --network host --rm -it metalheart/status-service:latest 

# useful command - show memory and cpu usage
docker stats
```
