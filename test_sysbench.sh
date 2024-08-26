#echo "usage: ./sysbench_test.sh <port>"
DATABASE=sbtest
#TIME=300
SCENARIO=oltp_read_only.lua #works
#SCENARIO=oltp_point_select.lua #works
#SCENARIO=oltp_update_index.lua #works
#SCENARIO=oltp_update_non_index.lua #works
#SCENARIO=oltp_insert.lua #works
#SCENARIO=oltp_delete.lua #works
#SCENARIO=select_random_ranges.lua #works
#SCENARIO=oltp_write_only.lua #problems
#SCENARIO=oltp_read_write.lua #problems
#SCENARIO=bulk_insert.lua #problems
#SCENARIO=select_random_points.lua #problems

psql -h localhost -p $1 -U postgres -c "create database $DATABASE;" --set=sslmode=disable

#per one scenario
sysbench /usr/share/sysbench/$SCENARIO --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=5 --table-size=50 prepare
sysbench /usr/share/sysbench/$SCENARIO --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=5 --table-size=50 --threads=30 --time=10 run
sysbench /usr/share/sysbench/$SCENARIO --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE cleanup

#integrated scenarios
#sysbench /usr/share/sysbench/oltp_read_only.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 prepare
#sysbench /usr/share/sysbench/oltp_read_only.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 --threads=10 --time=$TIME run
#sysbench /usr/share/sysbench/oltp_read_only.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE cleanup
#sysbench /usr/share/sysbench/oltp_point_select.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 prepare
#sysbench /usr/share/sysbench/oltp_point_select.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 --threads=10 --time=$TIME run
#sysbench /usr/share/sysbench/oltp_point_select.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE cleanup
#sysbench /usr/share/sysbench/oltp_update_index.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 prepare
#sysbench /usr/share/sysbench/oltp_update_index.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 --threads=10 --time=$TIME run
#sysbench /usr/share/sysbench/oltp_update_index.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE cleanup
#sysbench /usr/share/sysbench/oltp_update_non_index.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 prepare
#sysbench /usr/share/sysbench/oltp_update_non_index.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 --threads=10 --time=$TIME run
#sysbench /usr/share/sysbench/oltp_update_non_index.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE cleanup
#sysbench /usr/share/sysbench/oltp_insert.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 prepare
#sysbench /usr/share/sysbench/oltp_insert.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 --threads=10 --time=$TIME run
#sysbench /usr/share/sysbench/oltp_insert.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE cleanup
#sysbench /usr/share/sysbench/oltp_delete.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 prepare
#sysbench /usr/share/sysbench/oltp_delete.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 --threads=10 --time=$TIME run
#sysbench /usr/share/sysbench/oltp_delete.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE cleanup
#sysbench /usr/share/sysbench/select_random_ranges.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 prepare
#sysbench /usr/share/sysbench/select_random_ranges.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE --tables=10 --table-size=50 --threads=10 --time=$TIME run
#sysbench /usr/share/sysbench/select_random_ranges.lua --db-driver=pgsql --pgsql-host=127.0.0.1 --pgsql-port=$1 --pgsql-user=postgres --pgsql-password=123 --pgsql-db=$DATABASE cleanup

psql -h localhost -U postgres -c "SELECT pg_terminate_backend(pid) FROM pg_stat_activity WHERE datname = '$DATABASE';"
psql -h localhost -U postgres -c "drop database $DATABASE;"