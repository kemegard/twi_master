# TWI Master — nRF54L15DK

I2C master example for the nRF54L15DK development kit, built with
**nRF Connect SDK v3.2.4**.

Every second the firmware writes a 32-bit counter to the I2C slave
at address **0x54**, then reads 4 bytes back and verifies they match.

The companion slave project is at
<https://github.com/kemegard/twi_slave>.

---

## Hardware

**Board:** PCA10156 (nRF54L15DK), `nrf54l15dk/nrf54l15/cpuapp`

**Peripheral used:** `i2c21` (TWIM — I2C master controller)

---

## Physical wiring

Connect the two nRF54L15DK boards as follows:

| Master board | Signal | Slave board |
|--------------|--------|-------------|
| P1.08        | SDA    | P1.09       |
| P1.12        | SCL    | P1.13       |
| GND          | GND    | GND         |

> **Note:** Pull-up resistors on SDA and SCL are handled in software
> via `bias-pull-up` in the device tree overlay — no external
> resistors are needed.

---

## Build

From the NCS environment (west workspace at `D:\work\ncs\v3.2.0`):

```bash
cd twi_master
west build -b nrf54l15dk/nrf54l15/cpuapp
```

---

## Flash

Find the serial number of the **master** board:

```bash
nrfutil device list
```

Then flash:

```bash
west flash --snr <serial-number>
```

Or using the build directory explicitly:

```bash
west flash --build-dir build --snr <serial-number>
```

---

## Expected serial output

Connect a terminal at **115200 8N1** to the master's COM port.
After reset the master writes and reads once per second:

```
*** Booting nRF Connect SDK v3.2.4-... ***
*** Using Zephyr OS v4.2.99-... ***
=== TWI Master started (slave addr 0x54) ===
[0] Wrote counter = 0
[0] Read back  = 0  [OK]
[1] Wrote counter = 1
[1] Read back  = 1  [OK]
[2] Wrote counter = 2
[2] Read back  = 2  [OK]
...
```

`[OK]` means the value read back from the slave matches what was
written. If the slave is not connected or not running you will see
`Write FAILED (err -5)` instead.
