# Image
FROM debian:latest

# Add Needed Linux Repos
#RUN wget -qO - https://download.sublimetext.com/sublimehq-pub.gpg | gpg --dearmor | tee /etc/apt/trusted.gpg.d/sublimehq-archive.gpg > /dev/null
#RUN echo "deb https://download.sublimetext.com/ apt/stable/" | tee /etc/apt/sources.list.d/sublime-text.list

# Install Needed Packages
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y sudo
RUN apt-get install -y libssh-dev
RUN apt-get install -y iputils-ping curl bash cron
RUN apt-get install -y apt-utils dialog
RUN apt-get install -y openssh-client openssh-server
RUN apt-get install -y g++

# Remove Standard SSH Keys
RUN rm -f /etc/ssh/ssh_host_rsa_key
RUN rm -f /etc/ssh/ssh_host_ecdsa_key
RUN rm -f /etc/ssh/ssh_host_ed25519_key

# Set Files and Working Directory
COPY . /usr/src/honeypi
WORKDIR /usr/src/honeypi

# Compile C++ Code
RUN g++ -o randomize randomize.cpp
RUN g++ -o run running.cpp standardloops.cpp adminconsole.cpp -lssh
RUN g++ -o beta beta.cpp -lssh

# Remove Unneeded Dependencies
RUN apt-get remove -y g++
RUN apt-get autoremove -y

# REMOVE FILES
RUN mv randomize /usr/src/randomize
RUN mv run /usr/src/run
RUN mv beta /usr/src/beta
RUN rm /usr/src/honeypi/*
RUN mv /usr/src/randomize /usr/src/honeypi/randomize
RUN mv /usr/src/run /usr/src/honeypi/run
RUN mv /usr/src/beta /usr/src/honeypi/beta

# MKDIR FOR LOG FILES
RUN mkdir /var/rund
RUN touch /var/rund/log.txt

# Expose the SSH port
EXPOSE 22

# Start Application Command
ENTRYPOINT ["./run"]

# Branch and Version Stuff
LABEL Name=honeypotpi Version=0.2.0





#######################################
######### REMOVED COMMANDS!!! #########
#######################################

# DIRECTORY FOR SSH KEYS
# 8/8/24 - Already Attached

# SSH-Keygen Commands - REMOVED 8/8/24
# Reason: Handled by Program (Custom SSH Keys)

# Remove Unneeded Dependencies - REMOVED 8/8/24
# Reason: Handled by Program (Needed to Generate Random SSH Keys)

# Compile C++ Code
# RUN g++ -o ssh mainsshserverthreads.cpp -lssh
# Reason: Code Converted into C++ and into the main Running Script