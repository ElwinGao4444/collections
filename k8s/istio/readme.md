istio官方文档：https://istio.io/latest/zh/docs/setup/getting-started/

1. 下载安装（https://istio.io/latest/zh/docs/setup/install/istioctl/）
* wget https://istio.io/downloadIstio -O download_istio.sh
* sh download_istio.sh
* 将下载好的目录放到合适的位置，并将bin目录下的istioctl命令添加到环境变量PATH中
* 安装带有ingressgateway的istio：istioctl install
* 输出安装清单：istioctl manifest generate

1. 卸载istio（https://istio.io/latest/zh/docs/setup/install/istioctl/）
* istioctl uninstall --purge
* kubectl delete namespace istio-system
* kubectl delete -f https://github.com/kubernetes-sigs/gateway-api/releases/download/v1.3.0/standard-install.yaml

1. 部署bookinfo应用（https://istio.io/latest/zh/docs/examples/bookinfo/）
   1. 安装Gateway API CRD：kubectl get crd gateways.gateway.networking.k8s.io &> /dev/null || kubectl apply -f https://github.com/kubernetes-sigs/gateway-api/releases/download/v1.3.0/standard-install.yaml
   2. 在default命名空间中，进行envoy注入：kubectl label namespace default istio-injection=enabled
   3. 部署bookinfo应用：kubectl apply -f bookinfo.yaml
   4. 部署istio网关：kubectl apply -f bookinfo-gateway.yaml
   5. 查看ingress信息：kubectl get svc istio-ingressgateway -n istio-system
     * export INGRESS_NAME=istio-ingressgateway && echo $INGRESS_NAME
     * export INGRESS_NS=istio-system && echo $INGRESS_NS
     * export INGRESS_IP=$(kubectl -n "$INGRESS_NS" get service "$INGRESS_NAME" -o jsonpath='{.status.loadBalancer.ingress[0].ip}') && echo $INGRESS_IP
     * export INGRESS_HOST=$(kubectl -n "$INGRESS_NS" get service "$INGRESS_NAME" -o jsonpath='{.status.loadBalancer.ingress[0].hostname}') && echo $INGRESS_HOST
     * export INGRESS_PORT=$(kubectl -n "$INGRESS_NS" get service "$INGRESS_NAME" -o jsonpath='{.spec.ports[?(@.name=="http2")].port}') && echo $INGRESS_PORT
     * export SECURE_INGRESS_PORT=$(kubectl -n "$INGRESS_NS" get service "$INGRESS_NAME" -o jsonpath='{.spec.ports[?(@.name=="https")].port}') && echo $SECURE_INGRESS_PORT
     * export TCP_INGRESS_PORT=$(kubectl -n "$INGRESS_NS" get service "$INGRESS_NAME" -o jsonpath='{.spec.ports[?(@.name=="tcp")].port}') && echo $TCP_INGRESS_PORT
   6. 确认服务可连通性：curl -s http://localhost/productpage | fgrep title
2. 