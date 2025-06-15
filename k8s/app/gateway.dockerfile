FROM golang

WORKDIR /app
COPY go.mod gateway.go .
RUN go mod tidy
RUN go build gateway.go

ENTRYPOINT ["./gateway"]
