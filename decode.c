#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "decode.h"

/* Function Definitions */

// function to validate the number of arguments and type of file format
Status read_and_validate_decode_args(char *argv[], int argc, DecodeInfo *decInfo)
{
	char *bmp;
	char *ext;
	// check image is in ".bmp" file
        bmp = strstr(argv[2],".bmp");
	if(bmp != NULL)
        {
                if(strcmp(bmp,".bmp") == 0)
                {
			// store dest image file
                        decInfo->dsrc_image_fname = argv[2];
			//return e_success;
			if(argc == 4)
			{
				ext = strchr(argv[3],'.');
				if(ext == NULL)
				{
					decInfo->dsecret_fname = argv[3];
					return e_success;
				}
				else
					return e_failure;
			}
			else if(argc == 3)
			{
				decInfo->dsecret_fname = malloc(strlen("Itssecret") + 5);
       				if (decInfo->dsecret_fname == NULL)
        			{
                			fprintf(stderr, "Memory allocation for extn failed\n");
                			return e_failure;
  				}
				else
					strcpy(decInfo->dsecret_fname,"Itssecret");
				return e_success;
			}
		}
		// if .bmp format is not valid
		else
			return e_failure;
	}
	// if .bmp is not present
	else
		return e_failure;

}

// Function to decode size from LSB
int decode_size_from_lsb(char *buffer)
{
	int i,data = 0,get=0;
	// decode extn size from lsb
	for(i=0;i<32;i++)
	{
		get = buffer[i] & 1;
                data = data & ~(1 << i);
                data = data | (get << i);
	}
	return data;
}

// Function to decode bytes from LSB
char decode_byte_from_lsb(char *buffer)
{
	int i;
	char ch = 0,get;
	// decode magic string from lsb
	for(i=0;i<8;i++)
	{
		get = buffer[i] & 1;
		ch = ch & ~(1 << i);
		ch = ch | (get << i);
	}
	// return the decoded character
	return ch;
}

// Function to perform file opening in required mode 
Status open_dfiles(DecodeInfo *decInfo)
{
	// Opening image file
	decInfo->fptr_dsrc_image = fopen(decInfo->dsrc_image_fname,"r");
	// Error checking
	if (decInfo->fptr_dsrc_image == NULL)
    	{
    		perror("fopen");
       	 	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->dsrc_image_fname);

        	return e_failure;
    	}
	
	// If file is opened successfully
	return e_success;
}

// Functio to skip header files of image file
Status skip_bmp_header(DecodeInfo *decInfo)
{
	// move the file offset pointer to 54th byte
	fseek(decInfo->fptr_dsrc_image, 54, SEEK_SET);
	return e_success;
}

// Function to decode magic string
Status decode_magic_string(DecodeInfo *decInfo)
{
	printf("\n");
	int i,j=0;
	char magic_str[3], buffer[8],mg[3];
	// Decode magic string (2 characters)
	for(i=0;i<2;i++)
	{
		fread(buffer, 8, 1, decInfo->fptr_dsrc_image);
		magic_str[i] = decode_byte_from_lsb(buffer);
	}
	// Append null char at last
	magic_str[2] = '\0';

	// Read magic string from user and check both are same 
	// Max 3 attemps
	while(j<3)
	{
		printf("(%d) Attempts remaining !!\n",(3-j));
		printf("Enter magic string : ");
		scanf("%s",mg);
		if(strcmp(magic_str, mg) == 0)
		{
			printf("Magic string match successful !\n");
			break;
		}
		else
		{
			printf("Magic string match failed. Try Again !\n");
		}
		j++;
	}
	if(j == 3)
	{
		printf("Maximum attempts reached !\n");
		return e_failure;
	}
	else
		return e_success;
}

// Function to decode secret file extension size
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
	int i;
	char buffer[32];
	fread(buffer, 32, 1, decInfo->fptr_dsrc_image);
	// Get size of secret file extension
	decInfo->secret_file_extn_size = decode_size_from_lsb(buffer);
	return e_success;
}

// Function to decode secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
	char extn[(decInfo->secret_file_extn_size) + 1], buffer[8];
	int i;
	// Deocode secret file extension
	for(i=0; i < decInfo->secret_file_extn_size ;i++)
	{
		fread(buffer,8,1,decInfo->fptr_dsrc_image);
		extn[i] = decode_byte_from_lsb(buffer);
	}
	// append with null character 
	extn[decInfo->secret_file_extn_size] = '\0';
	
	// Append the extension with secret file name
	strcat(decInfo->dsecret_fname,extn);
	
	// Opening secret file
        decInfo->fptr_secret = fopen(decInfo->dsecret_fname,"w");
        // Error checking
        if (decInfo->fptr_secret == NULL)
        {
                perror("fopen");
                fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->dsecret_fname);

                return e_failure;
        }
	return e_success;
}

// Function decode size of secret file
Status decode_secret_file_size(DecodeInfo *decInfo)
{
	int i;
        char buffer[32];
        fread(buffer, 32, 1, decInfo->fptr_dsrc_image);
	// Get size of secret file
        decInfo->secret_file_size = decode_size_from_lsb(buffer);
        return e_success;
}

// Function to decode data of secret file
Status decode_secret_file_data(DecodeInfo *decInfo)
{
	int i;
	char buffer[8], ch=0;
	// Decode secret file data
	for(i=0;i <  decInfo->secret_file_size;i++)
	{
		fread(buffer, 8, 1, decInfo->fptr_dsrc_image);
		ch = decode_byte_from_lsb(buffer);
		fwrite(&ch, 1, 1, decInfo->fptr_secret);
	}
	return e_success;
}

// perform decoding operation
Status do_decoding(DecodeInfo * decInfo)
{
	int sts = 0;
	// Open all files in required mode
	if(open_dfiles(decInfo) == e_success)
	{
		printf("Info : Open required files\nDone\n");
		sts ++;
		// Skip the header file contents
		if(skip_bmp_header(decInfo) == e_success)
		{
			printf("Info : Header files decode\nDone\n");
			sts ++;
			// Decode magic string
			if(decode_magic_string(decInfo) == e_success)
			{
				printf("\nInfo : Magic string decode\nDone\n");
				sts ++;
				// Decode secret file extension size
				if(decode_secret_file_extn_size(decInfo) == e_success)
				{
					printf("Info : Secret file extension size decode\nDone\n");
					sts ++;
					// Decode secret file extension
					if(decode_secret_file_extn(decInfo) == e_success)
					{
						printf("Info : Secret file extension decode\nDone\n");
						sts ++;
						// Decode secret file size
						if(decode_secret_file_size(decInfo) == e_success)
						{
							printf("Info : Secret file size decode\nDone\n");
							sts ++;
							// Decode secret file data
							if(decode_secret_file_data(decInfo) == e_success)
							{
								printf("Info : Secret file Data decode\nDone\n");
								sts ++;
							}
							else
								printf("Info : Secret file Data decode failed\n");
						}
						else
							("Info : Secret file size decode failed\n");
					}
					else
						printf("Info : Secret file extension decode failed\n");
				}
				else
					printf("Info : Secret file extension size decode failed\n");
			}
			else
				printf("Info : Magic string decode failed\n");
		}
		else
			printf("Info : Header files decode failed\n");
	}
	else	
		printf("Info : Open required files failed\n");

	// If all the operation are success
	if(sts == 7)
		return e_success;
	else
		return e_failure;

	// deallocate the dynamically allocated memory
	free(decInfo->dsecret_fname);
}
