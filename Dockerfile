# Image
FROM gcc:latest

# Install Needed Packages
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y sudo
RUN apt-get install -y libssh-dev
RUN apt-get install -y iputils-ping curl bash cron
RUN apt-get install -y apt-utils dialog
RUN apt-get install -y openssh-client

# DIRECTORY FOR SSH KEYS
#RUN mkdir /etc/ssh

# SSH-Keygen Commands
RUN ssh-keygen -t rsa -f /etc/ssh/ssh_host_rsa_key -N '' && \
    ssh-keygen -t ecdsa -f /etc/ssh/ssh_host_ecdsa_key -N '' && \
    ssh-keygen -t ed25519 -f /etc/ssh/ssh_host_ed25519_key -N '' 
#    ssh-keygen -t rsa -b 4096 -f /etc/ssh/ssh_host_rsa_key -N ''


# Remove Unneeded Dependencies
RUN apt-get remove openssh-server openssh-client -y

# Set Files and Working Directory
COPY . /usr/src/honeypi
WORKDIR /usr/src/honeypi

# Compile C++ Code
RUN g++ -o honeypi start.cpp
RUN g++ -o randomize randomize.cpp
RUN g++ -o run running.cpp
RUN gcc -o ssh mainsshserverthreads.c -lssh

# Expose the SSH port
EXPOSE 22

# Start Application Command
ENTRYPOINT ["./honeypi"]

# Branch and Version Stuff
LABEL Name=honeypotpi Version=0.0.2
