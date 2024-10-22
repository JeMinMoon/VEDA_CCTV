#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <iostream>

#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// 스트림 핸들러 (현재는 기본 설정만 수행)
static void media_constructed(GstRTSPMediaFactory *factory, GstRTSPMedia *media, gpointer user_data) {
    GstElement *pipeline = gst_rtsp_media_get_element(media);
    if (!pipeline) {
        std::cerr << "Failed to get pipeline from media.\n";
        return;
    }

    // 추가적인 파이프라인 설정이 필요할 경우 여기에 작성

    gst_object_unref(pipeline);
}

std::string get_ip_addr();

int main(int argc, char *argv[]) {
    GstRTSPServer *server;
    GstRTSPMountPoints *mounts;
    GstRTSPMediaFactory *factory;
    GMainLoop *loop;
    std::string ip_address = get_ip_addr();

    // GStreamer 초기화
    gst_init(&argc, &argv);

    // 메인 루프 생성
    loop = g_main_loop_new(NULL, FALSE);

    // RTSP 서버 생성
    server = gst_rtsp_server_new();
    gst_rtsp_server_set_service(server, "8554"); // 포트 설정


    // 마운트 포인트 가져오기
    mounts = gst_rtsp_server_get_mount_points(server);

    // 미디어 팩토리 생성
    factory = gst_rtsp_media_factory_new();

    // 카메라 소스를 사용하는 파이프라인 설정
    gst_rtsp_media_factory_set_launch(factory,
        "( libcamerasrc camera-name=/base/axi/pcie@120000/rp1/i2c@88000/ov5647@36 \
    ! video/x-raw,width=640,height=480,framerate=40/1,format=YUY2 ! queue ! videoconvert ! x264enc speed-preset=ultrafast tune=fastdecode \
    ! video/x-h264,profile=high ! rtph264pay name=pay0 pt=96 )");
    
    gst_rtsp_media_factory_set_shared(factory, TRUE);

    // 스트림 핸들러 연결 (옵션)
    g_signal_connect(factory, "media-constructed", (GCallback)media_constructed, NULL);

    // 마운트 포인트에 미디어 세션 추가 (rtsp://<IP>:8554/camera)
    gst_rtsp_mount_points_add_factory(mounts, "/camera", factory);

    // 마운트 포인트 객체 참조 해제
    g_object_unref(mounts);

    // RTSP 서버 활성화
    gst_rtsp_server_attach(server, NULL);

    std::cout << "RTSP 서버가 8554 포트에서 실행 중입니다.\n";
    std::cout << "RTSP URL: rtsp://" << ip_address << ":8554/camera\n";

    // 메인 루프 실행
    g_main_loop_run(loop);

    return 0;
}

std::string get_ip_addr(){
    struct ifaddrs *ifap = nullptr;
    struct ifaddrs *i = nullptr;
    void *src;
    std::string ip_address = "";

    if(getifaddrs(&ifap)==0){ //creates a linked list of structures describing the network interfaces of the local system
        for(i = ifap; i != nullptr; i=i->ifa_next){
            if(i->ifa_addr == nullptr)
                continue;
            
            if(i->ifa_addr->sa_family == AF_INET){
                char tmp[INET_ADDRSTRLEN];
                src = &((struct sockaddr_in *)i->ifa_addr)->sin_addr;
                inet_ntop(AF_INET, src, tmp, INET_ADDRSTRLEN); // converts the network address structure to character string
                if(strcmp(i->ifa_name, "lo")!=0){ // 127.0.0.1 제외
                    ip_address = tmp;
                    break;
                }
            }
        }
    }

    if (ifap != nullptr) {
        freeifaddrs(ifap);
    }

    return ip_address;
}