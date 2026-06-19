savedcmd_device_driver.mod := printf '%s\n'   device_driver.o | awk '!x[$$0]++ { print("./"$$0) }' > device_driver.mod
