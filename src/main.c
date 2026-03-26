/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 *
 * TWI Master example for nRF54L15
 *
 * Every 1 second this firmware:
 *   1. Writes a 32-bit counter value to the TWI slave (address 0x54).
 *   2. Reads 4 bytes back from the slave and verifies they match.
 *
 * Hardware wiring (to slave board):
 *   Master P1.08 (SDA)  ----  Slave P1.08 (SDA)
 *   Master P1.12 (SCL)  ----  Slave P1.12 (SCL)
 *   Common GND          ----  Common GND
 *
 * Both boards use the same pin numbers - wire P1.08-to-P1.08, P1.12-to-P1.12.
 *
 * Build:
 *   west build -b nrf54l15dk/nrf54l15/cpuapp -- -DAPP_DIR=<path_to_twi_master>
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

/* i2c21 is the TWIM (master) peripheral on nRF54L15 */
#define I2C_MASTER_NODE  DT_NODELABEL(i2c21)
#define SLAVE_ADDR       0x54

int main(void)
{
	const struct device *i2c_dev = DEVICE_DT_GET(I2C_MASTER_NODE);
	uint32_t counter = 0;
	uint8_t write_buf[4];
	uint8_t read_buf[4];
	int ret;

	if (!device_is_ready(i2c_dev)) {
		printk("ERROR: I2C master device not ready\n");
		return -ENODEV;
	}

	printk("=== TWI Master started (slave addr 0x%02X) ===\n", SLAVE_ADDR);

	while (1) {
		/* --- WRITE: send the counter (little-endian, 4 bytes) --- */
		write_buf[0] = (uint8_t)((counter >>  0) & 0xFF);
		write_buf[1] = (uint8_t)((counter >>  8) & 0xFF);
		write_buf[2] = (uint8_t)((counter >> 16) & 0xFF);
		write_buf[3] = (uint8_t)((counter >> 24) & 0xFF);

		ret = i2c_write(i2c_dev, write_buf, sizeof(write_buf), SLAVE_ADDR);
		if (ret != 0) {
			printk("[%u] Write FAILED (err %d)\n", counter, ret);
		} else {
			printk("[%u] Wrote counter = %u\n", counter, counter);
		}

		/* Short pause so the slave has time to latch the data */
		k_msleep(10);

		/* --- READ: read back 4 bytes from the slave --- */
		memset(read_buf, 0, sizeof(read_buf));
		ret = i2c_read(i2c_dev, read_buf, sizeof(read_buf), SLAVE_ADDR);
		if (ret != 0) {
			printk("[%u] Read  FAILED (err %d)\n", counter, ret);
		} else {
			uint32_t received = (uint32_t)read_buf[0]
					  | ((uint32_t)read_buf[1] <<  8)
					  | ((uint32_t)read_buf[2] << 16)
					  | ((uint32_t)read_buf[3] << 24);

			printk("[%u] Read back  = %u  [%s]\n", counter, received,
			       (received == counter) ? "OK" : "MISMATCH");
		}

		counter++;
		k_sleep(K_SECONDS(1));
	}

	return 0;
}
