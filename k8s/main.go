package main

import (
	"log"
	"log/slog"
	"os"
	"time"
)

func main() {
	logFile, err := os.OpenFile("file.log", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		log.Fatal(err)
	}
	defer logFile.Close()
	logger := slog.New(slog.NewJSONHandler(logFile, nil))

	ticker := time.NewTicker(1 * time.Second)
	defer ticker.Stop()

	for {
		select {
		case <-ticker.C:
			log.Printf("Log generated at: %v\n", time.Now().Format("2006-01-02 15:04:05"))
			logger.Info("Log generated", "time", time.Now().Format("2006-01-02 15:04:05"))
		}
	}
}
