github: https://github.com/prometheus-community/helm-charts
安装过程：
kubectl create namespace prometheus
helm install prometheus prometheus-community/kube-prometheus-stack --namespace prometheus
helm uninstall prometheus prometheus-community/kube-prometheus-stack --namespace prometheus

如果遇到node export报错，可以通过一下命令修复（https://github.com/prometheus-community/helm-charts/issues/467）
kubectl patch ds prometheus-prometheus-node-exporter --type "json" -p '[{"op": "remove", "path" : "/spec/template/spec/containers/0/volumeMounts/2/mountPropagation"}]'

