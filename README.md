# TWI Master

I2C master example for Nordic DK boards, built with
**nRF Connect SDK v3.2.4+**.

Every second the firmware writes a 32-bit counter to the I2C slave
at address **0x54**, then reads 4 bytes back and verifies they match.

The companion slave project is at
<https://github.com/kemegard/twi_slave>.

---

## Supported boards

| Board target | DK | Peripheral | SDA | SCL |
|---|---|---|---|---|
| `nrf54l15dk/nrf54l15/cpuapp` | PCA10156 (nRF54L15DK) | `i2c21` | P1.08 | P1.12 |
| `nrf52dk/nrf52832` | PCA10040 (nRF52DK) | `i2c0` | P0.26 | P0.27 |
| `nrf52840dk/nrf52840` | PCA10056 (nRF52840DK) | `i2c0` | P0.26 | P0.27 |
| `nrf52833dk/nrf52833` | PCA10100 (nRF52833DK) | `i2c0` | P0.26 | P0.27 |

The I2C bus is selected at build time via the `i2c-master` devicetree
alias defined in each board overlay under `boards/`.

> **Note:** Pull-up resistors on SDA and SCL are handled in software
> via `bias-pull-up` in the device tree overlay — no external
> resistors are needed.

---

## Physical wiring

### nRF54L15DK

| Master board | Signal | Slave board |
|--------------|--------|-------------|
| P1.08        | SDA    | P1.09       |
| P1.12        | SCL    | P1.13       |
| GND          | GND    | GND         |

# nRF52DK / nRF52840DK / nRF52833DK

| Master board | Signal | Slave board |
|--------------|--------|-------------|
| P0.26        | SDA    | (slave SDA) |
| P0.27        | SCL    | (slave SCL) |
| GND          | GND    | GND         |

---

## Build

```bash
cd twi_master

# nRF54L15DK
west build -b nrf54l15dk/nrf54l15/cpuapp

# nRF52DK (nRF52832)
west build -b nrf52dk/nrf52832

# nRF52840DK
west build -b nrf52840dk/nrf52840

# nRF52833DK
west build -b nrf52833dk/nrf52833
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
