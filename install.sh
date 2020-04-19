#!/bin/bash

docker-compose up -d --build
docker exec -d astrobee_container touch test12.txt
