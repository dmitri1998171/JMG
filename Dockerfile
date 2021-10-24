FROM ubuntu
ADD . JMG

RUN apt update
RUN apt install -y make
RUN apt install -y g++ cmake freeglut3 freeglut3-dev binutils-gold libglew-dev mesa-common-dev  libglew1.5-dev libglm-dev  
RUN apt install -y build-essential 
RUN apt install -y libxxf86vm-dev libxcursor-dev 
RUN apt install -y libirrlicht-dev

ENTRYPOINT JMG/run.sh