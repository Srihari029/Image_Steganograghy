#ifndef DECODE_H
#define DECODE_H

#include <stdlib.h>
#include <string.h>
#include "types.h" // Contains user defined types

typedef struct _DecodeInfo
{
	/* Destination Image info */
    	char *dsrc_image_fname; // file name 
    	FILE *fptr_dsrc_image; // file ptr or opening and closing img file

	/* Secret File Info */
    	char *dsecret_fname;
    	FILE *fptr_secret; // file ptr for secret file
	int secret_file_size; // size of the secret file
	uint secret_file_extn_size; // size of the secret file extension

} DecodeInfo;

/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], int argc, DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_dfiles(DecodeInfo *decInfo);

/* Skip the header file contents */
Status skip_bmp_header(DecodeInfo *decInfo);

/* Decode Magic string */
Status decode_magic_string(DecodeInfo * decInfo);

/* Decode bytes from LSB of image file bytes */
char decode_byte_from_lsb(char *buffer);

/* Decode size from LSB of image file bytes */
int decode_size_from_lsb(char *buffer);

/* Decode Secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode Secret file extension */
Status decode_secret_file_extn(DecodeInfo *decode);

/* Decode Secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);
#endif
