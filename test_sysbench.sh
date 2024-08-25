#echo "usage: ./sysbench_test.sh <database> <port>"
psql -h localhost -p $2 -U postgres -c "create database $1;" --set=sslmode=disable
sysbench /usr/share/sysbench/oltp_read_write.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$2 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$1 --tables=10 --table-size=50 prepare
sysbench /usr/share/sysbench/oltp_read_write.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$2 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$1 --tables=10 --table-size=50 --threads=2 --time=60 run
sysbench /usr/share/sysbench/oltp_read_write.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$2 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$1 cleanup