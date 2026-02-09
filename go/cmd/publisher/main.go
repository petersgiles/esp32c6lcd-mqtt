package main

import (
	"context"
	"log"
	"os"
	"strconv"
	"time"

	dapr "github.com/dapr/go-sdk/client"
)

func getenv(key, fallback string) string {
	value := os.Getenv(key)
	if value == "" {
		return fallback
	}
	return value
}

func getenvInt(key string, fallback int) int {
	value := os.Getenv(key)
	if value == "" {
		return fallback
	}
	parsed, err := strconv.Atoi(value)
	if err != nil {
		return fallback
	}
	return parsed
}

func main() {
	pubsubName := getenv("DAPR_PUBSUB_NAME", "mqtt-pubsub")
	topic := getenv("MQTT_TOPIC", "esp32c6lcd/boot_button")
	message := getenv("MQTT_MESSAGE", "test")
	count := getenvInt("MQTT_COUNT", 1)
	delayMs := getenvInt("MQTT_DELAY_MS", 0)

	client, err := dapr.NewClient()
	if err != nil {
		log.Fatalf("dapr client error: %v", err)
	}
	defer client.Close()

	delay := time.Duration(delayMs) * time.Millisecond
	for i := 0; i < count; i++ {
		if err := client.PublishEvent(context.Background(), pubsubName, topic, []byte(message)); err != nil {
			log.Fatalf("publish error: %v", err)
		}
		if delay > 0 {
			time.Sleep(delay)
		}
	}

	log.Printf("published %d message(s) to %s/%s", count, pubsubName, topic)
}
