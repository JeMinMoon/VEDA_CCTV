PKGS:=gstreamer-1.0 gstreamer-rtsp-server-1.0
CFLAGS:=$(shell pkg-config --cflags $(PKGS))
LDFLAGS:=$(shell pkg-config --libs $(PKGS))

all: RtspServerWithCamera

RtspServerWithCamera: RtspServerWithCamera.o
	g++ -o $@ $< $(CLAGS) $(LDFLAGS)

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm -f RtspServerWithCamera RtspServerWithCamera.o 