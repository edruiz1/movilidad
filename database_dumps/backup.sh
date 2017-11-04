#!/bin/bash

DATE=`date '+%Y%m%d%H%M%S'`
NAME="movilidad$DATE.sql"
mysqldump -u root -ptoor movilidad > $NAME

