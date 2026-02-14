#!/bin/bash
# 1. Kill all processes owned by the ctf_player user
# This clears the process table for the new connection
pkill -u cin-drallig -9

# 2. Small grace period for the kernel to reap the PIDs
sleep 0.1

# 3. Execute the nursery as the ctf_player
# We use 'exec' so the script is replaced by the binary
exec runuser -u cin-drallig -- /chall/jedi-temple


