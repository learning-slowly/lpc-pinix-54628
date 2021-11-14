#
# SPDX-License-Identifier: Apache-2.0
# SPDX-License-Identifier: Apache-2.0

board_runner_args(jlink "--device=LPC54628J512" "--speed=4000")

include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)
