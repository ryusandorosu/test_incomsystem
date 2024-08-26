DATABASE=sbtest
psql -h localhost -U postgres -c "SELECT pg_terminate_backend(pid) FROM pg_stat_activity WHERE datname = '$DATABASE';"
psql -h localhost -U postgres -c "drop database $DATABASE;"