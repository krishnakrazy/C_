cmd_/home/test/C/C_/kmod/hello_world_mod/modules.order := {   echo /home/test/C/C_/kmod/hello_world_mod/hw.ko; :; } | awk '!x[$$0]++' - > /home/test/C/C_/kmod/hello_world_mod/modules.order
