FROM ubuntu:24.04

# Avoid interactive installs
ENV DEBIAN_FRONTEND=noninteractive

# Install only what we need
RUN apt-get update && apt-get install -y \
    gcc socat   \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /chall

RUN useradd -m cin-drallig


# Copy challenge
COPY jedi-temple.c .
COPY flag.txt .
COPY run.sh .


# Build challenge
RUN gcc jedi-temple.c -o jedi-temple
RUN chmod +x ./run.sh
RUN chmod 755 ./jedi-temple

EXPOSE 1337

CMD ["socat", "TCP-LISTEN:1337,reuseaddr,fork", "EXEC:./run.sh"]
