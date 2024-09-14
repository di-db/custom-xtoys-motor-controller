# Motor Controller

| Device Details ||
|----------------|------------|
| Device Type:   | Fucking Machine |
| Tested Hardware: | [MCU ESP8266 Development Board with Motor Driver](https://www.amazon.co.uk/gp/product/B07ZCMZW9Q) |
| MQTT Publish Topic: | Configurable in code |
| MQTT Message Format: | {"channel" : &lt;&lt;channel&gt;&gt;, "speed": &lt;&lt;vibrate&gt;&gt; } |

Channel `A` is Motor A, Channel `B` is Motor B, max speed is 100, min speed is 0.
