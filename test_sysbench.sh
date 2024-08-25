echo "usage: ./sysbench_test.sh <port> <database> <password>"
psql -h localhost -p $1 -U postgres -c "create database $2;" --set=sslmode=disable
sysbench /usr/share/sysbench/oltp_read_write.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=$3 --pgsql-db=$2 --tables=10 --table-size=100000 prepare
sysbench /usr/share/sysbench/oltp_read_write.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=$3 --pgsql-db=$2 --tables=10 --table-size=100000 --threads=10 --time=60 run
sysbench /usr/share/sysbench/oltp_read_write.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=$3 --pgsql-db=$2 cleanup
