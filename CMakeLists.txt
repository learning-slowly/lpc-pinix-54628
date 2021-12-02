# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.16.0)

#set(ENV{ZEPHYR_TOOLCHAIN_VARIANT} gnuarmemb)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)

zephyr_library_link_libraries(/home/kkoroke/study/lpc-pinix-54628/libpower_hardabi.a)
target_sources(app PRIVATE src/main.c src/clock.c)
