cat isa-trace | awk '{print $1}' | grep "v_" | sort --unique
