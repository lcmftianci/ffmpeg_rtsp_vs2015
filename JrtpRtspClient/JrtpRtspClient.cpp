#include <jrtplib3\rtcppacket.h>
#include <iostream>
#include "rtspClient.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
using std::cout;
using std::endl;

#ifndef _WIN32
#include <unistd.h>
#include <fcntl.h>
#else
#pragma comment(lib, "jrtplib.lib")
#pragma comment(lib, "jthread.lib")
#pragma comment(lib, "libJrtpRtspClient.lib")
#pragma comment(lib, "ws2_32.lib")
#endif

using namespace std;

//using std::cout;
//using std::endl;

bool ByeFromServerFlag = false;
void ByeFromServerClbk()
{
	cout << "Server send BYE" << endl;
	ByeFromServerFlag = true;
}
#if 1
int main(int argc, char* argv[])
{
	cout << "test rtspclient" << endl;
	string RtspUri("rtsp://192.168.47.1/Titanic.mkv");
	// string RtspUri("rtsp://192.168.81.145/ansersion");
	RtspClient Client;

	/* Set up rtsp server resource URI */
	Client.SetURI(RtspUri);

	/* Send DESCRIBE command to server */
	Client.DoDESCRIBE();

	/* Parse SDP message after sending DESCRIBE command */
	Client.ParseSDP();

	/* Send SETUP command to set up all 'audio' and 'video'
	* sessions which SDP refers. */
	Client.DoSETUP();

	printf("start PLAY\n");
	printf("SDP: %s\n", Client.GetSDP().c_str());

	/* Send PLAY command to play only 'video' sessions.*/
	Client.DoPLAY("video");

	/* Receive 1000 RTP 'video' packets
	* note(FIXME):
	* if there are several 'video' session
	* refered in SDP, only receive packets of the first
	* 'video' session, the same as 'audio'.*/
	int packet_num = 0;
	const size_t BufSize = 98304;
	uint8_t buf[BufSize];
	size_t size = 0;

	/* Write h264 video data to file "test_packet_recv.h264"
	* Then it could be played by ffplay */
#ifndef _WIN32
	int fd = open("test_packet_recv.h264", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);

	while (++packet_num < 1000) {
		if (!Client.GetMediaData("video", buf, &size, BufSize)) continue;
		if (fwrite(fd, buf, size) < 0) {
			perror("write");
		}
		if (ByeFromServerFlag) {
			break;
		}
		printf("recv %u\n", size);
	}
#else
	FILE* fp = fopen("test_packet_recv.h264", "wb");

	while (++packet_num < 1000) {
		if (!Client.GetMediaData("video", buf, &size, BufSize)) continue;
		if (fwrite(buf, 1, size, fp) < 0) {
			perror("write");
		}
		if (ByeFromServerFlag) {
			break;
		}
		printf("recv %u\n", size);
	}
#endif

	printf("start TEARDOWN\n");
	/* Send TEARDOWN command to teardown all of the sessions */
	Client.DoTEARDOWN();
	getchar();
	return 0;
}
#endif