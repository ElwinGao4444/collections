安装ingress-nginx-controller：https://kubernetes.github.io/ingress-nginx/deploy/#docker-for-mac
命令（二选一即可—）：
1. helm upgrade --install ingress-nginx ingress-nginx --repo https://kubernetes.github.io/ingress-nginx --namespace ingress-nginx --create-namespace
2. kubectl apply -f https://raw.githubusercontent.com/kubernetes/ingress-nginx/controller-v1.12.3/deploy/static/provider/cloud/deploy.yaml

安装cert-manager：https://cert-manager.io/docs/getting-started/
命令（二选一即可）：
1. helm安装
   * helm repo add jetstack https://charts.jetstack.io --force-update
   * helm install cert-manager jetstack/cert-manager --namespace cert-manager --create-namespace --version v1.18.0 --set crds.enabled=true
2. kubectl apply -f https://github.com/cert-manager/cert-manager/releases/download/v1.18.0/cert-manager.yaml
