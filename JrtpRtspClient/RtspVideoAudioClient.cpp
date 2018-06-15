#include <iostream>
#include "rtspClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#else
#pragma comment(lib, "jrtplib.lib")
#pragma comment(lib, "jthread.lib")
#pragma comment(lib, "libJrtpRtspClient.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "pthreadVC2.lib")
#endif

using std::cout;
using std::endl;

void * audio(void * args);
void * video(void * args);
#if 0
int main(int argc, char *argv[])
{
	pthread_t RecvVideo;
	pthread_t RecvAudio;

	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <URL>" << endl;
		cout << "For example: " << endl;
		cout << argv[0] << " rtsp://127.0.0.1/ansersion" << endl;
		return 1;
	}

	cout << "Start play " << argv[1] << endl;
	cout << "Then put video data into test_video_recv.h264; ";
	cout << "And put audio data into test_audio_recv.mp3" << endl;
	string RtspUri(argv[1]);

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

	/* Send PLAY command to play 'video' and 'audio' sessions.*/
	Client.DoPLAY();

	pthread_create(&RecvAudio, NULL, audio, (void*)(&Client));
	pthread_create(&RecvVideo, NULL, video, (void*)(&Client));
	pthread_join(RecvAudio, NULL);
	pthread_join(RecvVideo, NULL);

	printf("start TEARDOWN\n");
	/* Send TEARDOWN command to teardown all of the sessions */
	Client.DoTEARDOWN();

	return 0;
}
#endif

void * audio(void * args)
{
	/* Receive 1000 RTP 'video' packets
	* note(FIXME):
	* if there are several 'video' session
	* refered in SDP, only receive packets of the first
	* 'video' session, the same as 'audio'.*/
	int packet_num = 0;
	const size_t BufSize = 65534;
	uint8_t buf[BufSize];
	size_t size = 0;

	RtspClient * Client = (RtspClient*)args;

	/* Write h264 video data to file "test_packet_recv.h264"
	* Then it could be played by ffplay */
#ifndef _WIN32
	int fd = open("test_audio_recv.mp3", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);

	while (++packet_num < 1000) {
		if (!Client->GetMediaData("audio", buf, &size, BufSize)) continue;
		if (write(fd, buf, size) < 0) {
			perror("write");
		}
		printf("recv audio data %d: %u bytes\n", packet_num, size);
	}
#else
	FILE* fd = fopen("test_audio_recv.mp3", "wb");

	while (++packet_num < 1000) {
		if (!Client->GetMediaData("audio", buf, &size, BufSize)) continue;
		if (fwrite(buf, 1, size, fd) < 0) {
			perror("write");
		}
		printf("recv audio data %d: %u bytes\n", packet_num, size);
	}
#endif
	return NULL;
}

void * video(void * args)
{
	/* Receive 1000 RTP 'video' packets
	* note(FIXME):
	* if there are several 'video' session
	* refered in SDP, only receive packets of the first
	* 'video' session, the same as 'audio'.*/
	int packet_num = 0;
	const size_t BufSize = 65534;
	uint8_t buf[BufSize];
	size_t size = 0;

	RtspClient * Client = (RtspClient*)args;

	/* Write h264 video data to file "test_packet_recv.h264"
	* Then it could be played by ffplay */
#ifndef _WIN32
	int fd = open("test_video_recv.h264", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);

	while (++packet_num < 1000) {
		if (!Client->GetMediaData("video", buf, &size, BufSize)) continue;
		if (write(fd, buf, size) < 0) {
			perror("write");
		}
		printf("recv video data %d: %u bytes\n", packet_num, size);
	}
#else
	FILE* fd = fopen("test_video_recv.h264", "wb");

	while (++packet_num < 1000) {
		if (!Client->GetMediaData("video", buf, &size, BufSize)) continue;
		if (fwrite(buf, 1, size, fd) < 0) {
			perror("write");
		}
		printf("recv video data %d: %u bytes\n", packet_num, size);
	}
#endif
	return NULL;
}