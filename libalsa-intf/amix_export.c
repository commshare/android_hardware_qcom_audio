/*
 * amix_export.c
 *
 *  Created on: May 10, 2013
 *      Author: simingweng
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include "alsa_audio.h"
#include "amix_export.h"

struct mixer_ctl *get_ctl(struct mixer *mixer, char *name)
{
    char *p;
    unsigned idx = 0;

    if (isdigit(name[0]))
        return mixer_get_nth_control(mixer, atoi(name) - 1);

    p = strrchr(name, '#');
    if (p) {
        *p++ = 0;
        idx = atoi(p);
    }

    return mixer_get_control(mixer, name, idx);
}

int configure_mixer(int argc, char **argv)
{
    struct mixer *mixer;
    struct mixer_ctl *ctl;
    int r;
    const char* device = "/dev/snd/controlC0";

    mixer = mixer_open(device);
    if (!mixer){
        fprintf(stderr,"oops: %s: %d\n", strerror(errno), __LINE__);
        return -1;
    }

    if (argc < 2) {
    	fprintf(stderr,"invalid number of arguments\n");
        return -1;
    }

    ctl = get_ctl(mixer, argv[0]);
    argc -= 1;
    argv += 1;

    if (!ctl) {
        fprintf(stderr,"can't find control\n");
        mixer_close(mixer);
        return -1;
    }

	if (isdigit(argv[0][0]))
		r = mixer_ctl_set_value(ctl, argc, argv);
	else
		r = mixer_ctl_select(ctl, argv[0]);
	if (r)
		fprintf(stderr,"oops: %s: %d\n", strerror(errno), __LINE__);

    mixer_close(mixer);
    return 0;
}
