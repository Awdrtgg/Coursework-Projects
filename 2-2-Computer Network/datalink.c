#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "protocol.h"
#include "datalink.h"

#define seq_nr unsigned int
#define packet unsigned char *

#define DATA_TIMER  2000
#define ACK_TIMER   1000

#define MAX_SEQ 7
#define NR_BUFS ((MAX_SEQ + 1)/2)

#define PHL_OVERLOAD (64 * 1024 - 2 * PKT_LEN)

typedef enum {false, true} bool;

typedef struct { 
    unsigned char kind;
    unsigned char ack;
    unsigned char seq;
    unsigned char data[PKT_LEN];   
    unsigned int  padding;
}FRAME;

static bool no_nak = true;
static seq_nr oldest_frame[NR_BUFS];

static seq_nr ack_expected = 0;
static seq_nr next_frame_to_send = 0;
static seq_nr frame_expected = 0;
static seq_nr too_far = NR_BUFS;

static packet out_buf[NR_BUFS];
static packet in_buf[NR_BUFS];
static unsigned int in_len[NR_BUFS];
static bool arrived[NR_BUFS];
static seq_nr nbuffered = 0;

static int phl_ready = 0;

static bool between(seq_nr a, seq_nr b, seq_nr c) 
{
    return ((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a));
}

static void put_frame(unsigned char *frame, int len)
{
    *(unsigned int *)(frame + len) = crc32(frame, len);
    send_frame(frame, len + 4);
    if (phl_sq_len() > PHL_OVERLOAD)
        phl_ready = 0;
}

static void send_some_frame(unsigned int frame_kind, seq_nr frame_nr, seq_nr frame_expected, packet buffer[])
{
    FRAME s;

    s.kind = frame_kind;
    s.seq = frame_nr;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

    if (frame_kind == FRAME_DATA)
		memcpy(s.data, buffer[frame_nr % NR_BUFS], PKT_LEN);
    if (frame_kind == FRAME_NAK) 
        no_nak = false;

    dbg_frame("Send FRAME %d %d, ID %d\n", s.seq, s.ack, *(short *)s.data);
    if (frame_kind == FRAME_DATA) {
        dbg_frame("Send FRAME %d %d, ID %d\n", s.seq, s.ack, *(short *)s.data);
        put_frame((unsigned char *)&s, 3 + PKT_LEN);
    }
    else {
	put_frame((unsigned char *)&s, 2); // why 2?
	if (frame_kind == FRAME_ACK)
	    dbg_frame("Send ACK  %d\n", s.ack);
	else
	    dbg_frame("Send NAK  %d\n", s.ack);
    }
    if (frame_kind == FRAME_DATA) {
		start_timer(frame_nr % NR_BUFS, DATA_TIMER);
		oldest_frame[frame_nr % NR_BUFS] = frame_nr % (MAX_SEQ + 1);
	}
    stop_ack_timer();
}


int main(int argc, char **argv)
{
    int event, arg;

    FRAME f;

    for (int i = 0; i < NR_BUFS; i++) {
        arrived[i] = false;
		out_buf[i] = (packet)malloc(sizeof(char)*PKT_LEN);
		in_buf[i] = (packet)malloc(sizeof(char)*PKT_LEN);
		in_len[i] = PKT_LEN;
		oldest_frame[i] = 0;
    }

    int len = 0;

    protocol_init(argc, argv); 
    lprintf("Designed by Xiao Yunming, build: " __DATE__"  "__TIME__"\n");

    disable_network_layer();

    for (;;) {
        event = wait_for_event(&arg);

        switch (event) {
        case NETWORK_LAYER_READY:
//log_printf("NETWORK_LAYER_READY\n");
            nbuffered++;
            get_packet(out_buf[next_frame_to_send % NR_BUFS]);
//log_printf("get packet from network layer\n");
            send_some_frame(FRAME_DATA, next_frame_to_send, frame_expected, out_buf);
//log_printf("send data frame to phsical layer, seq = %d, ack = %d\n", next_frame_to_send, frame_expected);
	    	next_frame_to_send = (next_frame_to_send + 1) % (MAX_SEQ + 1);
//log_printf("Send Window: ack_expected = %d, next_frame_to_send = %d, nbuffered = %d\n", ack_expected, next_frame_to_send, nbuffered);
            break;

        case PHYSICAL_LAYER_READY:
//log_printf("PHSICAL_LAYER_READY\n");
            phl_ready = 1;
            break;

        case FRAME_RECEIVED: 
//log_printf("FRAME_RECEIVED\n");
            len = recv_frame((unsigned char *)&f, sizeof f);
            if (len < 5 || crc32((unsigned char *)&f, len) != 0) {
                dbg_event("**** Receiver Error, Bad CRC Checksum\n");
				send_some_frame(FRAME_NAK, 0, frame_expected, out_buf); // frame_nr = 0 ???
//log_printf("**** Receiver Error, Bad CRC Checksum\n");
//log_printf("send nak frame to phsical layer, seq = 0, ack = %d\n", frame_expected);
                break;
            }
            if (f.kind == FRAME_DATA) {
                dbg_frame("Recv DATA %d %d, ID %d\n", f.seq, f.ack, *(short *)f.data);
		if ((f.seq != frame_expected) && no_nak == true) { // no_nak ???
		    send_some_frame(FRAME_NAK, 0, frame_expected, out_buf); // frame_nr = 0 ???
//log_printf("send nak frame to phsical layer, seq = 0, ack = %d\n", frame_expected);
}
		else 
		    start_ack_timer(ACK_TIMER);

		if (between(frame_expected, f.seq, too_far) && arrived[f.seq % NR_BUFS] == false) {
		    arrived[f.seq % NR_BUFS] = true;
//log_printf("frame %d arrived\n", f.seq % MAX_SEQ);
		    memcpy(in_buf[f.seq % NR_BUFS], f.data, PKT_LEN);
		    in_len[f.seq % NR_BUFS] = len - 7;
		    while (arrived[frame_expected % NR_BUFS] == true) {
//log_printf("send packet to network layer, seq = %d\n", frame_expected);
				put_packet(in_buf[frame_expected % NR_BUFS], in_len[frame_expected % NR_BUFS]); // attention! check lenth
				no_nak = true;
				arrived[frame_expected % NR_BUFS] = false;
				frame_expected = (frame_expected + 1) % (MAX_SEQ + 1);
				too_far = (too_far + 1) % (MAX_SEQ + 1);
//log_printf("Receive Window: frame_expected = %d, too_far = %d\n", frame_expected, too_far);
				start_ack_timer(ACK_TIMER);
	        }
		}
	    }
	    else if ((f.kind == FRAME_NAK) && between(ack_expected, (f.ack + 1) % (MAX_SEQ + 1), next_frame_to_send)) {
			send_some_frame(FRAME_DATA, (f.ack + 1) % (MAX_SEQ + 1), frame_expected, out_buf);
//log_printf("receive nak frame, ack = %d\n", (f.ack + 1) % (MAX_SEQ + 1));
//log_printf("send data frame to phsical layer, seq = %d, ack = %d\n", (f.ack + 1) % (MAX_SEQ + 1), frame_expected);
}
	    else if(f.kind = FRAME_ACK) {
			dbg_frame("Recv ACK  %d\n", f.ack);
//log_printf("receive ack frame, ack = %d\n", (f.ack + 1) % (MAX_SEQ + 1));
	    }
	    while (between(ack_expected, f.ack, next_frame_to_send)) {
//log_printf("receive piggybacking ack = %d, increase the ack_expected(current = %d)\n", f.ack, ack_expected);
			nbuffered = nbuffered - 1;
			stop_timer(ack_expected % NR_BUFS);
			ack_expected = (ack_expected + 1 ) % (MAX_SEQ + 1);
//log_printf("Send Window: ack_expected = %d, next_frame_to_send = %d, nbuffered = %d\n", ack_expected, next_frame_to_send, nbuffered);
	    }
            break; 

        case DATA_TIMEOUT:
//log_printf("DATA_TIMEOUT\n");
            dbg_event("---- DATA %d timeout\n", arg); 
            send_some_frame(FRAME_DATA, oldest_frame[arg], frame_expected, out_buf);
//log_printf("send data frame to phsical layer, seq = %d, ack = %d\n", arg, frame_expected);
            break;

	case ACK_TIMEOUT:
//log_printf("ACK_TIMEOUT\n");
	    send_some_frame(FRAME_ACK, 0, frame_expected, out_buf);
//log_printf("send ack frame to phsical layer, seq = %d, ack = %d\n", 0, frame_expected);
	    break;
	}

        if (nbuffered < 4 && phl_ready)
            enable_network_layer();
        else
            disable_network_layer();
   }
}
