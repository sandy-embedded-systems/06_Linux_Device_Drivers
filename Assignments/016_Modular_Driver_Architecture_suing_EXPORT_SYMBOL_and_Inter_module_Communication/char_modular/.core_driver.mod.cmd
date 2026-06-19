savedcmd_core_driver.mod := printf '%s\n'   core_driver.o | awk '!x[$$0]++ { print("./"$$0) }' > core_driver.mod
