# VEDA에서 진행한 팀 프로젝트
## 주제
Raspberry Pi를 이용한 cctv와 서버 개발. 서버에서 송출하는 실시간 영상을 수신하는 Qt기반 클라이언트 개발.
### 기능

### 기술 스택  
gstreamer  
Qt  
C/C++

### 하드웨어
raspberrypi 5  
pi CameraModule v1

## 서버 구동을 위한 환경 설정
raspberrypi에서 진행.
### 1. GStreamer 설치
```
sudo apt-get install libgstreamer1.0-dev libgstrtspserver-1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly
```

## Qt 개발을 위한 환경 설정

### 1. Qt 설치

Qt는 크로스 플랫폼 애플리케이션 개발을 위한 프레임워크입니다. Qt Maintenance Tool을 사용하여 필요한 구성 요소를 설치합니다.

1. **Qt Maintenance Tool 실행**
   - Qt를 설치할 때 함께 설치된 `Qt Maintenance Tool`을 실행합니다.
   
2. **구성 요소 추가**
   - **Qt 버전 선택**: `Qt 6.7.3`을 선택합니다.
   - **MinGW 선택**: 하위 목록에서 `MinGW 11.2.0`을 선택합니다.
   - **추가 라이브러리 선택**: 같은 경로의 `Additional Libraries`에서 `Qt Multimedia`를 선택합니다.
   - **설치 완료**: 선택한 구성 요소를 설치합니다.

3. **환경 변수 설정**
   - 시스템 변수 `Path`에 `C:\Qt\Tools\mingw1120_64\bin`을 추가합니다.
   - **설정 방법**:
     1. **시스템 속성** 열기: `Win + Pause` → `고급 시스템 설정` → `환경 변수`.
     2. **Path 편집**: `Path`를 선택하고 `편집`을 클릭합니다.
     3. **새 항목 추가**: `C:\Qt\Tools\mingw1120_64\bin`을 추가하고 확인을 클릭합니다.

### 2. GStreamer 설치

GStreamer는 멀티미디어 처리 파이프라인을 구성하기 위한 프레임워크입니다.

1. **GStreamer 다운로드 및 설치**
   - [GStreamer 공식 다운로드 페이지](https://gstreamer.freedesktop.org/download/#windows)로 이동합니다.
   - **MinGW 64-bit Runtime 및 Development 설치**:
     - `MinGW` 용 패키지에서 `Runtime`과 `Development` 패키지를 모두 설치합니다.

2. **환경 변수 설정**
   - 시스템 변수 `Path`에 `C:\gstreamer\1.0\mingw_x86_64\bin`을 추가합니다.

### 3. MSYS2 설치

MSYS2는 Windows에서 Unix 스타일의 개발 환경을 제공하는 도구입니다. 필요한 패키지와 라이브러리를 설치하기 위해 사용됩니다.

1. **MSYS2 다운로드 및 설치**
   - [MSYS2 공식 웹사이트](https://www.msys2.org/)에서 최신 설치 파일인 `msys2-x86_64-20240727.exe`를 다운로드합니다.
   - 다운로드한 설치 파일을 실행하여 MSYS2를 설치합니다.

2. **MSYS2 업데이트 및 패키지 설치**
   - **MSYS2 실행**: 설치가 완료된 후 MSYS2 터미널을 실행합니다.
   - **시스템 업데이트**:
     ```bash
     pacman -Su
     ```
   - **필요한 라이브러리 설치**:
     ```bash
     pacman -S mingw-w64-x86_64-pkg-config mingw-w64-x86_64-gstreamer mingw-w64-x86_64-gst-plugins-base mingw-w64-x86_64-gst-plugins-good mingw-w64-x86_64-gst-plugins-bad mingw-w64-x86_64-gst-plugins-ugly
     ```

3. **환경 변수 설정**
   - 시스템 변수 `Path`에 `C:\msys64\mingw64\bin`을 추가합니다.
