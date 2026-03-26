/*
 * GPIO Connectivity Test - MASTER
 *
 * Drives P1.08 (SDA wire) and P1.12 (SCL wire) as outputs.
 * Toggles them HIGH/LOW every second so the slave can observe
 * the transitions and confirm physical connectivity.
 *
 * Expected slave output when wired correctly:
 *   Even cycles:  SDA=1  SCL=1
 *   Odd  cycles:  SDA=0  SCL=0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define GPIO_NODE   DT_NODELABEL(gpio1)
#define PIN_SDA     8    /* P1.08 -> slave P1.09 */
#define PIN_SCL     12   /* P1.12 -> slave P1.13 */
#define CYCLES      6

int main(void)
{
	const struct device *gpio = DEVICE_DT_GET(GPIO_NODE);

	if (!device_is_ready(gpio)) {
		printk("ERROR: gpio1 not ready\n");
		return -ENODEV;
	}

	gpio_pin_configure(gpio, PIN_SDA, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure(gpio, PIN_SCL, GPIO_OUTPUT_INACTIVE);

	printk("=== GPIO Connectivity Test - MASTER ===\n");
	printk("P1.08->SDA  P1.12->SCL  (slave reads P1.09 / P1.13)\n");
	printk("Toggling %d cycles, 1 second per level...\n\n", CYCLES);

	for (int i = 0; i < CYCLES; i++) {
		int lvl = i % 2;

		gpio_pin_set(gpio, PIN_SDA, lvl);
		gpio_pin_set(gpio, PIN_SCL, lvl);
		printk("[%d] Driving SDA=P1.08=%d  SCL=P1.12=%d\n", i, lvl, lvl);
		k_msleep(1000);
	}

	/* Leave both pins LOW when done */
	gpio_pin_set(gpio, PIN_SDA, 0);
	gpio_pin_set(gpio, PIN_SCL, 0);

	printk("\n=== Master GPIO test complete ===\n");

	while (1) {
		k_sleep(K_FOREVER);
	}
	return 0;
}
