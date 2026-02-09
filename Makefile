GO_DIR := go
GO_CMD := $(GO_DIR)/cmd/mqtt-listener
SKETCH_FILE := esp32c6lcd-mqtt/esp32c6lcd-mqtt.ino
ARDUINO_FQBN := esp32:esp32:esp32c6
ARDUINO_PORT := /dev/ttyACM0
ARDUINO_CLI_CONFIG := arduino-cli.yaml
ESP32_FQBN_OPTS := PartitionScheme=huge_app

.PHONY: all go-build go-run go-tidy go-fmt esp32-build esp32-upload

all: go-build

go-build:
	cd $(GO_DIR) && go build ./...

go-run:
	cd $(GO_DIR) && go run ./cmd/mqtt-listener

go-tidy:
	cd $(GO_DIR) && go mod tidy

go-fmt:
	cd $(GO_DIR) && gofmt -w cmd

esp32-init:
	arduino-cli --config-file arduino-cli.yaml core update-index
	arduino-cli --config-file arduino-cli.yaml core install esp32:esp32
	arduino-cli --config-file arduino-cli.yaml lib install lvgl PubSubClient

esp32-build:
	arduino-cli --config-file $(ARDUINO_CLI_CONFIG) compile --fqbn $(ARDUINO_FQBN)$(if $(ESP32_FQBN_OPTS),:$(ESP32_FQBN_OPTS)) $(SKETCH_FILE)

esp32-upload:
	arduino-cli --config-file $(ARDUINO_CLI_CONFIG) upload -p $(ARDUINO_PORT) --fqbn $(ARDUINO_FQBN)$(if $(ESP32_FQBN_OPTS),:$(ESP32_FQBN_OPTS)) $(SKETCH_FILE)
