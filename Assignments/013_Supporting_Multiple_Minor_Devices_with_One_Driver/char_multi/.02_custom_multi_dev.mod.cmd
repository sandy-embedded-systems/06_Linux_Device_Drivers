savedcmd_02_custom_multi_dev.mod := printf '%s\n'   02_custom_multi_dev.o | awk '!x[$$0]++ { print("./"$$0) }' > 02_custom_multi_dev.mod
