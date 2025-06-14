FROM golang

WORKDIR /app
COPY gateway.go .
RUN go build gateway.go

ENTRYPOINT ["./gateway"]
