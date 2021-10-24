open -a XQuartz
export IP=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}') 
xhost + $IP
docker run --rm -it -e DISPLAY=$IP:0 -v /tmp/.X11-unix:/tmp/.X11-unix -v /Users/dmitry/Documents/JMG:/JMG $1 $2
# docker run --rm -it -e DISPLAY=$IP:0 -v /tmp/.X11-unix:/tmp/.X11-unix $1 $2