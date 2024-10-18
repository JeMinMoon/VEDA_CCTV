PKGS:=gstreamer-1.0 gstreamer-rtsp-server-1.0
CFLAGS:=$(shell pkg-config --cflags $(PKGS))
LDFLAGS:=$(shell pkg-config --libs $(PKGS))

all: GStreamerRTSPServer RtspServerWithCamera rtsp_streamer 

GStreamerRTSPServer: GStreamerRTSPServer.o
	g++ -o $@ $< $(CFLAGS) $(LDFLAGS)

RtspServerWithCamera: RtspServerWithCamera.o
	g++ -o $@ $< $(CLAGS) $(LDFLAGS)

rtsp_streamer: rtsp_streamer.o
	g++ -o $@ $< $(CLAGS) $(LDFLAGS)

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm -f GStreamerRTSPServer RtspServerWithCamera rtsp_streamer GStreamerRTSPServer.o RtspServerWithCamera.o rtsp_streamer.o