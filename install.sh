#!/bin/bash

docker-compose up -d
docker exec -d dockertest_os_1 touch test11.txt
