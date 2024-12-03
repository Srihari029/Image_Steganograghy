#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* Get secret file size
 * Input: secret file ptr
 * Output: Size of file (using ile offset)
 */
uint get_secret_file_size(FILE *fptr_secr)
{
	// Set offset of file to end to get file size 
	fseek(fptr_secr,0,SEEK_END);
	return ftell(fptr_secr);
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

// To check operation encode or decode
OperationType check_operation_type(char *argv[],int argc){
	
	// If argument has less than 1 argument 
	if(argc < 2)
		return e_unsupported;
	
	// checks the second argument 
	if(strcmp (argv[1],"-e") == 0){
	   // if "-e" ---> encode 	
           if((argc >= 4) && (argc <= 5)){
                 return e_encode;
	   }
	   else
                return e_unsupported;
	}
	// checks the second argument 
	else if(strcmp (argv[1],"-d") == 0){
	   // if "-d" ---> decode 
	   if((argc >= 3 ) && (argc <= 4)){
                return e_decode;
           }
	   else
                return e_unsupported;
	}
	// if number of arguments is not 4 or 5
	else if((argc < 4) || (argc > 5))
		return e_unsupported;
	else
		return e_unsupported;
}

// function to validate the number of arguments and type of file format
Status read_and_validate_encode_args(char *argv[], int argc, EncodeInfo *encInfo)
{
	
	// pointer declaration for validation
	char* bmp = strstr(argv[2],".bmp");
	char* ext;	
	char* dest;
	
	// check source image is in ".bmp" file
	if(bmp != NULL)
	{
		if(strcmp(bmp,".bmp") == 0)
		{
			// store source image file
			encInfo->src_image_fname = argv[2];
			
			// for .txt file
			if(ext = strstr(argv[3],".txt"))
			{
				if(strcmp(ext,".txt") == 0)
				{
					// store secret text file 
					encInfo->secret_fname = argv[3];

					//store secret file extension
					strcpy(encInfo->extn_secret_file,".txt");
					//return e_success;	
				        
					// check whether it has 5th argument (destination file name)
					if(argc == 5)
					{
						dest = strstr(argv[4],".bmp");
						if(dest != NULL)
						{
               						 if(strcmp(dest,".bmp") == 0)
							 {
                      						// store destination image file
                        					encInfo->stego_image_fname = argv[4];
								return e_success;
							 }
							 else
								 return e_failure;
						}
						else
							return e_failure;
					}
					// if no destination file arguments is passed in CLA, create a file with .bmp extension
					else if(argc == 4)
					{
						encInfo->stego_image_fname = "dest_img.bmp";
						return e_success;
					}

				}
				else
					return e_failure;
			}

			// for .c file
			else if(ext = strstr(argv[3],".c"))
			{
                                if(strcmp(ext,".c") == 0)
				{
                                        // store secret text file 
                                        encInfo->secret_fname = argv[3];

                                        //store secret file extension
                                        strcpy(encInfo->extn_secret_file,".c");

					// check whether it has 5th argument (destination file name)
					if(argc == 5)
					{
                                                dest = strstr(argv[4],".bmp");
                                                if(dest != NULL)
						{
                                                         if(strcmp(dest,".bmp") == 0)
							 {
                                                                // store destination image file
                                                                encInfo->stego_image_fname = argv[4];
                                                                return e_success;
                                                         }
                                                         else
                                                                 return e_failure;
                                                }
                                                else
                                                        return e_failure;
                                        }
                                        // if no destination file arguments is passed in CLA, create a file with .bmp extension
                                        else if(argc == 4)
					{
                                                encInfo->stego_image_fname = "dest_img.bmp";
                                                return e_success;
                                        }
                                }
				else
					return e_failure;
			}

			// for .sh file
			else if(ext = strstr(argv[3],".sh"))
			{
                                if(strcmp(ext,".sh") == 0)
				{
                                        // store secret text file 
                                        encInfo->secret_fname = argv[3];

                                        //store secret file extension
                                        strcpy(encInfo->extn_secret_file,".sh");
                                        
					// check whether it has 5th argument (destination file name)
                                        if(argc == 5)
					{
                                                dest = strstr(argv[4],".bmp");
                                                if(dest != NULL)
						{
                                                         if(strcmp(dest,".bmp") == 0)
							 {
                                                                // store destination image file
                                                                encInfo->stego_image_fname = argv[4];
                                                                return e_success;
                                                         }
                                                         else
                                                                 return e_failure;
                                                }
                                                else
                                                        return e_failure;
                                        }
                                        // if no destination file arguments is passed in CLA, create a file with .bmp extension
                                        else if(argc == 4)
					{
                                                encInfo->stego_image_fname = "dest_img.bmp";
                                                return e_success;
                                        }
                                }
				else
					return e_failure;
			}
			// if none of the extension is present (".txt",".c",".sh")
			else
				return e_failure;

		}
		// if source image file is not in ".bmp" format
		else
			return e_failure;	
	}
	// if source image file is not in ".bmp" format
	else
		return e_failure;
}

// Function to check source image has enough capacity to encode secret file
Status check_capacity(EncodeInfo *encInfo)
{
	// calculate image file size	
	encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
	// calculate secret file size	
	encInfo->secret_file_size = get_secret_file_size(encInfo->fptr_secret);

	uint encode_things = 54 + (2 + 4 + strlen(encInfo->extn_secret_file) + 4 + encInfo->secret_file_size) * 8 ;
	
	// if image file size is greater than secret file data
	if(encInfo->image_capacity > encode_things)
		return e_success;
	// else return failure
	else
		return e_failure;
}

// function to copy header contents of img file (first 54 bytes)
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	rewind(fptr_src_image);
	// create buffer of size 54 bytes
	char buffer[54];
	fread(buffer,54,1,fptr_src_image);
	fwrite(buffer,54,1,fptr_dest_image);
	return e_success;
}

// Function to encode sizes to lsb of the op image file
Status encode_size_to_lsb(int data, char *size_buffer)
{
	int get,i;
        for(i=0;i<32;i++)
        {
                get = (data & (1<<i)) >> i ;              // get the required bit
                size_buffer[i] = size_buffer[i] & (~1);   // clear lsb of buffer bit we need to store
                size_buffer[i] = size_buffer[i] | get;    // store the bit we got from data
        }
	return e_success;
}

// Function to encode bytes to lsb of the op image file
Status encode_byte_to_lsb(char data, char *image_buffer)
{
	int get,i;
	for(i=0;i<8;i++)
	{
		get = (data & (1<<i)) >> i ;               // get the required bit
		image_buffer[i] = image_buffer[i] & (~1);  // clear lsb of buffer bit we need to store
		image_buffer[i] = image_buffer[i] | get;   // store the bit we got from data
	}
	return e_success;
}

// Function to encode data to image and set required buffer size for encoding
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
	int i;
	char buffer[8];
	// loop will run for size times
	for(i=0;i<size;i++)
	{
		fread(buffer,8,1,fptr_src_image);     // read 8 bytes from source image file
		encode_byte_to_lsb(data[i], buffer);  // encode the required data
		fwrite(buffer,8,1,fptr_stego_image);  // write the encoded 8 bytes to destination image file
	}
	return e_success;
}

// Function to encode magic string
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
	encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image);
	return e_success;	
}

// Function to encode secret file's extension size
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
	char buffer[32];
	fread(buffer, 32, 1, encInfo->fptr_src_image);     // read 32 bytes from source image
	encode_size_to_lsb(extn_size, buffer);             // encode extension size 
	fwrite(buffer, 32, 1, encInfo->fptr_stego_image);  // write the encoded 32 bytes to destination image
	return e_success;	
}

// Function to encode secret file extension
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image);
	return e_success;
}

// Function to encode secret file size
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
	char buffer[32];
        fread(buffer, 32, 1, encInfo->fptr_src_image);     // read 32 bytes from source image 
        encode_size_to_lsb(file_size, buffer);             // encode secret file size 
	fwrite(buffer, 32, 1, encInfo->fptr_stego_image);  // write the encoded 32 bytes to destination image 
	return e_success;
}

// Function to encode secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
	char file_data[encInfo->secret_file_size];
	// To move the file pointer to the starting position
	fseek(encInfo->fptr_secret, 0, SEEK_SET);
	fread(file_data, encInfo->secret_file_size, 1, encInfo->fptr_secret);
	encode_data_to_image(file_data, encInfo->secret_file_size, encInfo->fptr_src_image, encInfo->fptr_stego_image);
	return e_success;
}

// Function to copy remaining data to destination image file unaltered
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	char ch[1];
	while(fread(ch, 1, 1, fptr_src) == 1)
	{
		// read and write remaining bytes to destination image (1 byte by 1 byte)
		fwrite(ch, 1, 1,fptr_dest);
	}
	return e_success;
}

// perform encoding operation
Status do_encoding(EncodeInfo * encInfo)
{
	int sts = 0;
	// Open all files in required mode
	if(open_files(encInfo) == e_success)
	{
		printf("Info : Open required files\nDone\n");
		sts++;
		// check whether image file size is sufficient to encode secret data
        	if(check_capacity(encInfo) == e_success)
		{
			printf("Info : Check file capacity\nDone\n");
			sts++;
        		// function to copy header contents (first 54 bytes)
        		if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
			{
				printf("Info : Transfer header files\nDone\n");
				sts++;				
        			// function to encode magic string 
				if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
				{
					printf("Info : Magic string encode\nDone\n");
					sts++;
	        			// function to encode size of secret file extension
					if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
					{
						printf("Info : Secret file extension size encode\nDone\n");
						sts++;
  	      					// function to encode secret file extension
						if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
						{
							printf("Info : Secret file extension encode\nDone\n");
							sts++;
		        				// function to encode size of secret file
							if(encode_secret_file_size(encInfo->secret_file_size, encInfo) ==e_success)
							{
								printf("Info : Secret file size encode\nDone\n");
								sts++;
	        						// function to encode secret file data
								if(encode_secret_file_data(encInfo) == e_success)
								{
									printf("Info : Secret file data encode\nDone\n");
									sts++;
        								// function to copy remaining unaltered data 
									if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
									{
										printf("Info : Transfer remaining file contents\nDone\n");
										sts++;
									}
									else
										printf("Info : Transfer remaining file contents failed\n");
								}
								// Secret file size encoding failure
                                                        	else
                                                                	printf("Info : Secret file data encode failed\n");
							}
							// Secret file size encoding failure
							else
								printf("Info : Secret file size encode failed\n");
						}
						// Secret file extn encoding failure
						else
							printf("Info : Secret file extension encode failed\n");
					}
					// Extn size encoding failure 
					else
						printf("Info : Secret file extension size encode failed\n");

				}
				// Magic string encoding failure
				else
					printf("Info : Magic string encode failed\n");
			}
			// Header file copy failure
			else
				printf("Info : Transfer header files failed\n");
        	}
		// Image file size not sufficient
		else
			printf("Info : Check file capacity failed\n");
	}
	// Failure in file opening
	else
		printf("Info : Open required files failed\n");
        // if size is insufficient
	
	// if all function operations are performed successfully return success else failure
	if(sts == 9)
		return e_success;
	else
		return e_failure;
}
