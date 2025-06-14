docker build -t app-demo:latest .

docker build --no-cache -t app-gateway:latest -f gateway.dockerfile .
kubectl apply -f gateway.yaml

docker build --no-cache -t app-agent:latest -f agent.dockerfile .
kubectl apply -f agent.yaml

