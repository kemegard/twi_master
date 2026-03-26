# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(twi_master)

if(CONFIG_GPIO_CONNECTIVITY_TEST)
  target_sources(app PRIVATE src/gpio_test_main.c)
else()
  target_sources(app PRIVATE src/main.c)
endif()
