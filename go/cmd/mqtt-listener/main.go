package main

import (
	"crypto/tls"
	"crypto/x509"
	"fmt"
	"log"
	"os"
	"time"

	mqtt "github.com/eclipse/paho.mqtt.golang"
)

func main() {
	broker := getenv("MQTT_BROKER", "ssl://broker.example.com:8883")
	topic := getenv("MQTT_TOPIC", "esp32c6lcd/boot_button")
	clientID := getenv("MQTT_CLIENT_ID", "esp32c6lcd-listener")
	username := getenv("MQTT_USERNAME", "")
	password := getenv("MQTT_PASSWORD", "")
	useTLS := getenv("MQTT_TLS", "true") == "true"

	opts := mqtt.NewClientOptions().AddBroker(broker).SetClientID(clientID)
	if username != "" {
		opts.SetUsername(username)
		opts.SetPassword(password)
	}

	if useTLS {
		tlsConfig, err := buildTLSConfig()
		if err != nil {
			log.Fatalf("TLS config error: %v", err)
		}
		opts.SetTLSConfig(tlsConfig)
	}

	opts.OnConnect = func(c mqtt.Client) {
		if token := c.Subscribe(topic, 1, onMessage); token.Wait() && token.Error() != nil {
			log.Printf("subscribe error: %v", token.Error())
		} else {
			log.Printf("subscribed to %s", topic)
		}
	}

	client := mqtt.NewClient(opts)
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		log.Fatalf("connect error: %v", token.Error())
	}

	for {
		time.Sleep(5 * time.Second)
	}
}

func onMessage(_ mqtt.Client, msg mqtt.Message) {
	fmt.Printf("%s %s\n", msg.Topic(), string(msg.Payload()))
}

func buildTLSConfig() (*tls.Config, error) {
	caPath := getenv("MQTT_CA_CERT", "")
	clientCertPath := getenv("MQTT_CLIENT_CERT", "")
	clientKeyPath := getenv("MQTT_CLIENT_KEY", "")

	if caPath == "" || clientCertPath == "" || clientKeyPath == "" {
		return nil, fmt.Errorf("missing TLS env vars")
	}

	caCert, err := os.ReadFile(caPath)
	if err != nil {
		return nil, err
	}

	caPool := x509.NewCertPool()
	if ok := caPool.AppendCertsFromPEM(caCert); !ok {
		return nil, fmt.Errorf("failed to parse CA cert")
	}

	clientCert, err := tls.LoadX509KeyPair(clientCertPath, clientKeyPath)
	if err != nil {
		return nil, err
	}

	return &tls.Config{
		RootCAs:      caPool,
		Certificates: []tls.Certificate{clientCert},
		MinVersion:   tls.VersionTLS12,
	}, nil
}

func getenv(key, fallback string) string {
	value := os.Getenv(key)
	if value == "" {
		return fallback
	}
	return value
}
