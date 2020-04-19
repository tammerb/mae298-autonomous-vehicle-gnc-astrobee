#!/bin/bash

docker-compose up -d --build
docker exec -d astrobee_container touch test11.txt
