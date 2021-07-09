cmd_/home/test/C/C_/kmod/netfileter_mod/modules.order := {   echo /home/test/C/C_/kmod/netfileter_mod/pre_hook_mod.ko; :; } | awk '!x[$$0]++' - > /home/test/C/C_/kmod/netfileter_mod/modules.order
