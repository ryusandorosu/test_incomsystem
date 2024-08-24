#PORT = $1
#sudo lsof -i :5400
#sudo kill -9 [id]
#psql -h localhost -p 5400 -U postgres -d postgres --set=sslmode=disable

psql "host=localhost port=$1 user=postgres dbname=postgres sslmode=disable"
#./PSQLProxyServer $1
