#sudo lsof -i :5400
#sudo kill -9 [id]
psql -h localhost -p $1 -U postgres -d postgres --set=sslmode=disable
#psql "host=localhost port=$1 user=postgres dbname=postgres sslmode=disable"
#./server.out $1