# Arduino Line Sensor
Firmware to manage a Pololu QTR-8A sensor array connected to two ADS1015's communication over I<sup>2</sup>C with an Arduino Nano communicating over USB using COBS encoding.

## Packet Descriptions

### Request Packet Format

| Start Flag | Command | Payload Size |
|------------|---------|--------------|
| 1 byte     | 1 byte  | x bytes      |

### Response Packet Format

| Start Flag 1 | Start Flag 2 | Command | Response Data Length | Response |
|--------------|--------------|---------|----------------------|----------|
| `0xA1`       | `0x1A`       | 1 byte  | 1 byte               | x bytes  |

### Requests Overview

| Request  | Value  | Payload |
|----------|--------|---------|
| IS_READY | `0x01` | N/A     |
| RESET    | `0x02` | N/A     |

#### Is ready Request
Request: `0xA1` `0x01`

Triggers the ready response to make sure the Arduino is ready for operation.

#### Reset Request
Request: `0xA1` `0x02`

Resets the ready boolean to false to prevent data being sent out when not connected.

### Responses Overview

| Request  | Value  | Payload                       |
|----------|--------|-------------------------------|
| IS_READY | `0xFF` | N/A                           |
| DATA     | `0x10` | 8 times sensor data (2 bytes) |

#### Ready Response
**Response:** `0xA1` `0x1A` `0xFF` `0x00`

This response will be sent when the Teensy is ready to be controlled.

#### Data Response
**Response:** `0xA1` `0x1A` `0x10` `0x10` `...data`

This response will be sent 50 times per second.
