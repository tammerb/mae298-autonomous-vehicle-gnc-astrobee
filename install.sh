#!/bin/bash

docker-compose up -d
docker exec -d astrobee_container touch test11.txt
