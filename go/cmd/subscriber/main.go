package main

import (
	"context"
	"fmt"
	"log"
	"os"

	"github.com/dapr/go-sdk/service/common"
	"github.com/dapr/go-sdk/service/http"
)

func getenv(key, fallback string) string {
	value := os.Getenv(key)
	if value == "" {
		return fallback
	}
	return value
}

func main() {
	pubsubName := getenv("DAPR_PUBSUB_NAME", "mqtt-pubsub")
	topic := getenv("MQTT_TOPIC", "esp32c6lcd/boot_button")
	appPort := getenv("APP_PORT", "6001")

	service := http.NewService(":" + appPort)
	sub := &common.Subscription{PubsubName: pubsubName, Topic: topic, Route: "/messages"}

	if err := service.AddTopicEventHandler(sub, func(ctx context.Context, e *common.TopicEvent) (bool, error) {
		_ = ctx
		payload := ""
		if len(e.RawData) > 0 {
			payload = string(e.RawData)
		} else {
			payload = fmt.Sprintf("%v", e.Data)
		}
		log.Printf("received topic=%s data=%s", e.Topic, payload)
		return true, nil
	}); err != nil {
		log.Fatalf("subscribe error: %v", err)
	}

	if err := service.Start(); err != nil {
		log.Fatalf("service error: %v", err)
	}
}
