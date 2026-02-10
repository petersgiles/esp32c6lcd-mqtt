#pragma once

#include <stdint.h>

namespace AppUI {
void Init();
void UpdateStatus(bool wifiConnected,
				  const char * wifiIp,
				  bool mqttConnected,
				  int mqttState,
				  bool buttonPressed,
				  bool publishPending,
				  bool lastPublishOk,
				  const char * subscriberId,
				  uint32_t nowMs,
				  uint32_t lastPressMs,
				  uint32_t lastAckMs);
void AnimationTask(void * timer);
}
